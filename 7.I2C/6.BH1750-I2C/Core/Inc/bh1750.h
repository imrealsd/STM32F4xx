#ifndef BH1750_H
#define BH1750_H

/*------------Driver Macros-------------------------------------------*/
   
#define BH1750_ADDR_L                           (0x23 << 1)     // ADDR pin connected to GND 
#define BH1750_ADDR_H                           (0x5C << 1)     // ADDR oin connected to VCC
#define CMD_PWR_DOWN                            (0x00)
#define CMD_PWR_ON                              (0x01)
#define CMD_RESET                               (0x07)

#define CMD_ONE_TIME_H_RESOLUTION_MODE_1        (0x20)           // Resolution 1.0 lux       
#define CMD_ONE_TIME_H_RESOLUTION_MODE_2        (0x21)           // Resolution 0.5 lux

#define CMD_CONTINUOS_H_RESOLUTION_MODE_1       (0x10)           // Resolution 1.0 lux
#define CMD_CONTINUOS_H_RESOLUTION_MODE_2       (0x11)           // Resolution 0.5 lux

#define CMD_ONE_TIME_L_RESOLUTION_MODE          (0x13)           // Resolution 4.0 lux
#define CMD_CONTINUOS_L_RESOLUTION_MODE         (0x23)           // Resolution 4.0 lux

/*---------------Driver APIs---------------------------------------------*/


/**
 * @brief initialize bh1750 sensor
 * @param uint8_t measurement mode
 * @retval none
 */
void bh1750_init(uint8_t measurement_mode);

/**
 * @brief read illuminance from sensor & return (in Lux uinit)
 * @param none
 * @retval float 
 */
float bh1750_get_illuminance(void);

/**
 * @brief write command to sensor
 * @param uint8_t command
 * @retval none
 */
void bh1750_write_command(uint8_t cmd);


#endif