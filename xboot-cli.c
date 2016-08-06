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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <espressif/spi_flash.h>
#include <esp/uart.h>
#include <sysparam.h>
#include <cli.h>

#include "xboot-cli.h"
#include "params.h"

#include "timeutils.h"

static void print_text_value(char *key, char *value)
{
    printf("  '%s' = '%s'\n", key, value);
}

static void print_binary_value(char *key, uint8_t *value, size_t len)
{
    size_t i;
    printf("  %s:", key);
    for (i = 0; i < len; i++) {
        if (!(i & 0x0f)) {
            printf("\n   ");
        }
        printf(" %02x", value[i]);
    }
    printf("\n");
}

static sysparam_status_t dump_params(void) {
    sysparam_status_t status;
    sysparam_iter_t iter;

    status = sysparam_iter_start(&iter);
    if (status < 0) return status;
    while (true) {
        status = sysparam_iter_next(&iter);
        if (status != SYSPARAM_OK) break;
        if (!iter.binary) {
            print_text_value(iter.key, (char *)iter.value);
        } else {
            print_binary_value(iter.key, iter.value, iter.value_len);
        }
    }
    sysparam_iter_end(&iter);

    if (status == SYSPARAM_NOTFOUND) {
        // This is the normal status when we've reached the end of all entries.
        return SYSPARAM_OK;
    } else {
        // Something apparently went wrong
        return status;
    }
}

static void wssid_name_cmd(uint32_t argc, char *argv[])
{
	sysparam_status_t status = sysparam_set_string(SSID_NAME_PARAM, argv[1]);
	if (status == SYSPARAM_OK) {
		printf("ok\n");
	} else {
		printf("Error %d\n", status);
	}
}

static void rssid_name_cmd(uint32_t argc, char *argv[])
{
    char *ssid;
	sysparam_status_t status = sysparam_get_string(SSID_NAME_PARAM, &ssid);
	if (status == SYSPARAM_OK) {
		printf("%s:%s\n", SSID_NAME_PARAM, ssid);
	    free(ssid);
	} else {
		printf("Error %d\n", status);
	}
}

static void wssid_pass_cmd(uint32_t argc, char *argv[])
{
	sysparam_status_t status = sysparam_set_string(SSID_PASS_PARAM, argv[1]);
	if (status == SYSPARAM_OK) {
		printf("ok\n");
	} else {
		printf("Error %d\n", status);
	}
}

static void rssid_pass_cmd(uint32_t argc, char *argv[])
{
    char *ssid;
	sysparam_status_t status = sysparam_get_string(SSID_PASS_PARAM, &ssid);
	if (status == SYSPARAM_OK) {
		printf("%s:%s\n", SSID_PASS_PARAM, ssid);
	    free(ssid);
	} else {
		printf("Error %d\n", status);
	}
}

static void dump_cmd(uint32_t argc, char *argv[])
{
	printf("Dumping sysparam\n");
	sysparam_status_t status = dump_params();
	if (SYSPARAM_OK == status) {
		printf("OK\n");
	} else {
		printf("Dump failed, %d\n", status);
	}
}

static void format_cmd(uint32_t argc, char *argv[])
{
	printf("Formatting sysparam\n");
    uint32_t base_addr, num_sectors;
    sysparam_status_t status = sysparam_get_info(&base_addr, &num_sectors);
    if (status != SYSPARAM_OK) {
        printf("No current sysparam region (initialization problem during boot?)]\n");
        // Default to the same place/size as the normal system initialization
        // stuff, so if the user uses this utility to reformat it, it will put
        // it somewhere the system will find it later
        num_sectors = DEFAULT_SYSPARAM_SECTORS;
        base_addr = sdk_flashchip.chip_size - (4 + num_sectors) * sdk_flashchip.sector_size;
    }

	status = sysparam_create_area(base_addr, num_sectors, true);
	if (status == SYSPARAM_OK) {
	    status = sysparam_init(base_addr, 0);
	}
	if (SYSPARAM_OK == status) {
		printf("OK\n");
	} else {
		printf("Formatting failed, %d\n", status);
	}
}

void enter_cli(void)
{
	bool start_cli = false;
	uint32_t start = systime_ms();
	do {
		if (uart_getc_nowait(0) == ':') {
			start_cli = true;
			break;
		}
	} while(systime_ms() - start < CLI_TIMEOUT_MS);

	if (start_cli) {
	  const command_t cmds[] = {
			CLI_CMD_USAGE("wsn",    wssid_name_cmd, 1, 1, "Set ssid name", "<ssid name>"),
			CLI_CMD      ("rsn",    rssid_name_cmd, 0, 0, "Get ssid name"),
			CLI_CMD_USAGE("wsp",    wssid_pass_cmd, 1, 1, "Set ssid password", "<ssid pass>"),
			CLI_CMD      ("rsp",    rssid_pass_cmd, 0, 0, "Get ssid password"),
			CLI_CMD      ("dump",   dump_cmd,       0, 0, "Dump sysparam"),
			CLI_CMD      ("format", format_cmd,     0, 0, "Format sysparam"),
	  };
	  cli_run(cmds, sizeof(cmds) / sizeof(command_t), "the xboot cli");
	}
}
