/*
 * spi_send_data_test.c
 *
 *  Created on: 31-Jul-2026
 *      Author: Valkrie
 */


#include <stdint.h>
#include "STM32F401CCUx.h"
#include "stm32f401ccux_gpio_driver.h"
#include "spi.h"
#include <string.h>

// Send "HELLO WORLD"  in Master Mode
	// SPI2 MASTER MODE
	// SCLK Max
	// DFF = 0 and DFF = 1
	//pg num 45
	// PB12 as NSS
	// PB13 as SCLK
	// PB15 as MOSI
	// PB14 as MISO
	// AF05

void SPI2_GPIO_INIT(void)
{
	// GPIO_CLK_ENABLE

	gpio_handle_t SPI_PINS;
	SPI_PINS.pGPIOx = GPIOB;
	SPI_PINS.gpio_config.gpio_PinMode = GPIO_MODE_ALT_FUN;
	SPI_PINS.gpio_config.gpio_Alt_Func_Mode = GPIO_AF5;
	SPI_PINS.gpio_config.gpio_PinOtype = GPIO_OTYPE_PP;
	SPI_PINS.gpio_config.gpio_PinPupdControl = GPIO_NO_PUPD;
	SPI_PINS.gpio_config.gpio_PinSpeed = SPEED_VERY_HIGH;
	// CLK CONFIG
	SPI_PINS.gpio_config.gpio_PinNumber = GPIO_PIN_NO_13;
	GPIO_INIT(&SPI_PINS);

	// MISO CONFIG
		//SPI_PINS.gpio_config.gpio_PinNumber = GPIO_PIN_NO_14;
		//GPIO_INIT(&SPI_PINS);

	// MOSI CONFIG
	    SPI_PINS.gpio_config.gpio_PinNumber = GPIO_PIN_NO_15;
		GPIO_INIT(&SPI_PINS);

	// NSS CONFIG
		SPI_PINS.gpio_config.gpio_PinNumber = GPIO_PIN_NO_12;
		GPIO_INIT(&SPI_PINS);
}

void SPI2_INIT(void)
{

	SPI_HANDLE_T SPI2_HANDLE;
	SPI2_HANDLE.pSPIx = SPI2;
	SPI2_HANDLE.SPI_CONFIG.SPI_BusConfig = SPI_DEVICE_FD;
	SPI2_HANDLE.SPI_CONFIG.SPI_DeviceMode = SPI_DEVICE_MASTER;
	SPI2_HANDLE.SPI_CONFIG.SPI_SCLKspeed = SPI_SCLK_DIV2;
	SPI2_HANDLE.SPI_CONFIG.SPI_DFF = SPI_DFF_8BIS;
	SPI2_HANDLE.SPI_CONFIG.SPI_CPOL = SPI_CPOL_LOW;
	SPI2_HANDLE.SPI_CONFIG.SPI_CPHA = SPI_CPHA_LOW;
	SPI2_HANDLE.SPI_CONFIG.SPI_SSM= SPI_SSM_DI; // soft slave management enable

	SPI_INIT(&SPI2_HANDLE);
}


void button(){
	gpio_handle_t SWITCH;
	SWITCH.pGPIOx = GPIOA;
		SWITCH.gpio_config.gpio_PinNumber = GPIO_PIN_NO_0;
		SWITCH.gpio_config.gpio_PinMode  = GPIO_IT_FT;
		SWITCH.gpio_config.gpio_PinPupdControl = GPIO_PU;
		GPIO_INIT(&SWITCH);

}
int main(void)
{
	char buff1[] = "Hello From STM32 We R Communicating via SPI\n";
	//char buff2[] = "Please Press Button To Initiate Transfer!!\n";
	SPI2_GPIO_INIT();

    SPI2_INIT();

    SPI_SSI_Config(SPI2, DISABLE);

    // SSM 0 SSOE 1 this will set NSS to 0 so that Slave Nss is Grounded
    SPI_SSOE_Config(SPI2, ENABLE);
    button();


    // enable the SPI2 periphral




    for(volatile uint32_t i = 0; i < 1000000; i++);





    while (1)
    {
       if(!GPIO_Read_From_GPIOPIN(GPIOA, GPIO_PIN_NO_0))

    	{
            // debounce
            for(volatile int i=0;i<30000;i++);

           if(!GPIO_Read_From_GPIOPIN(GPIOA, GPIO_PIN_NO_0))

            {
            	for(volatile uint32_t i = 0; i < 3000000; i++);
                SPI_Peripheral_control(SPI2, ENABLE);

                SPI_SendData(SPI2,(uint8_t *)buff1,strlen(buff1));

                while(SPI_Get_Flag_status(SPI2, SPI_BUSY_FLAG));

                SPI_Peripheral_control(SPI2, DISABLE);

                // Wait until button released
                while(!GPIO_Read_From_GPIOPIN(GPIOA, GPIO_PIN_NO_0));
            }
        }
    }
	return 0;
}
