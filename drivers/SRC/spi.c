/*
 * spi.c
 *
 *  Created on: 31-Jul-2026
 *      Author: viresh
 */


#include <stdint.h>
#include "stm32f401ccux_gpio_driver.h"
#include "STM32F401CCUx.h"
#include "spi.h"


/* SPI CLOCK API's  */
void SPI_Peripheral_control(SPI_REGDEF_T *pSPIx , uint8_t EnorDi)
{
	if(EnorDi == ENABLE){
		pSPIx->SPI_CR1 |= (1 << SPI_CR1_SPE);
	}
	else{
		pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SPE);
	}
}


void SPI_SSOE_Config(SPI_REGDEF_T *pSPIx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
        pSPIx->SPI_CR2 |= (1 << SPI_CR2_SSOE);
    else
        pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_SSOE);
}


void SPI_SSI_Config(SPI_REGDEF_T *pSPIx , uint8_t EnorDi)
{
	if(EnorDi == ENABLE){
			pSPIx->SPI_CR1 |= (1 << SPI_CR1_SSI);
		}
		else{
			pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SSI);
		}
}


void SPI_PeriClockControl(SPI_REGDEF_T *pSPIx , uint8_t  EN_DI)
{

	if(EN_DI == ENABLE)
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_EN;
			}
			else if(pSPIx == SPI2)
			{
				SPI2_PCLK_EN;
			}
			else if(pSPIx == SPI3)
			{
				SPI3_PCLK_EN;
			}

		}
		else
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_DI();
			}
			else if(pSPIx == SPI2)
			{
				SPI2_PCLK_DI();
			}
			else if(pSPIx == SPI3)
			{
				SPI3_PCLK_DI();
			}

	     }
}


/* SPI INITIATE AND DEINITIATE API's */
void SPI_INIT(SPI_HANDLE_T  *pSPI_HANDLE)
{
	SPI_PeriClockControl(pSPI_HANDLE->pSPIx, ENABLE);
/// configure CR1 REG
	uint32_t temp = 0;
	// Cnfigure Device Mode!
	temp |= pSPI_HANDLE->SPI_CONFIG.SPI_DeviceMode << SPI_CR1_MSTR;  //SPI_CR1_MSTR

	// Bus Config

	if(pSPI_HANDLE->SPI_CONFIG.SPI_BusConfig == SPI_DEVICE_FD){
		// BIDI Mode should be Cleared
		temp &= ~(1 << SPI_CR1_BIDI_MODE);  // SPI_CR1_BIDI_MODE
	}
	else if(pSPI_HANDLE->SPI_CONFIG.SPI_BusConfig == SPI_DEVICE_HD){
		// BIDI MODE IS ENABLED!
		temp |= (1 << SPI_CR1_BIDI_MODE); // SPI_CR1_BIDI_MODE
	}
	else if(pSPI_HANDLE->SPI_CONFIG.SPI_BusConfig == SPI_DEVICE_SIMPLEX_RX){
		// BIDI  must be cleared
		temp &= ~(1 << SPI_CR1_BIDI_MODE); // SPI_CR1_BIDI_MODE
		// Rx only bit must be set!
		temp |= (1 << SPI_CR1_RX_ONLY); // SPI_CR1_RX_ONLY
	}

	// config SCLK(baud rate)
	temp |= pSPI_HANDLE->SPI_CONFIG.SPI_SCLKspeed << SPI_CR1_BR; // SPI_CR1_BR
	// Config DFF
	temp |= pSPI_HANDLE->SPI_CONFIG.SPI_DFF << SPI_CR1_DFF;  // SPI_CR1_DFF
	// Config CPHA
	temp |= pSPI_HANDLE->SPI_CONFIG.SPI_CPHA << SPI_CR1_CPHA;  // SPI_CR1_CPHA
	// connfig CPOL
	temp |= pSPI_HANDLE->SPI_CONFIG.SPI_CPOL << SPI_CR1_CPOL; // SPI_CR1_CPOL

	// Configure Software Slave Management (SSM)
	temp |= (pSPI_HANDLE->SPI_CONFIG.SPI_SSM << SPI_CR1_SSM);

	pSPI_HANDLE->pSPIx->SPI_CR1 = temp;
}

void SPI_DeINIT(SPI_REGDEF_T *pSPIx)
{

}

/*
 * DATA SEND AND RECIEVE APIs
 * */
uint8_t SPI_Get_Flag_status(SPI_REGDEF_T *pSPIx , uint32_t Flag)
{
	if(pSPIx->SPI_SR  &  Flag)  return Flag_Set;
	return Flag_Reset;
}

void SPI_SendData(SPI_REGDEF_T *pSPIx , uint8_t *pTxBuffer ,  uint32_t length)
{
	uint32_t len = length;
	while(len){
	// check length isnt 0 if 0 return

	// check TX buffer is empty if isnt wait till its empty we can use SR registers TXE bit to monitor buffer
	//	while(!((pSPIx->SPI_SR) & (1 << 1))); //wait!
		while(SPI_Get_Flag_status(pSPIx, SPI_TXE_FLAG) == Flag_Reset);

		// 2. Check DFF bit in CR1
		if(pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF)){
			// 16 bit data frame format!
			//1. Load Data into the DR register!
			pSPIx->SPI_DR = *((uint16_t*) pTxBuffer);
			len--;
			len--;
			pTxBuffer += 2;
		}
		else{
			// 8 bit Data frame Format!
			pSPIx->SPI_DR = *(pTxBuffer);
			len--;
			pTxBuffer++;
		}

	}

}


void SPI_RecieveData(SPI_REGDEF_T *pSPIx , uint8_t *pRxBuffer ,  uint32_t length)
{

	uint32_t len = length;
	while(len){
	// check length isnt 0 if 0 return

	// check RXNE buffer is empty if isnt wait till its empty we can use SR registers RXNE bit to monitor buffer
	//	while(!((pSPIx->SPI_SR) & (1 << 1))); //wait!
		while(SPI_Get_Flag_status(pSPIx, SPI_RXNE_FLAG) == Flag_Reset);

		// 2. Check DFF bit in CR1
		if(pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF)){
			// 16 bit data frame format!
			//1. Load Data FROM the DR register to Rx Buff !
			 *((uint16_t*) pRxBuffer)= pSPIx->SPI_DR;
			len--;
			len--;
			pRxBuffer += 2;
		}
		else{
			// 8 bit Data frame Format!
			*(pRxBuffer)=pSPIx->SPI_DR;
			len--;
			pRxBuffer++;
		}

	}
}





/*
 * IRQ CONFIG AND ISR HANDLING
 * */
void SPI_IRQ_IT_CONFIG(uint8_t IRQ_NUMBER ,uint8_t EN_DI);
void SPI_PRIORITY_CONFIG(uint8_t IRQ_number,  uint32_t IRQ_priority);
void SPI_IRQ_Handling(SPI_HANDLE_T *pHANDLE);

