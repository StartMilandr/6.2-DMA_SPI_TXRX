#ifndef _BRD_UTILS_H
#define _BRD_UTILS_H

#include <stdint.h>

#define HWREG(x) (*((volatile uint32_t *)(x)))

void Delay(uint32_t ticks);

#endif //_BRD_UTILS_H
