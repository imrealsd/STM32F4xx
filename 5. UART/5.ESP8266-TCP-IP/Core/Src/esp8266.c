#include "esp8266.h"

Esp8266_Status esp8266_init(void)
{   
    char msg[TX_MSG_SIZE];

    memset(msg, 0, TX_MSG_SIZE);
    strcpy(msg, "AT\n\r");
    HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart1, (uint8_t *) msg, OK_RESPONSE_SIZE, HAL_MAX_DELAY);
    if (strncmp(msg, "OK", OK_RESPONSE_SIZE) != 0) {
        return Esp8266_FAIL;
    }

    strcpy(msg, "AT+RESET\n\r");
    HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart1, (uint8_t *) msg, OK_RESPONSE_SIZE, HAL_MAX_DELAY);
    if (strncmp(msg, "OK", OK_RESPONSE_SIZE) != 0) {
        return Esp8266_FAIL;
    }
    
    strcpy(msg, "AT+RESTORE\n\r");
    HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart1, (uint8_t *) msg, OK_RESPONSE_SIZE, HAL_MAX_DELAY);
    if (strncmp(msg, "OK", OK_RESPONSE_SIZE) != 0) {
        return Esp8266_FAIL;
    }

    return Esp8266_OK;
}


Esp8266_Status esp8266_setMode(uint8_t mode) 
{   
}