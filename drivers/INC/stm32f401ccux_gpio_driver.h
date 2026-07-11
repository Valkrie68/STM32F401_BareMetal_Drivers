/*
 * stm32f401ccux_gpio_driver.h
 *
 *  Created on: 07-Jul-2026
 *      Author: veeresh
 */

#ifndef INC_STM32F401CCUX_GPIO_DRIVER_H_
#define INC_STM32F401CCUX_GPIO_DRIVER_H_




/* INC_STM32F401CCUX_GPIO_DRIVER_H_ */
#include "STM32F401CCUx.h"



typedef struct
{
uint8_t gpio_PinNumber;    /* POssible values From @GPIO_PIN_NUMBERS */
uint8_t gpio_PinMode;      /* Possible Values From @GPIO_PIN_MODES */
uint8_t gpio_PinSpeed;      /*Possible Values From @GPIO TYPE POSSIBLE SPEED TYPE MACROS*/
uint8_t gpio_PinPupdControl;/*possible Values from @GPIO TYPE PULL UP OR PULL DOWN MACROS */
uint8_t gpio_PinOtype;     /* Possible Values From @OUTPUT TYPE MACROS*/
uint8_t gpio_Alt_Func_Mode; /*Possible values from @GPIO Alternate Function Macros */
}Gpio_Config_t;

/*     handler Structure    */
typedef struct{
	gpio_reg_def *pGPIOx;  // This holds the base address of Gpio port which it belongs to
	Gpio_Config_t  gpio_config; /* This holds the Gpio Pin Configuration settings */
}gpio_handle_t;


/* peripheral Register definition structure for EXTI*/
typedef struct{

	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}exti_regdef_t;


/* PERIPHERAL REGISTER DEFINITION FOR SYSCFG */
typedef struct{
	volatile uint32_t SYSCFG_MEMRMP;   // 0x00
	volatile uint32_t SYSCFG_PMC;      // 0x04
	volatile uint32_t SYSCFG_EXTICR[4];  // 0x08  0x0C   0x10   0x14
	volatile uint32_t resrve1;         // 0x18
	volatile uint32_t resrve2;         // 0x1C
	volatile uint32_t SYSCFG_CMPCR;    // 0x20

}syscfg_regdef_t;
///////////// imp API's prototype ///////////


/*  CLOCK API's  */
void GPIO_PeriClockControl(gpio_reg_def *GPIOx , uint8_t  EN_DI);


/* GPIO API's */
void GPIO_INIT(gpio_handle_t  *pGPIOx_HANDLE);
void GPIO_DeINIT(gpio_reg_def *pGPIOx);

/* API's for REading from Port or Pin */
uint8_t GPIO_Read_From_GPIOPORT(gpio_reg_def *pGPIOx);
uint8_t GPIO_Read_From_GPIOPIN(gpio_reg_def *pGPIOx  , uint8_t GPIO_PIN_NUM);

/* API's for Writing from Port or Pin */
void GPIO_Write_to_GPIOPORT(gpio_reg_def *pGPIOx , uint16_t value);
void GPIO_Write_to_GPIOPIN(gpio_reg_def *pGPIOx  , uint8_t GPIO_PIN_NUM ,uint8_t value);
void GPIO_Toggle_pin(gpio_reg_def *pGPIOx  , uint8_t GPIO_PIN_NUM);

/// IRQ CONFIG ///
void GPIO_IRQ_IT_CONFIG(uint8_t IRQ_NUMBER ,uint8_t EN_DI);
void GPIO_PRIORITY_CONFIG(uint8_t IRQ_number,  uint32_t IRQ_priority);
void GPIO_IRQ_Handling(uint8_t PIN_NUMBER);



///  @GPIO_PIN_MODES
/*  Possible Modes of GPIO */
#define GPIO_MODE_IN         0
#define GPIO_MODE_OUT        1
#define GPIO_MODE_ALT_FUN    2
#define GPIO_MODE_ANALOG     3

/* Possible Interrupt Modes */ /// Note : If mode is greater than 3 that pin is a Interrupt mode
#define GPIO_IT_FT           4
#define GPIO_IT_RT           5
#define GPIO_IT_RFT          6

//// @GPIO_PIN_NUMBERS  /////////
/* GPIO_PIN_NO */
#define GPIO_PIN_NO_0        0
#define GPIO_PIN_NO_1        1
#define GPIO_PIN_NO_2        2
#define GPIO_PIN_NO_3        3
#define GPIO_PIN_NO_4        4
#define GPIO_PIN_NO_5        5
#define GPIO_PIN_NO_6        6
#define GPIO_PIN_NO_7        7
#define GPIO_PIN_NO_8        8
#define GPIO_PIN_NO_9        9
#define GPIO_PIN_NO_10       10
#define GPIO_PIN_NO_11       11
#define GPIO_PIN_NO_12       12
#define GPIO_PIN_NO_13       13
#define GPIO_PIN_NO_14       14
#define GPIO_PIN_NO_15       15



/* GPIO TYPE POSSIBLE @OUTPUT TYPE MACROS  */
#define GPIO_OTYPE_PP        0
#define GPIO_OTYPE_OD        1

/* @GPIO TYPE POSSIBLE SPEED TYPE MACROS  */
#define SPEED_LOW           0
#define SPEED_MED           1
#define SPEED_HIGH          2
#define SPEED_VERY_HIGH     3

/*  @GPIO TYPE PULL UP OR PULL DOWN MACROS  */

#define GPIO_NO_PUPD          0
#define GPIO_PU               1
#define GPIO_PD               2



/* @GPIO Alternate Function Macros */

#define GPIO_AF0               0
#define GPIO_AF1               1
#define GPIO_AF2               2
#define GPIO_AF3               3
#define GPIO_AF4               4
#define GPIO_AF5               5
#define GPIO_AF6               6
#define GPIO_AF7               7
#define GPIO_AF8               8
#define GPIO_AF9               9
#define GPIO_AF10              10
#define GPIO_AF11              11
#define GPIO_AF12              12
#define GPIO_AF13              13
#define GPIO_AF14              14
#define GPIO_AF15              15

#endif
