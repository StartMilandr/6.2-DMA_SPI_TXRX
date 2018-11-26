#ifndef BRD_SPI_SELECT_H
#define BRD_SPI_SELECT_H

#include "brdDef.h"
#include "brdSPI.h"

#if defined (USE_MDR1986VE1T)

  #define  BRD_SPI_PORT_CLK         BRD_SPI1_D_PORT_CLK 
  #define  BRD_SPI_PORT             BRD_SPI1_D_PORT
  #define  BRD_SPI_PINS             BRD_SPI1_D_PINS
  #define  BRD_SPI_PINS_FUNC        BRD_SPI1_D_PINS_FUNC
  #define  BRD_SPI_PINS_FUNC_CLEAR  BRD_SPI1_D_PINS_FUNC_CLEAR      
  
  #define  BRD_SPI_CLOCK          RST_CLK_PCLK_SSP1
  #define  BRD_SPI                MDR_SSP1

#elif defined (USE_MDR1986VE3)

  #define  BRD_SPI_PORT_CLK         BRD_SPI1_C_PORT_CLK 
  #define  BRD_SPI_PORT             BRD_SPI1_C_PORT
  #define  BRD_SPI_PINS             BRD_SPI1_C_PINS
  #define  BRD_SPI_PINS_FUNC        BRD_SPI1_C_PINS_FUNC
  #define  BRD_SPI_PINS_FUNC_CLEAR  BRD_SPI1_C_PINS_FUNC_CLEAR      
  
  #define  BRD_SPI_CLOCK          RST_CLK_PCLK_SSP1
  #define  BRD_SPI                MDR_SSP1

#elif defined (USE_MDR1986VE9x)

  //  SPI Selection
  //#define SPI2_PORT_F
  #define SPI2_PORT_D

  #if defined (SPI2_PORT_F)  
    #define  BRD_SPI_PORT_CLK         BRD_SPI1_F_PORT_CLK 
    #define  BRD_SPI_PORT             BRD_SPI1_F_PORT
    #define  BRD_SPI_PINS             BRD_SPI1_F_PINS
    #define  BRD_SPI_PINS_FUNC        BRD_SPI1_F_PINS_FUNC 
    #define  BRD_SPI_PINS_FUNC_CLEAR  BRD_SPI1_F_PINS_FUNC_CLEAR
  
    #define  BRD_SPI_CLOCK         (RST_CLK_PCLK_SSP2)  // All SSP and DMA - from Errata
    #define  BRD_SPI               MDR_SSP1
    
  #elif defined (SPI2_PORT_D)
    #define  BRD_SPI_PORT_CLK         BRD_SPI2_D_PORT_CLK 
    #define  BRD_SPI_PORT             BRD_SPI2_D_PORT
    #define  BRD_SPI_PINS             BRD_SPI2_D_PINS
    #define  BRD_SPI_PINS_FUNC        BRD_SPI2_D_PINS_FUNC 
    #define  BRD_SPI_PINS_FUNC_CLEAR  BRD_SPI2_D_PINS_FUNC_CLEAR    
  
    #define  BRD_SPI_CLOCK         (RST_CLK_PCLK_SSP2)  // All SSP and DMA - from Errata
    #define  BRD_SPI               MDR_SSP2
    
  #endif  


#endif


SSP_InitTypeDef _SSPInitStruct = 
{
  .SSP_SCR = 10,                         //  SSP_SCR - Фактор скорости передачи данных  
  .SSP_CPSDVSR = 100,                        //  SSP_CPSDVSR - Предделитель тактовой частоты
  .SSP_Mode = SSP_ModeMaster,             //  SSP_Mode
  .SSP_WordLength = SSP_WordLength8b,           //  SSP_WordLength - Длина кадра

  .SSP_SPH = SSP_SPH_1Edge,              //  SSP_SPH  - Фаза тактового сигнала
  .SSP_SPO = SSP_SPO_Low,                //  SSP_SPO  - Полярность тактовых импульсов
  .SSP_FRF = SSP_FRF_SPI_Motorola,       //  SSP_FRF  - Протокол передачи данных
  
  //.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE - Аппаратное управление передачей данных
  .SSP_HardwareFlowControl = SSP_HardwareFlowControl_LBM   // тестовый режим - выход на вход
};


SPI_Obj  BRD_SPIx = {

  //  Pins Init
  .PORTx          = BRD_SPI_PORT,            //  PORTx
  .Port_ClockMask = BRD_SPI_PORT_CLK,        //  Port_ClockMask 
  .Port_PinsSel   = BRD_SPI_PINS,            //  Port_PinsSel
  .Port_PinsFunc  = BRD_SPI_PINS_FUNC,       //  Port_PinsFunc
  .Port_PinsFunc_ClearMask = BRD_SPI_PINS_FUNC_CLEAR, //  Port_PinsFunc_ClearMask
  //  SPI Init
  .SPIx           = BRD_SPI,                 //  SPIx
  .SPI_ClockMask  = BRD_SPI_CLOCK,           //  SPI_ClockMask
  //  SPI Struct
  .pSSPInitStruct = &_SSPInitStruct
};

SPI_Obj* pBRD_SPIx = &BRD_SPIx;




#endif //BRD_SPI_SELECT_H
