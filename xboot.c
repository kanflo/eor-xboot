/* 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Johan Kanflo (github.com/kanflo)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <espressif/esp_common.h>
#include <esp/uart.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ssid_config.h>
#include <espressif/esp_sta.h>
#include <espressif/esp_wifi.h>
#include <semphr.h>
#include <ota-tftp.h>
#include <rboot-integration.h>
#include <rboot.h>
#include <rboot-api.h>
#include <sysparam.h>

#include "xboot-cli.h"
#include "timeutils.h"

xSemaphoreHandle wifi_alive;

static void print_mac_and_ip(void)
{
	char msg[64];
	struct ip_info ipconfig;
    uint8_t hwaddr[6];

	if (!sdk_wifi_get_ip_info(STATION_IF, &ipconfig)) {
		printf("Failed to read my own IP address...\n");
	}
	if (!sdk_wifi_get_macaddr(STATION_IF, (uint8_t*) hwaddr)) {
		printf("Failed to read my own MAC address...\n");
	}
	snprintf((char*) msg, sizeof(msg), "%d.%d.%d.%d : [" MACSTR "]", ip4_addr1(&ipconfig), ip4_addr2(&ipconfig), ip4_addr3(&ipconfig), ip4_addr4(&ipconfig), MAC2STR(hwaddr));
	printf("%s\n", msg);
}

static void wifi_task(void *pvParameters)
{
	uint8_t status	= 0;
	uint8_t retries = 30;
	struct sdk_station_config config = {
		.ssid = WIFI_SSID,
		.password = WIFI_PASS,
	};

	printf("WiFi: connecting to WiFi\n");
	sdk_wifi_set_opmode(STATION_MODE);
	sdk_wifi_station_set_config(&config);

	while(1)
	{
		while ((status != STATION_GOT_IP) && (retries)){
			status = sdk_wifi_station_get_connect_status();
			printf("%s: status = %d\n", __func__, status );
			if( status == STATION_WRONG_PASSWORD ){
				printf("WiFi: wrong password\n");
				break;
			} else if( status == STATION_NO_AP_FOUND ) {
				printf("WiFi: AP not found\n");
				break;
			} else if( status == STATION_CONNECT_FAIL ) {
				printf("WiFi: connection failed\n");
				break;
			}
			vTaskDelay( 1000 / portTICK_RATE_MS );
			--retries;
		}
		if (status == STATION_GOT_IP) {
			printf("WiFi: Connected\n");
			xSemaphoreGive( wifi_alive );
			taskYIELD();
		}

		while ((status = sdk_wifi_station_get_connect_status()) == STATION_GOT_IP) {
			xSemaphoreGive( wifi_alive );
			taskYIELD();
		}
		printf("WiFi: disconnected\n");
		sdk_wifi_station_disconnect();
		vTaskDelay( 1000 / portTICK_RATE_MS );
	}
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
	rboot_config conf = rboot_get_config();
    printf("\n\nxboot (%d/%d)\n", conf.current_rom, conf.count);

    printf("Images in flash:\n");
    for(int i = 0; i <conf.count; i++) {
        printf("%c%d: offset 0x%08x\n", i == conf.current_rom ? '*':' ', i, conf.roms[i]);
    }
	print_mac_and_ip();

	ota_tftp_init_server(TFTP_PORT);
	enter_cli();

    vSemaphoreCreateBinary(wifi_alive);

    xTaskCreate(&wifi_task, (int8_t *)"wifi_task", 256, NULL, 2, NULL);
}
