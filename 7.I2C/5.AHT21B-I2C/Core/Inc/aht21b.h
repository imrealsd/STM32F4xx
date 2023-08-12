#include "stm32f4xx_hal.h"
#include "i2c.h"
#include <stdint.h>
#include <string.h>
#include <math.h>

#define AHT21B_ADDRESS      0x38
#define CMD_GET_STATUS      0x71
#define CMD_TRIG_CONV       0xAC
#define TRIG_CONV_ARG1      0x33
#define TRIG_CONV_ARG2      0x00
#define STATUS_OK           0x18
#define SIZE_TRIG_CONV_CMD  3
#define SIZE_SESNOR_DATA    6
#define ONE_BYTE            8
#define ONE_NIBBLE          4
#define UPPER_NIBBLE        0xF0
#define LOWER_NIBBLE        0x0F


void AHT21B_setup(void);
float AHT21B_getHumidity(void);
float AHT21B_getTemparature(void);

