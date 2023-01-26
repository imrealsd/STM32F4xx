#include <Arduino.h>
#include <driver/gpio.h>
#include "driver/twai.h"
#include "esp_err.h"

uint64_t gCount = 0;

static void esp32_setup_TWAI(void);

void setup()
{	
	delay(1000);
	Serial.begin(115200);
	esp32_setup_TWAI();
}

void loop()
{	
	twai_message_t recvMsg;

	if (twai_receive(&recvMsg, pdMS_TO_TICKS(10000)) == ESP_OK){
		gCount++;
		Serial.print(gCount);
		Serial.println(" No. Msg Received");
	}
}


static void esp32_setup_TWAI(void)
{	
	twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_22, GPIO_NUM_23, TWAI_MODE_NORMAL);
  	twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  	twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
	
	if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK){
		Serial.println("TWAI Driver Initialised");

	} else {
		Serial.println("Failed to initialise TWAI driver");
	}

	if (twai_start() == ESP_OK){
		Serial.println("TWAI Driver Started");

	} else {
		Serial.println("Failed to Start TWAI driver");
	}
}