/**
 ******************************************************************************
 * @author         : subahdeep Dhang
 * @file           : hc05.c
 * @brief          : bluetooth driver source
 ******************************************************************************
*/

#include "hc05.h"

/*static functions*/
static void convertIntToString(char* str, uint16_t num);

/**
 * MCU VCC   : HC05 VCC
 * MCU GND   : HC05 GND
 * UART      : stm32 to hc05 communication [baud rate 38400] 
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
 *    connect to provided fix slave address using 'BIND',then power reset _|  [conection built in data mode after leaving at mode]
 *                                                                                                     _
 * 4. MASTER -> 'CMODE' = 1 , ROLE = 1:                                                                 |
 *    SLAVE  -> 'CMODE' = 1 , ROLE = 0:                                                                 |
 *    connect to any address by SPP 'INIT' -> 'INQ' -> 'RNAME' -> 'PAIR' -> 'LINK' [master side]        |-- Scan Connect
 *    SPP INIT -> STATE-PARIABLE -> STATE-PAIRED -> STATE-CONNECTED [slave side]                       _|   [connection built in at mode]
 *                                                                                                         
 * 5. Master & Slave password have be same in-oder to connect / pair
 * 6. This library only implements quick connect mode
*/

/**
 * @brief verify at mode 
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


/**
 * @brief perform a factory reset 
 */
HC05_StatusType HC05_backToDefaultMode(void)
{   
    /**
     * ORGL / Default Mode:
     * NAME = HC-05
     * ROLE  = 0 [slave]
     * CMODE = 0
     * PASS-Key = "1234"
     * Baud Rate [AT-CMD] = 38400
     * Baud Rate [DATA] = 9600 [can be modified]
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


/**
 * @brief get module information
 */
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

/**
 * @brief module as master [will connect to a fixed address (slave) ]
 */
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


/**
 * @brief bind with the address in fixed address salve mode [will connect to the slave automatically when availbale]
 */
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
    strncat(txMsg, slaveAddr, 14);
    strcat(txMsg,"\r\n");

    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) != 0){
        return HC05_FAIL;
    }
    return HC05_OK;
}


/**
 * @brief configure module as slave device 
 */
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


/**
 * initialise spp profile
*/
HC05_StatusType HC05_initSppProfile(void)
{   
    /**
     * In some modules the spp is initialised automatically & calling this func will give ERROR[0]
     * So modeling as : any response is a success [ok / ERROR[0]] , except 'FAIL'
    */
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];

    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+INIT\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  MAX_HC05_DELAY);

    if (strncmp(rxMsg, "FAIL", 4) == 0){
        return HC05_FAIL;
    }
    return HC05_OK;
}



/**
 * @brief get bluetooth state [iniatilised, pairable, paired ..etc]
 * @note send AT+INIT before calling this function
*/
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

/**
 * @brief power reset [go back to data mode if booted into at mode] 
 */
HC05_StatusType HC05_powerReset(void)
{
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    
    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+RESET\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) == 0){
        return HC05_OK;
    }
    return HC05_FAIL;
}

/**
 * @brief set bluetooth module name 
 */
HC05_StatusType HC05_setModuleName(char* const name)
{
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    
    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+NAME=");
    strcat(txMsg, name);
    strncat(txMsg, "\r\n", 2);
    
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) == 0){
        return HC05_OK;
    }
    return HC05_FAIL;
}


/**
 * @brief set uart data speed [baud rate] 
 */
HC05_StatusType HC05_setUartSpeed(uint16_t baudRate)
{
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    char speed[7];
    
    convertIntToString(speed, baudRate);
    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+UART=");
    strncat(txMsg, speed, 7);
    strncat(txMsg,",0,0\r\n", 6);

    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, OK_RESPONSE_LEN,  HAL_MAX_DELAY);

    if (strncmp(rxMsg, "OK", 2) == 0){
        return HC05_OK;
    }
    return HC05_FAIL;
}


/**
 * @brief function return the binded adddress 
 */
HC05_StatusType HC05_getBindedAddress(char* const bindAddr)
{
    char txMsg[MAX_COMMAND_LEN];
    char rxMsg[MAX_RESPONSE_LEN];
    uint8_t index;
    
    memset(txMsg, 0, MAX_COMMAND_LEN);
    memset(rxMsg, 0, MAX_RESPONSE_LEN);

    strcpy(txMsg, "AT+BIND?\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *) txMsg, strlen(txMsg), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart2, (uint8_t *) rxMsg, sizeof(rxMsg),  MAX_HC05_DELAY);

    index = 0;
        while (rxMsg[index] != '\0'){index++;}
        if (rxMsg[index-4] != 'O' || rxMsg[index-3] != 'K')
            return HC05_FAIL;

    strncpy(bindAddr, rxMsg, MAX_RESPONSE_LEN);
    return HC05_OK;
}





/***** Private functions *******/

static void convertIntToString(char* str, uint16_t num)
{   
    /**
     * Max baudrate is 7 digits
     * current implementation of this func. is a bit ugly but working fine
     * will implement this nicely with loops , later
    */
    int8_t d0 = (num  % 10);
    int8_t d1 = ((num / 10) % 10);
    int8_t d2 = ((num / 100) % 10);
    int8_t d3 = ((num / 1000) % 10);
    int8_t d4 = ((num / 10000) % 10);
    int8_t d5 = ((num / 100000) % 10);
    int8_t d6 = ((num / 1000000) % 10);

    str[0] = d6 + 48;
    str[1] = d5 + 48;
    str[2] = d4 + 48;
    str[3] = d3 + 48;
    str[4] = d2 + 48;
    str[5] = d1 + 48;
    str[6] = d0 + 48;
}



// TODO: 1. add functions for scan-connect mode  [for both master & slave]