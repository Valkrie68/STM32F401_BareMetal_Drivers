/*
 * STM32F401CCUx.h
 *
 *  Created on: 01-Jul-2026
 *      Author: viresh
 */
#include<stdint.h>
#ifndef INC_STM32F401CCUX_H_
#define INC_STM32F401CCUX_H_


/*  ARM CORTEX M4 SPECIFIC DETAILS */

#define NVIC_ISER0              (*(volatile uint32_t*)(0xE000E100UL))
#define NVIC_ISER1              (*(volatile uint32_t*)(0xE000E104UL))
#define NVIC_ISER2              (*(volatile uint32_t*)(0xE000E108UL))
#define NVIC_ISER3              (*(volatile uint32_t*)(0xE000E10CUL))



#define NVIC_ICER0              (*(volatile uint32_t *) (0xE000E180UL))
#define NVIC_ICER1              (*(volatile uint32_t *) (0xE000E184UL))
#define NVIC_ICER2              (*(volatile uint32_t *) (0xE000E188UL))
#define NVIC_ICER3              (*(volatile uint32_t *) (0xE000E18CUL))

#define NVIC_IPR0               ((volatile uint32_t *) (0xE000E400UL))



#define NVIC_PR_BASE_ADD        NVIC_IPR0

#define NO_OF_PR_BITS_IMPLEMENTED    4


//////////////////////  SYSTICK ADRESS ///////////////////
#define SYSTICK_BASE_ADDR       0xE000E010UL

#define SYSTICK ((SYSTICK_RegDef_t *)SYSTICK_BASE_ADDR)



#define RCC_BASE_ADD            0x40023800UL


#define FLASH_BASE_ADD          0x08000000UL
#define ROM_BASE_ADD            0x1FFF0000UL
#define SRAM1_BASE_ADD          0x20000000UL  //(64KB)
#define AHB1_BASE_ADD           0x40020000UL
#define AHB2_BASE_ADD           0x50000000UL
#define APB1_BASE_ADD           0x40000000UL
#define APB2_BASE_ADD           0x40010000UL


//// GPIO BASE ADD ON AHB1 //////
#define GPIOA_BASE_ADD          0x40020000UL
#define GPIOB_BASE_ADD          0x40020400UL
#define GPIOC_BASE_ADD          0x40020800UL


//////  pheri Pherals on APB1 BUS ///////////

#define I2C1_BASE_ADD           0x40005400UL
#define I2C2_BASE_ADD           0x40005800UL
#define I2C3_BASE_ADD           0x40005C00UL

#define SPI2_BASE_ADD           0x40003800UL
#define SPI3_BASE_ADD           0x40003C00UL

#define USART2_BASE_ADD         0x40004400UL

////////// Pheripherals on APB2 BUS /////////
#define USART1_BASE_ADD         0x40011000UL
#define USART6_BASE_ADD         0x40011400UL
#define SPI1_BASE_ADD           0x40013000UL
#define EXTI_BASE_ADD           0x40013C00UL
#define SYSCFG_BASE_ADD         0x40013800UL


typedef struct
{
   volatile uint32_t MODER;            /* writing all the registers in astructure so that they can be accesed Easily */
   volatile uint32_t OTYPER;
   volatile uint32_t OSPEEDR;
   volatile uint32_t PUPDR;
   volatile uint32_t IDR;
   volatile uint32_t ODR;
   volatile uint32_t BSRR;
   volatile uint32_t LCKR;
   volatile uint32_t AFR[2];   // an Array of 2 register meant to storeAFRL and AFRH register data
	// GPIO alternate function low register and GPIO alternate function HIGH register
} gpio_reg_def;

typedef struct{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	uint32_t reser1;
	uint32_t reser2;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t reser3;
	uint32_t reser4;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	uint32_t reser5;
	uint32_t reser6;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t reser7;
	uint32_t reser8;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	uint32_t reser9;
	uint32_t reser10;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t reser11;
	uint32_t reser12;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t reser13;
	uint32_t reser14;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	uint32_t reser15;
	volatile uint32_t DCKCFGR;
}RCC_reg_def;



typedef struct
{
    volatile uint32_t CTRL;    // 0x00
    volatile uint32_t LOAD;    // 0x04
    volatile uint32_t VAL;     // 0x08
    volatile uint32_t CALIB;   // 0x0C
} SYSTICK_RegDef_t;


/* Pheripheral definitions  (just base adrees of GPIOA type casted into gpio_reg_def) */

#define GPIOA              ((gpio_reg_def*) (GPIOA_BASE_ADD) )
#define GPIOB              ((gpio_reg_def*) (GPIOB_BASE_ADD) )
#define GPIOC              ((gpio_reg_def*) (GPIOC_BASE_ADD) )


#define RCC                 ((RCC_reg_def *) RCC_BASE_ADD)
#define EXTI                ((exti_regdef_t *) EXTI_BASE_ADD)
#define SYSCFG              ((syscfg_regdef_t *) (SYSCFG_BASE_ADD))
/* Enable MAcros for GPIOx  */

#define GPIOA_CLOCK_EN       (RCC->AHB1ENR  |= (1 << 0))
#define GPIOB_CLOCK_EN       (RCC->AHB1ENR  |= (1 << 1))
#define GPIOC_CLOCK_EN       (RCC->AHB1ENR  |= (1 << 2))

/* Enable cllk for I2Cx */
#define I2C1_PCLK_EN         (RCC->APB1ENR  |= (1<<21))
#define I2C2_PCLK_EN         (RCC->APB1ENR  |= (1<<22))
#define I2C3_PCLK_EN         (RCC->APB1ENR  |= (1<<23))

/* enable clk for SPI */
#define SPI1_PCLK_EN         (RCC->APB2ENR  |= (1<<12))
#define SPI2_PCLK_EN         (RCC->APB1ENR  |= (1<<14))
#define SPI3_PCLK_EN         (RCC->APB1ENR  |= (1<<15))
//#define SPI4_PCLK_EN         (RCC->APB2ENR  |= (1<<13))

/* enable clk for usart */
#define USART1_PCLK_EN       (RCC->APB2ENR  |= (1<<4))
#define USART2_PCLK_EN       (RCC->APB1ENR  |= (1<<17))
#define USART6_PCLK_EN       (RCC->APB2ENR  |= (1<<5))


/* Enable Clck for SYSCFG  */

#define SYSCFG_PCLK_EN       (RCC->APB2ENR  |= (1<<14))




/* disable MAcros for GPIOx  */

#define GPIOA_CLOCK_DI       (RCC->AHB1ENR  &= ~(1 << 0))
#define GPIOB_CLOCK_DI       (RCC->AHB1ENR  &= ~(1 << 1))
#define GPIOC_CLOCK_DI       (RCC->AHB1ENR  &= ~(1 << 2))

/* Enable cllk for I2Cx */
#define I2C1_PCLK_DI         (RCC->APB1ENR  &= ~(1<<21))
#define I2C2_PCLK_DI         (RCC->APB1ENR  &= ~(1<<22))
#define I2C3_PCLK_DI         (RCC->APB1ENR  &= ~(1<<23))

/* enable clk for SPI */
#define SPI1_PCLK_DI()         (RCC->APB2ENR  &= ~(1<<12))
#define SPI2_PCLK_DI()         (RCC->APB1ENR  &= ~(1<<14))
#define SPI3_PCLK_DI()         (RCC->APB1ENR  &= ~(1<<15))
//#define SPI4_PCLK_DI()         (RCC->APB2ENR  &= ~(1<<13))

/* enable clk for usart */
#define USART1_PCLK_DI       (RCC->APB2ENR  &= ~(1<<4))
#define USART2_PCLK_DI       (RCC->APB1ENR  &= ~(1<<17))
#define USART6_PCLK_DI       (RCC->APB2ENR  &= ~(1<<5))


/* Enable Clck for SYSCFG  */

#define SYSCFG_PCLK_DI       (RCC->APB2ENR  &= ~(1<<14))



////   SOME GENERIC MACROS FOR FUNCTION  //////////

#define ENABLE                   1
#define DISABLE                  0
#define SET                      ENABLE
#define RESET                    DISABLE
#define Flag_Reset               RESET
#define Flag_Set                 SET
#define GPIO_PIN_SET             SET
#define GPIO_PIN_RESET           RESET
#define GPIOA_REG_RESET         do {(RCC->AHB1STR  |= (1<<0));     (RCC->AHB1STR &= ~(1<<0));}while(0)
#define GPIOB_REG_RESET         do {(RCC->AHB1STR  |= (1<<1));     (RCC->AHB1STR &= ~(1<<1));}while(0)
#define GPIOC_REG_RESET         do {(RCC->AHB1STR  |= (1<<2));     (RCC->AHB1STR &= ~(1<<2));}while(0)




#define GPIO_BASE_ADD_TO_CODE(x) \
    ( ((x) == GPIOA) ? 0 : \
      ((x) == GPIOB) ? 1 : \
      ((x) == GPIOC) ? 2 : \
      0 )
/**
 * do {(RCC->AHB1STR  |= (1<<0));     (RCC->AHB1STR &= ~(1<<0));}while(0)  THIS METHOD IS USED TO
 * - SET A BIT AND CLEAR IT INSTANTANEOULSY JUST ENOUGH TO CODE TO WORK!
 */

////// MACROS FOR IRQ NUMBERS FOR NVIC  /////////

#define IRQ_NUMBER_EXTI0        6
#define IRQ_NUMBER_EXTI1        7
#define IRQ_NUMBER_EXTI2        8
#define IRQ_NUMBER_EXTI3        9
#define IRQ_NUMBER_EXTI4        10
#define IRQ_NUMBER_EXTI5_9      23
#define IRQ_NUMBER_EXTI10_15    40



//////////  MACROS FOR SYSTICK  CLK////////////
#define SYSTICK_CLKSOURCE_INTERNAL   1U
#define SYSTICK_CLKSOURCE_EXTERNAL   0U  // div / 8
#define SYSTICK_CLOCK_HZ             16000000U //// Internal

/*
 * Complete SPI register definitions
 * */
typedef struct{
	volatile uint32_t SPI_CR1;  // 00
	volatile uint32_t SPI_CR2;  //04
	volatile uint32_t SPI_SR;    //08
	volatile uint32_t SPI_DR;    //0c
	volatile uint32_t SPI_CRCPR;  // CRC Polynomial Reg 10
	volatile uint32_t SPI_RXCRCR; // 14
	volatile uint32_t SPI_TXCRCR; // 18
	volatile uint32_t SPI_I2SCFGR; // 1C
	volatile uint32_t SPI_I2SPR; // 0x20

}SPI_REGDEF_T;

#define SPI1      ((SPI_REGDEF_T*)(SPI1_BASE_ADD))  // APB2 BUS
#define SPI2      ((SPI_REGDEF_T*)(SPI2_BASE_ADD))  // APB1 BUS
#define SPI3      ((SPI_REGDEF_T*)(SPI3_BASE_ADD))  // APB1 BUS
//#define SPI4      ((SPI_REGDEF_T*)(SPI4_BASE_ADD))  // APB2 BUS

/*
 * SOME MACROS FOR SPI
 */
// CR1 bits
#define SPI_CR1_CPHA      0
#define SPI_CR1_CPOL      1
#define SPI_CR1_MSTR      2
#define SPI_CR1_BR        3
#define SPI_CR1_SPE       6
#define SPI_CR1_LSB1      7
#define SPI_CR1_SSI       8
#define SPI_CR1_SSM       9
#define SPI_CR1_RX_ONLY   10
#define SPI_CR1_DFF       11
#define SPI_CR1_CRC_NEXT  12
#define SPI_CR1_CRC_EN    13
#define SPI_CR1_BIDI_OE   14
#define SPI_CR1_BIDI_MODE 15
// CR2 bits
#define SPI_CR2_RXDMAEN   0
#define SPI_CR2_TXDMAEN   1
#define SPI_CR2_SSOE      2
#define SPI_CR2_RESERVED  3
#define SPI_CR2_FRF       4
#define SPI_CR2_ERRIE     5
#define SPI_CR2_RXNEIE    6
#define SPI_CR2_TXEIE     7

// SR bits
#define SPI_SR_RXNE        0
#define SPI_SR_TXE         1
#define SPI_SR_CHSIDE      2
#define SPI_SR_UDR         3
#define SPI_SR_CRCERR      4
#define SPI_SR_MODF        5
#define SPI_SR_OVR         6
#define SPI_SR_BSY         7
#define SPI_SR_FRE         8

#endif /* INC_STM32F401CCUX_H_ */
