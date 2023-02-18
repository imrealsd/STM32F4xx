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
 * UART2     : stm32 to hc05 communication [baud rate 38400] 
*/


/**
 * 1. Press & hold the the EN button on hc05 when powering on : boot into AT-CMD mode [led blink once in 2 sec]
 *    In AT-CMD mode master or slave mode can be selected & the all the parameters [name,paswd..ect] can be modified
 *    AT + RESET or Power Reset to go-back to data communication mode
 * 
 * 2. Normal powering on : boot into data mode 
 *                                                                        _
 * 3. MASTER -> 'CMODE' = 0 , ROLE = 1:                                    |
 *    SLAVE ->  'CMODE' = 1,  ROLW = 0:                                    |-- Quick Connect
 *    connect to provided fix slave address using 'BIND'                  _|
 *                                                                                                     _
 * 4. MASTER -> 'CMODE' = 1 , ROLE = 1:                                                                 |
 *    SLAVE  -> 'CMODE' = 1 , ROLE = 0:                                                                 |
 *    connect to any address by SPP 'INIT' -> 'INQ' -> 'RNAME' -> 'PAIR' -> 'LINK' [master side]        |-- Scan Connect
 *    SPP INIT -> STATE-PARIABLE -> STATE-PAIRED -> STATE-CONNECTED [slave side]                       _|
 *                                                                                                  
 * 5. Master & Slave password have be same in-oder to connect / pair
*/


HC05_StatusType HC05_verifyATMode(void)
{
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    
    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) == 0){
        return HC05_OK;
    }
    return HC05_FAIL;
} 



HC05_StatusType HC05_backToDefaultMode(void)
{   
    /**
     * ORGL / Default Mode:
     * ROLE  = 0 [slave]
     * Key = "1234"
     * Baud Rate = 38400
    */
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];

    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+ORGL\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) == 0){
        return HC05_OK;
    }
    return HC05_FAIL;
}



HC05_StatusType HC05_getModuleInfo(char* const name, char* const address, 
                char* const version, char* const mode, char* const passwd, char* const uartSpeed)
{   
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    char *usrPassedMemAddr = NULL;

    uint8_t index = 0;
    uint8_t count = 1;
    uint8_t totalParameters = 6;
   
    memset(name, 0, MAX_RESPONSE_LEN);
    memset(address, 0, MAX_RESPONSE_LEN);
    memset(version, 0, MAX_RESPONSE_LEN);
    memset(mode, 0, MAX_RESPONSE_LEN);
    memset(passwd, 0, MAX_RESPONSE_LEN);
    memset(uartSpeed, 0, MAX_RESPONSE_LEN);


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
            case 5:
                strcpy(txMsg, "AT+PSWD?\r\n");
                usrPassedMemAddr = passwd;
                break;
            case 6:
                strcpy(txMsg, "AT+UART?\r\n");
                usrPassedMemAddr = uartSpeed;
                break;
        }

        /*Using MAX_HC05_DELAY cause response size is unknown , communication is expected to be finished within that time
        using HAL_MAX_DELAY will hangup forever waiting for rx_complete_flag 
        [casuse response size < max response size and func wait forever]*/

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



HC05_StatusType HC05_fixedAddr_masterModeEnter(void)
{   
    /**
     * RMMAD : Delete Paired List
     * ROLE  : 1 (master)
     * CMODE : 0 (connect to specific address) (mention the address using 'BIND')
    */
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];

    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+RMAAD\r\n");
    /*using HAL_MAX_DELAY , response size is known , function will return after getting fixed no. of bytes
    program will not hangup here as long as we got response of that size*/
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) != 0){
        return HC05_FAIL;
    }

    memset(txMsg, 0, MAX_COMMAND_LEN);
    strcpy(txMsg, "AT+ROLE=1\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) != 0){
        return HC05_FAIL;
    }
   

    memset(txMsg, 0, MAX_COMMAND_LEN);
    strcpy(txMsg, "AT+CMODE=0\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) != 0){
        return HC05_FAIL;
    }
    
    return HC05_OK;
}


HC05_StatusType HC05_fixedAddr_masterModeBind(char* const slaveAddr)
{   
    /**
     * IN CMODE = 0 Mode : connect to specific address
     * BIND [Address to be connected] : Connect to address specified by BIND cmd
    */
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];

    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+BIND=");
    strcat(txMsg, slaveAddr);
    strcat(rxMsg,"\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) != 0){
        return HC05_FAIL;
    }
    return HC05_OK;
}



HC05_StatusType HC05_slaveModeEnter(void)
{   
    /**
     * RMMAD : Delete Paired List
     * ROLE  : 0 (slave)
     * CMODE : 1 (connect to any address)
    */
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];

    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+RMAAD\r\n");
    /*using HAL_MAX_DELAY , response size is known , function will return after getting fixed no. of bytes
    program will not hangup here as long as we got response of that size*/
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) != 0){
        return HC05_FAIL;
    }

    memset(txMsg, 0, MAX_COMMAND_LEN);
    strcpy(txMsg, "AT+ROLE=0\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) != 0){
        return HC05_FAIL;
    }
   

    memset(txMsg, 0, MAX_COMMAND_LEN);
    strcpy(txMsg, "AT+CMODE=1\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) != 0){
        return HC05_FAIL;
    }
    return HC05_OK;
}



HC05_StatusType HC05_getModuleState(char* const state)
{
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    uint8_t index = 0;

    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+STATE?\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, sizeof(rxMsg),  MAX_HC05_DELAY);

   
    while (rxMsg[index] != '\0'){index++;}
    if (rxMsg[index-4] != 'O' || rxMsg[index-3] != 'K'){
        return HC05_FAIL;
    }
    strncpy(state, rxMsg, MAX_RESPONSE_LEN);
    return HC05_OK;
}
