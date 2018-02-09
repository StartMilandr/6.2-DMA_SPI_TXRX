#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
//#include <MDR32F9Qx_dma.h>

#include "brdDMA.h"
#include "brdDef.h"

void BRD_DMA_Init(void)
{
  // Включение тактирования модуля DMA
  RST_CLK_PCLKcmd (DMA_CLOCK_SELECT, ENABLE);

  // Сброс прерывания от DMA
  NVIC_ClearPendingIRQ (DMA_IRQn);

  // Деинициализация DMA
  DMA_DeInit();	
}  

// Инициализация модуля DMA
void BRD_DMA_Init_Channel (uint32_t DMA_Channel, DMA_ChannelInitTypeDef*  pDMAChannelInitStruct)
{
  // Инициализация канала DMA,
	DMA_Init(DMA_Channel, pDMAChannelInitStruct);

  // Назначение приоритета аппаратных прерываний от DMA
  NVIC_SetPriority (DMA_IRQn, 1);

  // Включение аппаратных прерываний от DMA
  NVIC_EnableIRQ (DMA_IRQn); 
	
  DMA_Cmd(DMA_Channel, ENABLE);
}

//// Прототип - Обработчик прерываний от DMA
//void DMA_IRQHandler (void)
//{
//  // Формирование нового цикла аналого-цифровых преобразований
//  DMA_Init (DMA_Channel, &DMAChannelInitStruct);
//  NVIC_ClearPendingIRQ (DMA_IRQn);
//}	


