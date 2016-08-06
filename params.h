#ifndef _PARAMS_H_

#define _PARAMS_H_

// All parameters are stored as strings and each parameter below has a
// description of how that parameter shoud be interpreted (string, int or
// bool)

//
// User definable parameters
//

// Wifi credentials for this device, used when not in access point mode
#define PARAM_SSID_NAME "wifi.ssid.name" // string
#define PARAM_SSID_PASS "wifi.ssid.pass" // string

// Name of this node
#define PARAM_NODE_NAME "node.name" // string

// True if this node talks to an MQTT server
#define PARAM_MQTT_SERVER_ENABLE "mqtt.server.enable" // bool
// IP and port of MQTT server
#define PARAM_MQTT_SERVER_IP "mqtt.server.ip" // string
#define PARAM_MQTT_SERVER_PORT "mqtt.server.port" // uint32_t


//
// System parameters, should not be changed by the user
//


// Id of this node
#define PARAM_NODE_ID "node.id" // uint32_t

// Type of node
#define PARAM_NODE_TYPE "node.type" // string

// Name prefix (used when naming the access point)
#define PARAM_NODE_NAME_PREFIX "node.prefix" // string

// SPIFFS enabled
#define PARAM_SPIFFS_ENABLE "spiffs.enable" // bool
// SPIFFS start address in internal flash
#define PARAM_SPIFFS_START "spiffs.start" // uint32_t
// SPIFFS size in bytes
#define PARAM_SPIFFS_SIZE "spiffs.size" // uint32_t

// AES256 key
#define PARAM_AES256_KEY "key.aes" // string
// Public key in PEM format
#define PARAM_PUBKEY_PEM "key.pub" // string
// Private key in PEM format
#define PARAM_PRIVKEY_PEM "key.priv" // string

// Enable insecure TFTP server in xboot
#define PARAM_TFTP_SERVER_ENABLE "tftp.server.enable" // bool

// Enable core dumping for this node
#define PARAM_COREDUMP_SERVER_ENABLE "coredump.server.enable" // bool
// IP and port of core dump server
#define PARAM_COREDUMP_SERVER_IP "coredump.server.ip" // string
#define PARAM_COREDUMP_SERVER_PORT "coredump.server.port" // uint32_t

// Enable FOTA for this node
#define PARAM_FOTA_SERVER_ENABLE "fota.server.enable" // bool
// IP and port of FOTA server
#define PARAM_FOTA_SERVER_IP "fota.server.ip" // string
#define PARAM_FOTA_SERVER_PORT "fota.server.port" // uint32_t

// Wifi AP mode. If param is true then start as access point
#define PARAM_WIFI_AP_ENABLE "wifi.sta.enable" // bool
// Name and password of network when in AP mode
#define PARAM_WIFI_AP_SSID_NAME "wifi.sta.ssid.name" // string
#define PARAM_WIFI_AP_SSID_PAS "wifi.sta.ssid.pass" // string


// UI parameters for master reset button and LED

// Set to true when the device has a master reset button
#define PARAM_UI_BUTTON_ENABLED "ui.btn.enable" // bool
// GPIO pin where master reset button is connected (unless ui.btn.adc is set)
#define PARAM_UI_BUTTON_GPIO "ui.btn.gpio" // uint32_t
// Level of pin when the button is pressed
#define PARAM_UI_BUTTON_GPIO_ACTIVE_VALUE "ui.btn.gpio.active" // bool

// Set to true when master reset button is connected to adc (if available)
#define PARAM_UI_BUTTON_ADC "ui.btn.adc" // bool
// ADC threshold for button pressed
#define PARAM_UI_BUTTON_ADC_THRESHOLD "ui.btn.adc.thresh" // uint32_t
// True if button is pressed above threshold, false if button pressed below
#define PARAM_UI_BUTTON_ADC_THRESHOLD_GT "ui.btn.adc.thresh.gt" // bool

// Set to true when the device has an indicator LED
#define PARAM_UI_LED_ENABLED "ui.led.enable" // bool
// GPIO pin where LED is connected
#define PARAM_UI_LED_GPIO    "ui.led.gpio" // uint32_t
// Level of pin when the LED is on
#define PARAM_UI_LED_GPIO_ACTIVE_VALUE "ui.led.gpio.active" // bool

#endif // _PARAMS_H_
