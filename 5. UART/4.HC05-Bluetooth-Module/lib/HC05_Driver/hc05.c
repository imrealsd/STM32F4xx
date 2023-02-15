/**
 ******************************************************************************
 * @author         : subahdeep Dhang
 * @file           : hc05.c
 * @brief          : bluetooth driver source
 ******************************************************************************
*/

#include "hc05.h"

/**
 * MCU VCC   : HC05 VCC
 * MCU GND   : HC05 GND
 * UART2 : stm32 to hc05 communication [baud rate 38400] 
*/


/**
 * press & hold the the EN button on hc05 when powering on : boot into AT-CMD mode [led blink once in 2 sec]
 * normal powering on : boot into data mode [led blink continiously waiting for connection]
*/


HC05_StatusType HC05_verifyATMode(void)
{
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    
    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+ROLE=1\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), MAX_HC05_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, sizeof(rxMsg),  MAX_HC05_DELAY);

    if (strncmp(rxMsg, "OK", 2) == 0){
        return HC05_OK;
    }
    return HC05_FAIL;
} 



HC05_StatusType HC05_enterDataMode(void)
{   
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];

    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+RESET\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), MAX_HC05_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, sizeof(rxMsg), MAX_HC05_DELAY);

    if (strncmp(rxMsg, "OK", 2) == 0){
        return HC05_OK;
    }
    return HC05_FAIL;
}


HC05_StatusType HC05_getModuleInfo(char* const name, char* const address, 
                                   char* const version, char* const mode)
{   
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    char *usrPassedMemAddr = NULL;

    uint8_t index = 0;
    uint8_t count = 1;
    uint8_t totalParameters = 4;
   
    memset(name, 0, MAX_RESPONSE_LEN);
    memset(address, 0, MAX_RESPONSE_LEN);
    memset(version, 0, MAX_RESPONSE_LEN);
    memset(mode, 0, MAX_RESPONSE_LEN);


    while (count <= totalParameters){

        memset(txMsg, 0, MAX_COMMAND_LEN);
        memset(rxMsg, 0, MAX_RESPONSE_LEN);

        switch (count){
            case 1:
                strcpy(txMsg, "AT+NAME?\r\n");
                usrPassedMemAddr = name;
                break;
            case 2:
                strcpy(txMsg, "AT+ADDR?\r\n");
                usrPassedMemAddr = address;
                break;
            case 3:
                strcpy(txMsg, "AT+VERSION?\r\n");
                usrPassedMemAddr = version;
                break;
            case 4:
                strcpy(txMsg, "AT+ROLE?\r\n");
                usrPassedMemAddr = mode;
                break;
        }
        HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), MAX_HC05_DELAY);
        HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, sizeof(rxMsg),  MAX_HC05_DELAY);
        strncpy(usrPassedMemAddr, rxMsg, MAX_RESPONSE_LEN);

        index = 0;
        while (name[index] != '\0'){index++;}
        if (name[index-4] != 'O' || name[index-3] != 'K')
            return HC05_FAIL;

        count++;
    }
    return HC05_OK;
}



