#ifndef _BRD_CLOCK_H
#define _BRD_CLOCK_H

#include <stdint.h>

void BRD_Clock_Init_HSE_PLL(uint32_t PLL_Mul);

#ifndef USE_MDR1986VE3
  void BRD_Clock_Init_HSE_dir(void);
#endif

#endif  //_BRD_CLOCK_H
