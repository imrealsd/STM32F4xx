# STM32F407 Peripheral Programming

Programming STM32f407VE through registers & HAL using STM32F407xx datasheet & reference manual

# Contents :

```bash

├── 10. EXT INTERRUPT
│   └── External-Interrupt
│       ├── include
│       │   └── README
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── exti.c
│       │   ├── exti.h
│       │   ├── main.c
│       │   ├── sys_clock.c
│       │   ├── sys_clock.h
│       │   ├── UART1.c
│       │   └── UART1.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── 11.LOW-POWER-MODES
│   ├── 1.Sleep-Mode
│   │   ├── include
│   │   │   ├── gpio.h
│   │   │   ├── main.h
│   │   │   ├── README
│   │   │   ├── stm32f4xx_hal_conf.h
│   │   │   ├── stm32f4xx_it.h
│   │   │   └── usart.h
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── gpio.c
│   │   │   ├── main.c
│   │   │   ├── stm32f4xx_hal_msp.c
│   │   │   ├── stm32f4xx_it.c
│   │   │   ├── system_stm32f4xx.c
│   │   │   └── usart.c
│   │   └── test
│   │       └── README
│   ├── 2.Stop-Mode
│   │   ├── include
│   │   │   ├── gpio.h
│   │   │   ├── main.h
│   │   │   ├── README
│   │   │   ├── stm32f4xx_hal_conf.h
│   │   │   ├── stm32f4xx_it.h
│   │   │   └── usart.h
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── gpio.c
│   │   │   ├── main.c
│   │   │   ├── stm32f4xx_hal_msp.c
│   │   │   ├── stm32f4xx_it.c
│   │   │   ├── system_stm32f4xx.c
│   │   │   └── usart.c
│   │   └── test
│   │       └── README
│   └── 3.Standby-Mode
│       ├── include
│       │   ├── gpio.h
│       │   ├── main.h
│       │   ├── README
│       │   ├── stm32f4xx_hal_conf.h
│       │   ├── stm32f4xx_it.h
│       │   └── usart.h
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── gpio.c
│       │   ├── main.c
│       │   ├── stm32f4xx_hal_msp.c
│       │   ├── stm32f4xx_it.c
│       │   ├── system_stm32f4xx.c
│       │   └── usart.c
│       └── test
│           └── README
├── 12. RNG
│   └── RNG_TEST
│       ├── include
│       │   └── README
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── main.c
│       │   ├── RNG.c
│       │   ├── RNG.h
│       │   ├── sys_clock.c
│       │   ├── sys_clock.h
│       │   ├── UART1.c
│       │   └── UART1.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── 13.WDG
│   ├── IWDG-Test
│   │   ├── include
│   │   │   ├── gpio.h
│   │   │   ├── iwdg.h
│   │   │   ├── main.h
│   │   │   ├── stm32f4xx_hal_conf.h
│   │   │   └── stm32f4xx_it.h
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── gpio.c
│   │   │   ├── iwdg.c
│   │   │   ├── main.c
│   │   │   ├── stm32f4xx_hal_msp.c
│   │   │   ├── stm32f4xx_it.c
│   │   │   └── system_stm32f4xx.c
│   │   └── test
│   │       └── README
│   └── WWDG-Test
│       ├── include
│       │   ├── gpio.h
│       │   ├── main.h
│       │   ├── README
│       │   ├── stm32f4xx_hal_conf.h
│       │   ├── stm32f4xx_it.h
│       │   └── wwdg.h
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── gpio.c
│       │   ├── main.c
│       │   ├── stm32f4xx_hal_msp.c
│       │   ├── stm32f4xx_it.c
│       │   ├── system_stm32f4xx.c
│       │   └── wwdg.c
│       └── test
│           └── README
├── 14. CRC
│   └── CRC_TEST
│       ├── include
│       │   └── README
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── crc.c
│       │   ├── crc.h
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── main.c
│       │   ├── sys_clock.c
│       │   ├── sys_clock.h
│       │   ├── UART1.c
│       │   └── UART1.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── 15.RTC_ON_CHIP
│   └── RTC_ON_CHIP
│       ├── include
│       │   └── README
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── main.c
│       │   ├── RTC.c
│       │   ├── RTC.h
│       │   ├── sys_clock.c
│       │   ├── sys_clock.h
│       │   ├── UART1.c
│       │   └── UART1.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── 16.CAN
│   ├── 1.CAN-LoopBack-Mode
│   │   ├── include
│   │   │   ├── can.h
│   │   │   ├── gpio.h
│   │   │   ├── main.h
│   │   │   ├── README
│   │   │   ├── stm32f4xx_hal_conf.h
│   │   │   └── stm32f4xx_it.h
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── can.c
│   │   │   ├── gpio.c
│   │   │   ├── main.c
│   │   │   ├── stm32f4xx_hal_msp.c
│   │   │   ├── stm32f4xx_it.c
│   │   │   └── system_stm32f4xx.c
│   │   └── test
│   │       └── README
│   └── 2.CAN_STM32_Esp32
│       ├── CAN_ESP32_RX
│       │   ├── include
│       │   │   └── README
│       │   ├── lib
│       │   │   └── README
│       │   ├── platformio.ini
│       │   ├── src
│       │   │   └── main.cpp
│       │   └── test
│       │       └── README
│       ├── CAN_STM32_TX
│       │   ├── include
│       │   │   ├── can.h
│       │   │   ├── gpio.h
│       │   │   ├── main.h
│       │   │   ├── README
│       │   │   ├── stm32f4xx_hal_conf.h
│       │   │   └── stm32f4xx_it.h
│       │   ├── lib
│       │   │   └── README
│       │   ├── platformio.ini
│       │   ├── src
│       │   │   ├── can.c
│       │   │   ├── gpio.c
│       │   │   ├── main.c
│       │   │   ├── stm32f4xx_hal_msp.c
│       │   │   ├── stm32f4xx_it.c
│       │   │   └── system_stm32f4xx.c
│       │   └── test
│       │       └── README
│       └── Screenshot from 2023-01-27 19-31-10.png
├── 17.USB
├── 18.DMA
├── 19.SDIO
│   └── SDIO-Fatfs
│       ├── include
│       │   ├── bsp_driver_sd.h
│       │   ├── fatfs.h
│       │   ├── ffconf.h
│       │   ├── File_Handling.h
│       │   ├── gpio.h
│       │   ├── main.h
│       │   ├── README
│       │   ├── sd_diskio.h
│       │   ├── sdio.h
│       │   ├── stm32f4xx_hal_conf.h
│       │   ├── stm32f4xx_it.h
│       │   └── usart.h
│       ├── lib
│       │   ├── FatFs
│       │   │   └── src
│       │   │       ├── diskio.c
│       │   │       ├── diskio.h
│       │   │       ├── ff.c
│       │   │       ├── ff_gen_drv.c
│       │   │       ├── ff_gen_drv.h
│       │   │       ├── ff.h
│       │   │       ├── integer.h
│       │   │       └── option
│       │   │           └── syscall.c
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── bsp_driver_sd.c
│       │   ├── fatfs.c
│       │   ├── File_Handling.c
│       │   ├── gpio.c
│       │   ├── main.c
│       │   ├── sd_diskio.c
│       │   ├── sdio.c
│       │   ├── stm32f4xx_hal_msp.c
│       │   ├── stm32f4xx_it.c
│       │   ├── system_stm32f4xx.c
│       │   └── usart.c
│       └── test
│           └── README
├── 1. GPIO
│   ├── 1. Led Blink
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── main.c
│   │   │   ├── onboard_led.c
│   │   │   ├── onboard_led.h
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 2. Button
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── main.c
│   │   │   ├── onboard_button_led.c
│   │   │   ├── onboard_button_led.h
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 3. IR-Sensor
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── ir_sensor.c
│   │   │   ├── ir_sensor.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 4. 7-Segment-Display
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── main.c
│   │   │   ├── seven_segment.c
│   │   │   ├── seven_segment.h
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 5. 4x4 Matrix Keypad
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── keypad.c
│   │   │   ├── keypad.h
│   │   │   ├── main.c
│   │   │   ├── onboard_led.c
│   │   │   ├── onboard_led.h
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   └── 6. 8x8 Led Matrix
│       ├── include
│       │   └── README
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── led_matrix.c
│       │   ├── led_matrix.h
│       │   ├── main.c
│       │   ├── sys_clock.c
│       │   └── sys_clock.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── 2. 16 x 2 LCD
│   ├── include
│   │   └── README
│   ├── lib
│   │   └── README
│   ├── platformio.ini
│   ├── src
│   │   ├── lcd.c
│   │   ├── lcd.h
│   │   ├── main.c
│   │   ├── sys_clock.c
│   │   └── sys_clock.h
│   ├── test
│   │   └── README
│   └── upload.sh
├── 2. TIMER & COUNTER
│   ├── 1. Accurate Delay
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── onboard_led.c
│   │   │   ├── onboard_led.h
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 2. Input Pulse Counter ( EXT CLK MODE)
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── counter.c
│   │   │   ├── counter.h
│   │   │   ├── main.c
│   │   │   ├── onboard_led.c
│   │   │   ├── onboard_led.h
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 3. Pulse Freq Measurement (INPUT CAPTURE MODE)
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── input_capture.c
│   │   │   ├── input_capture.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── uart.c
│   │   │   └── uart.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 4. Pulse Freq & Duty Cycle Measurement (PWM INPUT)
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── pwm_input.c
│   │   │   ├── pwm_input.h
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── uart.c
│   │   │   └── uart.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   └── 5. Ultrasonic Distance Measurement
│       ├── include
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── main.c
│       │   ├── pwm_input.c
│       │   ├── pwm_input.h
│       │   ├── sys_clock.c
│       │   ├── sys_clock.h
│       │   ├── uart.c
│       │   ├── uart.h
│       │   ├── ultrasonic.c
│       │   └── ultrasonic.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── 3. PWM & MOTORS
│   ├── 1. DC Motor Direction Control [ L293D]
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── main.c
│   │   │   ├── Motor.c
│   │   │   ├── Motor.h
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 2. Controlling Servo Motor (PWM OUTPUT)
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── pwm_output.c
│   │   │   ├── pwm_output.h
│   │   │   ├── servo.c
│   │   │   ├── servo.h
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   └── 3. Stepper Motor
│       ├── include
│       │   └── README
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── main.c
│       │   ├── stepper.c
│       │   ├── stepper.h
│       │   ├── sys_clock.c
│       │   └── sys_clock.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── 4. ADC & DAC
│   ├── 1.Potentiometer  [ ADC]
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── ADC1.c
│   │   │   ├── ADC1.h
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── uart.c
│   │   │   └── uart.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 2. Temparature Sensor
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── ADC1.c
│   │   │   ├── ADC1.h
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── uart.c
│   │   │   └── uart.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 3. Sound Sensor
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── ADC1.c
│   │   │   ├── ADC1.h
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── uart.c
│   │   │   └── uart.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 4. Joysick Module
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── Pynput
│   │   │   └── emulate_keyboard.py
│   │   ├── src
│   │   │   ├── ADC1.c
│   │   │   ├── ADC1.h
│   │   │   ├── ADC2.c
│   │   │   ├── ADC2.h
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── uart.c
│   │   │   └── uart.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   └── 5. Led Fading - DAC
│       ├── include
│       │   └── README
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── DAC1.c
│       │   ├── DAC1.h
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── main.c
│       │   ├── sys_clock.c
│       │   └── sys_clock.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── 5. UART
│   ├── 1. Serial Communication With PC
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── onboard_led.c
│   │   │   ├── onboard_led.h
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── UART1.c
│   │   │   └── UART1.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 2. Reading Multiple Bytes Through ISR
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── UART1.c
│   │   │   └── UART1.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 3. Talking To Esp32
│   │   ├── Esp32
│   │   │   └── Esp32.ino
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── main.c
│   │   │   ├── onboard_button_led.c
│   │   │   ├── onboard_button_led.h
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── UART1.c
│   │   │   └── UART1.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   └── 4.HC05-BT-Quick-Connect
│       ├── Stm32f407-Master
│       │   ├── include
│       │   │   ├── gpio.h
│       │   │   ├── main.h
│       │   │   ├── README
│       │   │   ├── stm32f4xx_hal_conf.h
│       │   │   ├── stm32f4xx_it.h
│       │   │   └── usart.h
│       │   ├── lib
│       │   │   ├── HC05_Driver
│       │   │   │   ├── hc05.c
│       │   │   │   └── hc05.h
│       │   │   └── README
│       │   ├── platformio.ini
│       │   ├── src
│       │   │   ├── gpio.c
│       │   │   ├── main.c
│       │   │   ├── stm32f4xx_hal_msp.c
│       │   │   ├── stm32f4xx_it.c
│       │   │   ├── system_stm32f4xx.c
│       │   │   └── usart.c
│       │   └── test
│       │       └── README
│       └── Stm32f407-Slave
│           ├── include
│           │   ├── gpio.h
│           │   ├── main.h
│           │   ├── README
│           │   ├── stm32f4xx_hal_conf.h
│           │   ├── stm32f4xx_it.h
│           │   └── usart.h
│           ├── lib
│           │   ├── HC05_Driver
│           │   │   ├── hc05.c
│           │   │   └── hc05.h
│           │   └── README
│           ├── platformio.ini
│           ├── src
│           │   ├── gpio.c
│           │   ├── main.c
│           │   ├── stm32f4xx_hal_msp.c
│           │   ├── stm32f4xx_it.c
│           │   ├── system_stm32f4xx.c
│           │   └── usart.c
│           └── test
│               └── README
├── 6. I2C
│   ├── 1. PCF874T-I2C
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── I2C1.c
│   │   │   ├── I2C1.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 2. LCD-I2C
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── I2C1.c
│   │   │   ├── I2C1.h
│   │   │   ├── i2c_lcd.c
│   │   │   ├── i2c_lcd.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   └── sys_clock.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   ├── 3. BMP280-I2C
│   │   ├── include
│   │   │   └── README
│   │   ├── lib
│   │   │   └── README
│   │   ├── platformio.ini
│   │   ├── src
│   │   │   ├── bmp280_I2C.c
│   │   │   ├── bpm280_I2C.h
│   │   │   ├── delay.c
│   │   │   ├── delay.h
│   │   │   ├── I2C1.c
│   │   │   ├── I2C1.h
│   │   │   ├── main.c
│   │   │   ├── sys_clock.c
│   │   │   ├── sys_clock.h
│   │   │   ├── tempCodeRunnerFile.c
│   │   │   ├── UART1.c
│   │   │   └── UART1.h
│   │   ├── test
│   │   │   └── README
│   │   └── upload.sh
│   └── 4.OLED-SSD1306
│       ├── include
│       │   ├── bitmap.h
│       │   ├── fonts.h
│       │   ├── gpio.h
│       │   ├── i2c.h
│       │   ├── main.h
│       │   ├── ssd1306.h
│       │   ├── stm32f4xx_hal_conf.h
│       │   ├── stm32f4xx_it.h
│       │   └── usart.h
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── fonts.c
│       │   ├── gpio.c
│       │   ├── i2c.c
│       │   ├── main.c
│       │   ├── ssd1306.c
│       │   ├── stm32f4xx_hal_msp.c
│       │   ├── stm32f4xx_it.c
│       │   ├── system_stm32f4xx.c
│       │   └── usart.c
│       └── test
│           └── README
├── 7. SPI
│   └── 1.MAX-7221-SPI
│       ├── include
│       │   └── README
│       ├── lib
│       │   └── README
│       ├── platformio.ini
│       ├── src
│       │   ├── delay.c
│       │   ├── delay.h
│       │   ├── main.c
│       │   ├── SPI1.c
│       │   ├── SPI1.h
│       │   ├── sys_clock.c
│       │   └── sys_clock.h
│       ├── test
│       │   └── README
│       └── upload.sh
├── ALL DRIVERS
│   ├── 16x2 LCD
│   │   ├── lcd.c
│   │   └── lcd.h
│   ├── 4x4 Kepad
│   │   ├── keypad.c
│   │   └── keypad.h
│   ├── 7 Segment
│   │   ├── seven_segment.c
│   │   └── seven_segment.h
│   ├── 8x8 Led Matrix
│   │   ├── led_matrix.c
│   │   └── led_matrix.h
│   ├── ADC1
│   │   ├── ADC1.c
│   │   └── ADC1.h
│   ├── ADC2
│   │   ├── ADC2.c
│   │   └── ADC2.h
│   ├── Delay [ TIM-6]
│   │   ├── delay.c
│   │   └── delay.h
│   ├── EXTI
│   │   ├── exti.c
│   │   └── exti.h
│   ├── Ext Pulse Counter
│   │   ├── counter.c
│   │   └── counter.h
│   ├── I2C1
│   │   ├── I2C1.c
│   │   └── I2C1.h
│   ├── Input Capture Mode
│   │   ├── input_capture.c
│   │   └── input_capture.h
│   ├── Internal-RTC
│   │   ├── RTC.c
│   │   └── RTC.h
│   ├── IR Sensor
│   │   ├── ir_sensor.c
│   │   └── ir_sensor.h
│   ├── LCD-I2C
│   │   ├── I2C1.c
│   │   ├── I2C1.h
│   │   ├── i2c_lcd.c
│   │   └── i2c_lcd.h
│   ├── Millis [Timer7]
│   │   ├── millis.c
│   │   └── millis.h
│   ├── Onboard Button + LED
│   │   ├── onboard_button_led.c
│   │   └── onboard_button_led.h
│   ├── On Board Led
│   │   ├── onboard_led.c
│   │   └── onboard_led.h
│   ├── PWM input
│   │   ├── pwm_input.c
│   │   └── pwm_input.h
│   ├── Pwm output
│   │   ├── pwm_output.c
│   │   └── pwm_output.h
│   ├── RNG
│   │   ├── RNG.c
│   │   └── RNG.h
│   ├── SPI1
│   │   ├── SPI1.c
│   │   └── SPI1.h
│   ├── Stepper Motor
│   │   ├── stepper.c
│   │   └── stepper.h
│   ├── System Clock
│   │   ├── sys_clock.c
│   │   └── sys_clock.h
│   ├── UART1
│   │   ├── UART1.c
│   │   └── UART1.h
│   └── Ultrasonic Sensor
│       ├── ultrasonic.c
│       └── ultrasonic.h
└── README.md


```
