#ifndef _BRD_DMA_H
#define _BRD_DMA_H

#include <MDR32F9Qx_dma.h>

void BRD_DMA_Init(void);
void BRD_DMA_Init_Channel (uint32_t DMA_Channel, DMA_ChannelInitTypeDef*  pDMAChannelInitStruct);

#endif	// _BRD_DMA_H
