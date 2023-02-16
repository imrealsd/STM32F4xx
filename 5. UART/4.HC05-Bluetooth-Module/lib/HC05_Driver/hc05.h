/**
 ******************************************************************************
 * @author         : subahdeep Dhang
 * @file           : hc05.h
 * @brief          : bluetooth driver header
 ******************************************************************************
*/

/*Public Includes*/

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>

/*Driver Define*/
#ifndef HC05_H
#define HC05_H

#define MAX_COMMAND_LEN    50
#define MAX_RESPONSE_LEN   50
#define OK_RESPONSE_LEN    4
#define MAX_HC05_DELAY     300 // in ms

/* Enums */
typedef enum {

    HC05_OK   = 0,
    HC05_FAIL = 1

} HC05_StatusType;


/* Function Prototypes*/

HC05_StatusType HC05_verifyATMode(void);
HC05_StatusType HC05_backToDefaultMode(void);
HC05_StatusType HC05_getModuleInfo(char* const name, char* const address, char* const version, 
                                   char* const mode, char* const passwd, char* const uartSpeed);
HC05_StatusType HC05_FixedAddr_MasterMode(void);

#endif //  HC05_H