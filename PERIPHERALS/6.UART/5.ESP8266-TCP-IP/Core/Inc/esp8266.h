#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

#define TX_MSG_SIZE        100
#define OK_RESPONSE_SIZE   2
#define STA_MODE           1
#define AP_MODE            2
#define AP_STA_MODE        3
 
typedef enum {
    Esp8266_OK = 1,
    Esp8266_FAIL = 0
} Esp8266_Status;

Esp8266_Status esp8266_init(void);
