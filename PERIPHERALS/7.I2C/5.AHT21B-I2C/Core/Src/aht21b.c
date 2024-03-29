#include "aht21b.h"


/*private function prototypes*/
static float AHT21B_convertRawHumidity(uint32_t* pRawHumidity);
static float AHT21B_convertRawTemparature(uint32_t* pRawTemparature);
static void AHT21B_getRawValue(uint32_t* pRawTemparature, uint32_t* pRawHumidity);

/**
 * @brief check status of AHT21B 
*/
void AHT21B_setup(void)
{   
    uint8_t txData  = CMD_GET_STATUS;
    uint8_t rxData  = 0x00; 

    HAL_Delay(100);
    HAL_I2C_Master_Transmit(&hi2c1, AHT21B_ADDRESS, &txData, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, AHT21B_ADDRESS, &rxData, 1, HAL_MAX_DELAY);
    if (rxData != STATUS_OK) {
        // yet to be implemented
    }
}

/**
 * @brief read Temparature from AHT21B
*/
float AHT21B_getTemparature(void)
{
    uint32_t rawTemp  = 0;
    uint32_t rawHum   = 0;
    float celciusTemp = 0;

    AHT21B_getRawValue(&rawTemp, &rawHum);
    celciusTemp = AHT21B_convertRawTemparature(&rawTemp);
    return celciusTemp;
}

/**
 * @brief read Humidity from AHT21B
*/
float AHT21B_getHumidity(void)
{
    uint32_t rawTemp = 0;
    uint32_t rawHum  = 0;
    float humidity = 0;

    AHT21B_getRawValue(&rawTemp, &rawHum);
    humidity = AHT21B_convertRawHumidity(&rawHum);
    return humidity;
}   

/**
 * @brief read raw values of Temparature & Humidity from AHT21B
*/
static void AHT21B_getRawValue(uint32_t* pRawTemparature, uint32_t* pRawHumidity)
{
    uint8_t  txBuff[SIZE_TRIG_CONV_CMD];
    uint8_t  rxBuff[SIZE_SESNOR_DATA];
    uint32_t rawTemparature = 0x00;
    uint32_t rawHumidity = 0x00;

    txBuff[0] = CMD_TRIG_CONV;
    txBuff[1] = TRIG_CONV_ARG1;
    txBuff[2] = TRIG_CONV_ARG2;

    HAL_Delay(10);
    HAL_I2C_Master_Transmit(&hi2c1, AHT21B_ADDRESS, txBuff, SIZE_TRIG_CONV_CMD, HAL_MAX_DELAY);

    WAIT_FOR_CONVERSION:
    txBuff[0] = CMD_GET_STATUS;
    HAL_Delay(80);
    HAL_I2C_Master_Transmit(&hi2c1, AHT21B_ADDRESS, txBuff, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, AHT21B_ADDRESS, rxBuff, 1, HAL_MAX_DELAY);
    if ((rxBuff[0] & (1 << (ONE_BYTE-1))) != 0x00) {
        goto WAIT_FOR_CONVERSION;
    }

    memset(rxBuff, 0, SIZE_SESNOR_DATA);
    HAL_I2C_Master_Receive(&hi2c1, AHT21B_ADDRESS, rxBuff, SIZE_SESNOR_DATA, HAL_MAX_DELAY);
    
    rawHumidity  = (uint32_t) rxBuff[1];
    rawHumidity  = (rawHumidity << ONE_BYTE);
    rawHumidity |= (uint32_t) rxBuff[2];
    rawHumidity  = (rawHumidity << ONE_NIBBLE);
    rawHumidity |= (uint32_t) ((UPPER_NIBBLE & rxBuff[3]) >> ONE_NIBBLE);
    *pRawHumidity = rawHumidity;

    rawTemparature  = (uint32_t) (LOWER_NIBBLE & rxBuff[3]);
    rawTemparature  = (rawTemparature << ONE_BYTE);
    rawTemparature |= (uint32_t) rxBuff[4];
    rawTemparature  = (rawTemparature << ONE_BYTE);
    rawTemparature |= (uint32_t) rxBuff[5];
    *pRawTemparature = rawTemparature;
}


/**
 * @brief convert raw temparature value to celcius value
*/
static float AHT21B_convertRawTemparature(uint32_t* pRawTemparature)
{
    float celciusTemp;
    celciusTemp = (float)(*pRawTemparature) / (float) pow(2, 20);
    celciusTemp = (celciusTemp * 200)  - 50;
    return celciusTemp;
}

/**
 * @brief convert raw humidity value to percentage value
*/
static float AHT21B_convertRawHumidity(uint32_t* pRawHumidity)
{
    float humidity;
    humidity = (float)(*pRawHumidity) / (float) pow(2, 20);
    humidity = (humidity * 100);
    return humidity;
}