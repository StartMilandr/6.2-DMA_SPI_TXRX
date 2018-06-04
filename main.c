#include "brdClock.h"
#include "brdLed.h"
#include "brdUtils.h"
#include "brdSPI.h"
#include "brdDMA.h"

//  Определения для отладочной платы выбранной в "brdSelect.h"
#include "brdDef.h"
//  Выбор SPI и настроек - модифицируется под реализацию
#include "brdSPI_Select.h"
//  Выбор настроек DMA - модифицируется под реализацию
#include "brdDMA_Select.h"

#define LED_OK    BRD_LED_1
#define LED_ERR   BRD_LED_2

#define DELAY_TICKS     4000000
#define SPI_MASTER_MODE 1

#ifdef USE_MDR1986VE9x
  #define DMA_CH_SPI_TX  DMA_Channel_SSP2_TX
  #define DMA_CH_SPI_RX  DMA_Channel_SSP2_RX
#elif defined (USE_MDR1986VE1T)
  #define DMA_CH_SPI_TX  DMA_Channel_REQ_SSP1_TX
  #define DMA_CH_SPI_RX  DMA_Channel_REQ_SSP1_RX
#endif

#define  DATA_COUNT  10
uint32_t DestBuf[DATA_COUNT] __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
uint32_t SrcBuf[DATA_COUNT]  __attribute__((section("EXECUTABLE_MEMORY_SECTION")));

DMA_CtrlDataInitTypeDef DMA_DATA_TX;
DMA_CtrlDataInitTypeDef DMA_DATA_RX;
DMA_ChannelInitTypeDef  DMA_TX;
DMA_ChannelInitTypeDef  DMA_RX;

extern DMA_CtrlDataTypeDef DMA_ControlTable[DMA_Channels_Number * (1 + DMA_AlternateData)];

uint32_t DMA_Completed = 0;
uint32_t TX_Started = 0;


void PrepareData(void)
{
  uint32_t i;
  
  for (i = 0; i < DATA_COUNT; i++)
  {
    DestBuf[i] = 0;
    SrcBuf[i]  = i + 1; //  Fill with any data
  }  
}  

int main(void)
{
  uint32_t i;
  uint32_t errCnt;
  
  DMA_DATA_TX = DMA_DataCtrl_Pri;
  DMA_DATA_RX = DMA_DataCtrl_Pri;
  DMA_TX = DMA_ChanCtrl;
  DMA_RX = DMA_ChanCtrl;  
    
  //  Clock
  BRD_Clock_Init_HSE_PLL(10); // 80MHz
  
  //  Controls
  BRD_LEDs_Init();
  
  //  SPI Init
  // BRD_SPI_PortInit(pBRD_SPIx);  // В тестовом режиме SPI выводы не используются, в реальном включении - раскомментировать!
  BRD_SPI_Init(pBRD_SPIx, SPI_MASTER_MODE);

  //  DMA Init
  BRD_DMA_Init();
  
  //  DMA SPI TX
  DMA_DATA_TX.DMA_SourceBaseAddr = (uint32_t)&SrcBuf;  
  DMA_DATA_TX.DMA_DestBaseAddr   = (uint32_t)&pBRD_SPIx->SPIx->DR;
  DMA_DATA_TX.DMA_SourceIncSize  = DMA_SourceIncWord;  
  DMA_DATA_TX.DMA_DestIncSize    = DMA_DestIncNo;    
  DMA_DATA_TX.DMA_CycleSize      = DATA_COUNT;
  
  DMA_TX.DMA_PriCtrlData = &DMA_DATA_TX;
  DMA_TX.DMA_AltCtrlData = &DMA_DATA_TX;
  
  BRD_DMA_Init_Channel(DMA_CH_SPI_TX, &DMA_TX);
  
  //  DMA SPI RX
  DMA_DATA_RX.DMA_SourceBaseAddr = (uint32_t)&pBRD_SPIx->SPIx->DR;  
  DMA_DATA_RX.DMA_DestBaseAddr   = (uint32_t)&DestBuf;
  DMA_DATA_RX.DMA_SourceIncSize  = DMA_SourceIncNo;  
  DMA_DATA_RX.DMA_DestIncSize    = DMA_DestIncWord;      
  DMA_DATA_RX.DMA_CycleSize      = DATA_COUNT;

  DMA_RX.DMA_PriCtrlData = &DMA_DATA_RX;
  DMA_RX.DMA_AltCtrlData = &DMA_DATA_RX;

  BRD_DMA_Init_Channel(DMA_CH_SPI_RX, &DMA_RX);  
  
  //  DMA Transfer Start
  TX_Started = 1;
  DMA_Completed = 0;
  PrepareData();
  
  SSP_DMACmd(pBRD_SPIx->SPIx, SSP_DMA_TXE | SSP_DMA_RXE, ENABLE);
  
  // Write and read Ack
  while (1)
  {  
    //  Wait DMA Completed
    while (!DMA_Completed);
    
    //  Check Data
    errCnt = 0;
    for (i = 0; i < DATA_COUNT; i++)
      if (DestBuf[i] != SrcBuf[i])
        errCnt++;
      
    if (!errCnt)  
    {  
      BRD_LED_Switch(LED_OK);
      BRD_LED_Set(LED_ERR, 0);
    }  
    else
      BRD_LED_Set(LED_ERR, 1);
    
    //  Wait for Led show
    Delay(DELAY_TICKS);    
    
    //  DMA Start Next Cycle
    TX_Started = 1;
    DMA_Completed = 0;    
    PrepareData();
        
    //  DMA Next Transfer Start
    DMA_Init(DMA_CH_SPI_TX, &DMA_TX);
    DMA_Init(DMA_CH_SPI_RX, &DMA_RX);
    DMA_Cmd(DMA_CH_SPI_TX, ENABLE);
    DMA_Cmd(DMA_CH_SPI_RX, ENABLE);
    SSP_DMACmd(pBRD_SPIx->SPIx, SSP_DMA_TXE | SSP_DMA_RXE, ENABLE);
  }
}

void DMA_IRQHandler (void)
{
  uint32_t DMA_Ctrl_Tx = DMA_ControlTable[DMA_CH_SPI_TX].DMA_Control;
  uint32_t DMA_Ctrl_Rx = DMA_ControlTable[DMA_CH_SPI_RX].DMA_Control;

  // Disable DMA TX
  if (((DMA_Ctrl_Tx & 3) == 0) && TX_Started)   
  {
    SSP_DMACmd(pBRD_SPIx->SPIx, SSP_DMA_TXE, DISABLE);
    DMA_Cmd(DMA_CH_SPI_TX, DISABLE);
    
    TX_Started = 0;
  }  

  // Disable DMA RX  
  if ((DMA_Ctrl_Rx & 3) == 0)
  {
    SSP_DMACmd(pBRD_SPIx->SPIx, SSP_DMA_RXE, DISABLE);
    DMA_Cmd(DMA_CH_SPI_RX, DISABLE);
    
    DMA_Completed = 1;
  }    
      
  NVIC_ClearPendingIRQ (DMA_IRQn);
}	
