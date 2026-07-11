/*
 * stm32f401ccux_gpio.c
 *
 *  Created on: 07-Jul-2026
 *      Author: veeresh
 */


#include<stdint.h>
#include"stm32f401ccux_gpio_driver.h"
#include"STM32F401CCUx.h"
/**********************************************
 * @function            - GPIO_PeriClockControl
 *
 * Brief                - Enables peripheral clock for given GPIO port
 *
 * @param [int 1]       -  GPIO Portx  [X: A, B , C ...]
 *
 * @param [int 2]       - EN_DI used to enable or disable (  ENABLE , DISABLE)
 *
 * @param [int 3]       - None
 *
 * @return              - None
 *
 * @note                - None
 */

/*  CLOCK API's  */
void GPIO_PeriClockControl(gpio_reg_def *GPIOx , uint8_t  EN_DI)
{

	if(EN_DI == ENABLE)
	{
		if(GPIOx  == GPIOA)
		{
			GPIOA_CLOCK_EN;
		}
		else if(GPIOx == GPIOB)
		{
			GPIOB_CLOCK_EN;
		}
		else if(GPIOx == GPIOC)
		{
			GPIOC_CLOCK_EN;
		}
	}
	else
	{
		if(GPIOx  == GPIOA)
		{
		GPIOA_CLOCK_DI;
		}
	  else if(GPIOx == GPIOB)
		{
	   GPIOB_CLOCK_DI;
	    }
	  else if(GPIOx == GPIOC)
	   {
	   GPIOC_CLOCK_DI;
	   }
     }

}
/* GPIO API's */

/**********************************************
 * @function            - GPIO_INIT
 *
 * Brief                - Initializes GPIO to Perform any required operations
 *
 * @param [int 1]       -  *pGPIOx_HANDLE  Adress of handler Struct
 *
 * @param [int 2]       - None
 *
 * @param [int 3]       - None
 *
 * @return              - None
 *
 * @note                - None
 */

void GPIO_INIT(gpio_handle_t  *pGPIOx_HANDLE)
{


  uint32_t temp = 0;
	// 1. Configure Mode

	if(pGPIOx_HANDLE->gpio_config.gpio_PinMode <= GPIO_MODE_ANALOG)
	{
		// the non Interrupt Mode
		/* gpio_PinMode and gpio_PinNumber is set by user  */
		pGPIOx_HANDLE->pGPIOx->MODER &= ~(0x3UL <<(2 * pGPIOx_HANDLE->gpio_config.gpio_PinNumber));
		temp = (pGPIOx_HANDLE->gpio_config.gpio_PinMode  << ( 2 * pGPIOx_HANDLE->gpio_config.gpio_PinNumber));
		pGPIOx_HANDLE->pGPIOx->MODER |= temp;
		temp = 0;
	}
	else
	{
		// Interrupt Configuration
		if(pGPIOx_HANDLE->gpio_config.gpio_PinMode ==  GPIO_IT_FT)
		{
			// 1. Enable  FTSR REGISTER
           EXTI->FTSR |= (1 << pGPIOx_HANDLE->gpio_config.gpio_PinNumber);
           // 2 . Clear the corresponding RTSR bit
           EXTI->RTSR &= ~(1 << pGPIOx_HANDLE->gpio_config.gpio_PinNumber);
		}
		else if(pGPIOx_HANDLE->gpio_config.gpio_PinMode ==  GPIO_IT_RT)
		{
			// 1. Enable RTSR REGISTER
			EXTI->RTSR |= (1 << pGPIOx_HANDLE->gpio_config.gpio_PinNumber);
			// 2 . Clear the corresponding RTSR bit
			EXTI->FTSR &= ~(1 << pGPIOx_HANDLE->gpio_config.gpio_PinNumber);
		}
		else if(pGPIOx_HANDLE->gpio_config.gpio_PinMode ==  GPIO_IT_RFT)
		{
			// 1. configure both RTSR AND FTSR
			EXTI->RTSR |= (1 << pGPIOx_HANDLE->gpio_config.gpio_PinNumber);
			EXTI->FTSR |= (1 << pGPIOx_HANDLE->gpio_config.gpio_PinNumber);
		}

		// 2 Configure Port Selection Using SYSCFG_EXTICR
		uint8_t temp1 = (pGPIOx_HANDLE ->gpio_config.gpio_PinNumber  / 4);
		uint8_t temp2 = (pGPIOx_HANDLE ->gpio_config.gpio_PinNumber % 4);
		uint8_t portcode = GPIO_BASE_ADD_TO_CODE  (pGPIOx_HANDLE ->pGPIOx);

		SYSCFG_PCLK_EN ;

		SYSCFG->SYSCFG_EXTICR[temp1] = portcode << (temp2 * 2);

		// 3  Enable EXTI Interrupt Delivery using IMR
		EXTI->IMR |= (1 << pGPIOx_HANDLE->gpio_config.gpio_PinNumber);

	}



	// 2. Configure Speed
	pGPIOx_HANDLE->pGPIOx->OSPEEDR &= ~(0x3UL <<(2 * pGPIOx_HANDLE->gpio_config.gpio_PinNumber));
	temp = (pGPIOx_HANDLE->gpio_config.gpio_PinSpeed  << (2* pGPIOx_HANDLE->gpio_config.gpio_PinNumber));
	pGPIOx_HANDLE->pGPIOx->OSPEEDR |= temp;
	temp = 0;



	// 3. configure PULL PULLDOWN reg
	pGPIOx_HANDLE->pGPIOx->PUPDR &= ~(0x3UL <<(2* pGPIOx_HANDLE->gpio_config.gpio_PinNumber));
	temp =(pGPIOx_HANDLE ->gpio_config.gpio_PinPupdControl  << (2* pGPIOx_HANDLE->gpio_config.gpio_PinNumber));
	pGPIOx_HANDLE->pGPIOx->PUPDR |= temp;
	temp = 0;


	// 4. Configure Output type Reg

	pGPIOx_HANDLE->pGPIOx->OTYPER &= ~(0x1UL << pGPIOx_HANDLE->gpio_config.gpio_PinNumber);
	temp =(pGPIOx_HANDLE ->gpio_config.gpio_PinOtype  << (pGPIOx_HANDLE->gpio_config.gpio_PinNumber));
	pGPIOx_HANDLE->pGPIOx->OTYPER |= temp;
	temp = 0;

	// 5. Configure Alternate Functionality
	if(pGPIOx_HANDLE->gpio_config.gpio_PinMode  == GPIO_MODE_ALT_FUN )
	{

		uint8_t temp1, temp2;


		temp1= (pGPIOx_HANDLE->gpio_config.gpio_PinNumber / 8);  // eg : pin = 10 so temp1 = 10/ 8 = 1

	    temp2 = (pGPIOx_HANDLE->gpio_config.gpio_PinNumber % 8);  // eg : pin = 10 so temp1 = 10 % 8 = 2

	    pGPIOx_HANDLE->pGPIOx->AFR[temp1] &= ~(0xFUL << ( 4 * temp2));  // temp2 at bit pos 8 so bit 8 to bit 11 will be cleared

	    pGPIOx_HANDLE->pGPIOx->AFR[temp1] |= pGPIOx_HANDLE->gpio_config.gpio_Alt_Func_Mode << (4 * temp2);
	    // here bit pos 8 to 11 are set to value of 0 to 15 [AF NUMBERS @GPIO Alternate Function Macros]
	}
}


/**********************************************
 * @function            - GPIO_DeINIT
 *
 * Brief                - Disables GPIO to Perform any required operations
 *
 * @param [int 1]       -  gpio_reg_def *pGPIOx  BAse add of GPIOx Port
 *
 * @param [int 2]       - None
 *
 * @param [int 3]       - None
 *
 * @return              - None
 *
 * @note                - None
 */

void GPIO_DeINIT(gpio_reg_def *pGPIOx)
{

}


/* API's for REading from Port or Pin */
/**********************************************
 * @function            - GPIO_Read_From_GPIOPORT
 *
 * Brief                - can be used to read data from selected port
 *
 * @param [int 1]       -  gpio_reg_def *pGPIOx  BAse add of GPIOx Port
 *
 * @param [int 2]       - None
 *
 * @param [int 3]       - None
 *
 * @return              - int
 *
 * @note                - None
 */


uint8_t GPIO_Read_From_GPIOPORT(gpio_reg_def *pGPIOx)
{
	uint16_t val ;
	  val = (uint16_t)(pGPIOx->IDR);
	  return val;  /// returns entire ports state!
}


/* API's for REading from Port or Pin */
/**********************************************
 * @function            - GPIO_Read_From_GPIOPORT
 *
 * Brief                - can be used to read data from selected PIN NUMBER of Perticular Port`
 *
 * @param [int 1]       -  gpio_reg_def *pGPIOx  BAse add of GPIOx Port
 *
 * @param [int 2]       - GPIO_PIN_NUM  (select pin Num)
 *
 * @param [int 3]       - None
 *
 * @return              - int
 *
 * @note                - None
 */

uint8_t GPIO_Read_From_GPIOPIN(gpio_reg_def *pGPIOx  , uint8_t GPIO_PIN_NUM)
{
  uint8_t val ;
  val = (uint8_t)(pGPIOx->IDR >> GPIO_PIN_NUM)  &  (0x00000001);
  return val;
}

/* API's for Writing from Port or Pin */

/* API's for Writing from Port or Pin */
/**********************************************
 * @function            - GPIO_Write_to_GPIOPORT
 *
 * Brief                - can be used to Write data from selected Port`
 *
 * @param [int 1]       - gpio_reg_def *pGPIOx  BAse add of GPIOx Port
 *
 * @param [int 2]       - None
 *
 * @param [int 3]       - None
 *
 * @return              - None
 *
 * @note                - None
 */

void GPIO_Write_to_GPIOPORT(gpio_reg_def *pGPIOx , uint16_t value)
{
	pGPIOx->ODR  = value;
}


/* API's for Writing from Port or Pin */
/**********************************************
 * @function            - GPIO_Write_to_GPIOPIN
 *
 * Brief                - can be used to Write data from selected perticular Ports specific Pin `
 *
 * @param [int 1]       - gpio_reg_def *pGPIOx  BAse add of GPIOx Port
 *
 * @param [int 2]       - GPIO_PIN_NUM  [Enter required PIN Num]
 *
 * @param [int 3]       - Value  [SET or RESET]
 *
 * @return              - None
 *
 * @note                - None
 */

void GPIO_Write_to_GPIOPIN(gpio_reg_def *pGPIOx  , uint8_t GPIO_PIN_NUM ,uint8_t value)
{
   if(value == GPIO_PIN_SET)
   {
   pGPIOx->ODR |= (SET << GPIO_PIN_NUM);
   }
   else
   {
	   pGPIOx->ODR &= ~(SET << GPIO_PIN_NUM);
   }

}

/* API's for Writing from Port or Pin */
/**********************************************
 * @function            - GPIO_Toggle_pin
 *
 * Brief                - can be used to Toggle Pin state from prev state.`
 *
 * @param [int 1]       - gpio_reg_def *pGPIOx  BAse add of GPIOx Port
 *
 * @param [int 2]       - GPIO_PIN_NUM  [Enter required PIN Num]
 *
 * @param [int 3]       - None
 *
 * @return              - None
 *
 * @note                - None
 */

void GPIO_Toggle_pin(gpio_reg_def *pGPIOx  , uint8_t GPIO_PIN_NUM)
{
	pGPIOx->ODR ^= (1 << GPIO_PIN_NUM);
}

/// IRQ Handling ///
void GPIO_IRQ_Handling(uint8_t PIN_NUMBER )
{

	// clear the EXTI_PR register
	if(EXTI->PR  & (1 << PIN_NUMBER)){
		// Clear the bit
		EXTI->PR |= (1 << PIN_NUMBER);
	}
}


void GPIO_IRQ_IT_CONFIG(uint8_t IRQ_number  , uint8_t EN_DI)
{
     if(EN_DI  == ENABLE){
    	 if(IRQ_number <= 31){
    		 // program ISER0 register
    		 NVIC_ISER0 |= (1<<IRQ_number);
    	 }
    	 else if(IRQ_number > 31 && IRQ_number < 64){
    		 // program ISER1 register
    		 NVIC_ISER1 |= (1<<(IRQ_number % 32));
    	 }
    	 else if(IRQ_number >= 64 && IRQ_number < 96){
    		 // program ISER2 register
    		 NVIC_ISER2 |= (1<<(IRQ_number % 32));
    	 }
     }
     else{
    	 if(IRQ_number <= 31){
    		 // program ICER1 register
    		 NVIC_ICER0 |= (1<<IRQ_number);
    	 }
    	 else if(IRQ_number > 31 && IRQ_number < 64){
    	    		 // program ICER1 register
    		 NVIC_ICER1 |= (1<<(IRQ_number % 32));
    	 }
    	  else if(IRQ_number >= 64 && IRQ_number < 96){
    	    		 // program ICER2 register
    		  NVIC_ICER2 |= (1<<(IRQ_number % 32));
    	 }

     }
}



void GPIO_PRIORITY_CONFIG(uint8_t IRQ_number,  uint32_t IRQ_priority){
	uint8_t iprx = IRQ_number / 4;
	uint8_t iprx_section = IRQ_number % 4;

	uint32_t shift_number = (8 *iprx_section) + (8 - NO_OF_PR_BITS_IMPLEMENTED);
	  *(NVIC_PR_BASE_ADD + iprx) &= ~(0xFUL << shift_number);
	    *(NVIC_PR_BASE_ADD + iprx) |=  (IRQ_priority << shift_number);
}

