/**
  ******************************************************************************
  * @file    sd_stm32.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   efs µSD card driver's low level interface for STM32F107 (using 
  *          µSD card provided with STM3210C-EVAL board) 
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_spi.h"
#include "interface/sd.h"
#include "config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Select MSD Card: ChipSelect pin low  */
#define MSD_CS_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_4)
/* Deselect MSD Card: ChipSelect pin high */
#define MSD_CS_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_4)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SPI_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the SPI and GPIOs used to drive the µSD card.
  * @param  None
  * @retval None
  */
void SPI_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* GPIOA and GPIOC Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);

  /* SPI3 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

  /* Configure SPI3 pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PA4 pin: CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* SPI3 Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI3, &SPI_InitStructure);

  /* SPI3 enable */
  SPI_Cmd(SPI3, ENABLE);
}

/*****************************************************************************/

esint8 if_readBuf(hwInterface* file, euint32 address, euint8* buf)
{
  return(sd_readSector(file, address, buf, 512));
}
/*****************************************************************************/

esint8 if_writeBuf(hwInterface* file, euint32 address, euint8* buf)
{
  return(sd_writeSector(file, address, buf));
}
/*****************************************************************************/

esint8 if_setPos(hwInterface* file, euint32 address)
{
  return(0);
}
/*****************************************************************************/

// Utility-functions which does not toogle CS.
// Only needed during card-init. During init
// the automatic chip-select is disabled for SSP

static euint8 my_if_spiSend(hwInterface *iface, euint8 outgoing)
{
  euint8 incoming;

  SPI_I2S_SendData(SPI3, outgoing);
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
  incoming = SPI_I2S_ReceiveData(SPI3);

  return(incoming);
}
/*****************************************************************************/

void if_spiInit(hwInterface *iface)
{
  euint8 i;

  SPI_Config();

  MSD_CS_HIGH();

  /* Send 20 spi commands with card not selected */
  for (i = 0;i < 21;i++)
    my_if_spiSend(iface, 0xff);
}
/*****************************************************************************/

euint8 if_spiSend(hwInterface *iface, euint8 outgoing)
{
  euint8 incoming;

  MSD_CS_LOW();

  SPI_I2S_SendData(SPI3, outgoing);

  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

  incoming = SPI_I2S_ReceiveData(SPI3);

  MSD_CS_HIGH();

  return(incoming);
}
/*****************************************************************************/

esint8 if_initInterface(hwInterface* file, eint8* opts)
{
  euint32 sc;

  if_spiInit(file); /* init at low speed */

  if (sd_Init(file) < 0)
  {
    DBG((TXT("Card failed to init, breaking up...\n")));
    return(-1);
  }
  if (sd_State(file) < 0)
  {
    DBG((TXT("Card didn't return the ready state, breaking up...\n")));
    return(-2);
  }

  sd_getDriveSize(file, &sc);
  file->sectorCount = sc / 512;
  if ( (sc % 512) != 0)
  {
    file->sectorCount--;
  }

  DBG((TXT("Drive Size is %lu Bytes (%lu Sectors)\n"), sc, file->sectorCount));
  DBG((TXT("Init done...\n")));

  return(0);
}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
