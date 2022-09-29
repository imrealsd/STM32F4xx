#include <stm32f407xx.h>
#include <stdint.h>
#include "I2C1.h"

void setup_I2C1_SM (void)
{
    /* scl & sda gpio config PB8 -> SCL, PB9->SDA 
     * open drain , high speed and  pulled up 
    */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;
    GPIOB->AFR[1] |= (0x4 << GPIO_AFRH_AFSEL8_Pos) | (0x4 << GPIO_AFRH_AFSEL9_Pos);
    GPIOB->OTYPER |= GPIO_OTYPER_OT9_Msk | GPIO_OTYPER_OT8_Msk;  
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_Msk;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0; 

    /* I2C1 CLK EN */
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    /* 16 MHZ peripheral freq [16 Mhz apb1 freq */
    I2C1->CR2 |= (16 << I2C_CR2_FREQ_Pos);  

    /* FPCLK = 16MHZ , TPCLK = 1/16000000 sec = 62.5 ns
     * For 100Khz T-high + T-low = 1/100000 = 100000 ns 
     * T-high = T-low = 5000 ns , acc to formulae - 
     * T-high = T-low = CCR * TPCLK , CCR = 5000 ns / 62.5 ns = 80
    */
    I2C1->CCR |= (80 << I2C_CCR_CCR_Pos);

    /* T-rise config [max feedback loop time] 
     * T-rise = (1000/62.5)+1 = 17
    */
    I2C1->TRISE |= (17 << I2C_TRISE_TRISE_Pos);
    /* I2C peripheral enable */
    I2C1->CR1 |= I2C_CR1_PE;
}

void setup_I2C1_FM (void)
{
    /* scl & sda gpio config PB8 -> SCL, PB9->SDA 
     * open drain , high speed and  pulled up 
    */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;
    GPIOB->AFR[1] |= (0x4 << GPIO_AFRH_AFSEL8_Pos) | (0x4 << GPIO_AFRH_AFSEL9_Pos);
    GPIOB->OTYPER |= GPIO_OTYPER_OT9_Msk | GPIO_OTYPER_OT8_Msk;  
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_Msk;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0; 

    /* I2C1 CLK EN */
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    /* 16 MHZ peripheral freq [16 Mhz apb1 freq */
    I2C1->CR2 |= (16 << I2C_CR2_FREQ_Pos);  
    /* I2c Fast Mode selection */
    I2C1->CCR |= (1 << 15);

    /* FPCLK = 16MHZ , TPCLK = 1/16000000 sec = 62.5 ns
     * For 400Khz T-high + T-low = 1/400,000 = 2,500 ns 
     * T-high = T-low = 1250 ns , acc to formulae - 
     * T-high = T-low = CCR * TPCLK , CCR = 1250 ns / 62.5 ns = 20
    */
    I2C1->CCR |= (20 << I2C_CCR_CCR_Pos);

    /* T-rise config [max feedback loop time] 
     * T-rise = (300/62.5)+1 = 5.8
    */
    I2C1->TRISE |= (5 << I2C_TRISE_TRISE_Pos);
    /* I2C peripheral enable */
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_start (void)
{   
    // Enable the ACK
    I2C1->CR1 |= (I2C_CR1_ACK);  
    /*start i2c */
    I2C1->CR1 |= I2C_CR1_START;
    /* wait for start bit to send */
    while (!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C1_write_addr (int8_t addr, char* operation)
{   
    /* operation READ / WRITE */
    if (operation[0] == 'W')
        addr = addr << 1;
    else if (operation[0] == 'R')
        addr = (addr << 1) | 0b1;
    /* sending address + R/W bit */
    I2C1->DR = addr;

    /* wait for address to be  matched and get ack*/
    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    /* wait for the total address phase to end */
    while (!(I2C1->SR2 & I2C_SR2_TRA));
}

void I2C1_write_data (int8_t data)
{   
    /* wait for data reg to empty */
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    /* clear BTF flag */
    int8_t dummy_read =  I2C1->DR;
    /* write data */
    I2C1->DR = data;
    /* wait for the last bit to transmit */
    while (!(I2C1->SR2 & I2C_SR1_BTF));
}

void I2C1_stop (void)
{   
    /* send stop condition */
    I2C1->CR1 |= I2C_CR1_STOP;
}


// int8_t I2C1_read_data_ACK (void)
// {   
//     /* wait for byte to recieve */
//     while (!(I2C1->SR1 & I2C_SR1_RXNE));
//     /* read data */
//     int8_t data = I2C1->DR;
//     return data;
// }

// int8_t I2C1_read_data_NACK (void)
// {   
//     /* disable nack */
//     I2C1->CR1 &= ~(I2C_CR1_ACK);
//     /* wait for byte to recieve */
//     while (!(I2C1->SR1 & I2C_SR1_RXNE));
//     /* read data */
//     int8_t data = I2C1->DR;
//     return data;
// }

void I2C1_read(int32_t Address, int8_t buffer[], int size)
{
    /**** STEPS FOLLOWED  ************
1. If only 1 BYTE needs to be Read
	a) Write the slave Address, and wait for the ADDR bit (bit 1 in SR1) to be set
	b) the Acknowledge disable is made during EV6 (before ADDR flag is cleared) and the STOP condition generation is made after EV6
	c) Wait for the RXNE (Receive Buffer not Empty) bit to set
	d) Read the data from the DR

2. If Multiple BYTES needs to be read
  a) Write the slave Address, and wait for the ADDR bit (bit 1 in SR1) to be set
	b) Clear the ADDR bit by reading the SR1 and SR2 Registers
	c) Wait for the RXNE (Receive buffer not empty) bit to set
	d) Read the data from the DR 
	e) Generate the Acknowlegment by settint the ACK (bit 10 in SR1)
	f) To generate the nonacknowledge pulse after the last received data byte, the ACK bit must be cleared just after reading the 
		 second last data byte (after second last RxNE event)
	g) In order to generate the Stop/Restart condition, software must set the STOP/START bit 
	   after reading the second last data byte (after the second last RxNE event)
*/		
	
	int remaining = size;
	
/**** STEP 1 ****/	
	if (size == 1)
	{
		/**** STEP 1-a ****/	
		I2C1->DR = Address;                            //  send the address
		while (!(I2C1->SR1 & (I2C_SR1_ADDR)));         // wait for ADDR bit to set
		
		/**** STEP 1-b ****/	
		I2C1->CR1 &= ~(I2C_CR1_ACK);                   // clear the ACK bit 
		uint8_t temp = I2C1->SR1 | I2C1->SR2;          // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
		I2C1->CR1 |= (I2C_CR1_STOP);                   // Stop I2C

		/**** STEP 1-c ****/	
		while (!(I2C1->SR1 & (I2C_SR1_RXNE)));             // wait for RxNE to set
		
		/**** STEP 1-d ****/	
		buffer[size-remaining] = I2C1->DR;                   // Read the data from the DATA REGISTER
		
	}

/**** STEP 2 ****/		
	else 
	{
		/**** STEP 2-a ****/
		I2C1->DR = Address;  //  send the address
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		
		/**** STEP 2-b ****/
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
		
		while (remaining > 2)
		{
			/**** STEP 2-c ****/
			while (!(I2C1->SR1 & (I2C_SR1_RXNE)));  // wait for RxNE to set
			
			/**** STEP 2-d ****/
			buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer			
			
			/**** STEP 2-e ****/
			I2C1->CR1 |= I2C_CR1_ACK;           // Set the ACK bit to Acknowledge the data received
			
			remaining--;
		}
		
		// Read the SECOND LAST BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		
		/**** STEP 2-f ****/
		I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
		
		/**** STEP 2-g ****/
		I2C1->CR1 |= (1<<9);  // Stop I2C
		
		remaining--;
		
		// Read the Last BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer
	}	
	
}
