/*
 * spi.h
 *
 *  Created on: 31-Jul-2026
 *      Author: viresh
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <stdint.h>
#include "stm32f401ccux_gpio_driver.h"
#include "STM32F401CCUx.h"



/*
 * Configuration Structure for SPIx peripherals!
 * */
typedef struct{
	uint8_t SPI_DeviceMode; // Master or Slave
	uint8_t SPI_BusConfig;
	uint8_t SPI_SCLKspeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;

}SPI_CONFIG_T;

/*
 * HAndle Strut for SPI peripheral struct!
 * */
typedef struct{
	SPI_REGDEF_T *pSPIx;
	SPI_CONFIG_T SPI_CONFIG;
}SPI_HANDLE_T;

/*
 * SPI Related Flags!
 */

#define SPI_TXE_FLAG      (1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG     (1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG     (1 << SPI_SR_BSY)

/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MASTER    1
#define SPI_DEVICE_SLAVE     0

/*
 * @SPI_BusConfig
 */

#define SPI_DEVICE_FD           1
#define SPI_DEVICE_HD           2
#define SPI_DEVICE_SIMPLEX_RX   3

/*
 * @SPI_SCLKspeed
 */

#define SPI_SCLK_DIV2          0
#define SPI_SCLK_DIV4          1
#define SPI_SCLK_DIV8          2
#define SPI_SCLK_DIV16         3
#define SPI_SCLK_DIV32         4
#define SPI_SCLK_DIV64         5
#define SPI_SCLK_DIV128        6
#define SPI_SCLK_DIV256        7


/*
 * @SPI_DFF
 */

#define SPI_DFF_8BIS          0
#define SPI_DFF_16BIS         1

/*
 * @SPI_CPOL
 */
#define SPI_CPOL_HIGH         1
#define SPI_CPOL_LOW          0

/*
 * @SPI_CPHA
 */
#define SPI_CPHA_HIGH        1
#define SPI_CPHA_LOW         0
/*
 * @SPI_SSM   by default SSM is 0
 */
#define SPI_SSM_EN          1
#define SPI_SSM_DI          0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////  API DEV SECTION   //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* SPI CLOCK API's  */
void SPI_PeriClockControl(SPI_REGDEF_T *pSPIx , uint8_t  EN_DI);


/* SPI INITIATE AND DEINITIATE API's */
void SPI_INIT(SPI_HANDLE_T  *pSPI_HANDLE);
void SPI_DeINIT(SPI_REGDEF_T *pSPIx);

/*
 * DATA SEND AND RECIEVE APIs
 * */
void SPI_SendData(SPI_REGDEF_T *pSPIx , uint8_t *pTxBuffer ,  uint32_t length);
void SPI_RecieveData(SPI_REGDEF_T *pSPIx , uint8_t *pRxBuffer ,  uint32_t length);





/*
 * IRQ CONFIG AND ISR HANDLING
 * */
void SPI_IRQ_IT_CONFIG(uint8_t IRQ_NUMBER ,uint8_t EN_DI);
void SPI_PRIORITY_CONFIG(uint8_t IRQ_number,  uint32_t IRQ_priority);
void SPI_IRQ_Handling(SPI_HANDLE_T *pHANDLE);



/*
 * Other Peripheral Control APIs*/
void SPI_Peripheral_control(SPI_REGDEF_T *pSPIx , uint8_t EnorDi);

void SPI_SSI_Config(SPI_REGDEF_T *pSPIx , uint8_t EnorDi);

void SPI_SSOE_Config(SPI_REGDEF_T *pSPIx, uint8_t EnorDi);

uint8_t SPI_Get_Flag_status(SPI_REGDEF_T *pSPIx , uint32_t Flag);

#endif /* INC_SPI_H_ */
