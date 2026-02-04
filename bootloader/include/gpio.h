#ifndef GPIO_H_
#define GPIO_H_

#include "stdint.h"

#define TLMM_BASE 0x1000000
#define GPIO_CTL(n) (TLMM_BASE + (n) * 0x1000 + 0)
#define GPIO_IO(n) (TLMM_BASE + (n) * 0x1000 + 4)

enum GPIOFunc {
    GPIO_FUNC_GPIO = 0,
};

enum GPIOPull {
    GPIO_NO_PULL   = 0,
    GPIO_PULL_DOWN = 1,
    GPIO_KEEPER    = 2,
    GPIO_PULL_UP   = 3,
};

enum GPIODStr {
    GPIO_2MA  = 0,
    GPIO_4MA  = 1,
    GPIO_6MA  = 2,
    GPIO_8MA  = 3,
    GPIO_10MA = 4,
    GPIO_12MA = 5,
    GPIO_14MA = 6,
    GPIO_16MA = 7,
};

enum GPIODir {
    GPIO_INPUT  = 0,
    GPIO_OUTPUT = 1,
};

enum GPIOStatus {
    GPIO_ENABLE  = 0,
    GPIO_DISABLE = 1,
};

void tlmm_cfg(u16 port, enum GPIOPull pull, enum GPIOFunc func,
              enum GPIODStr drv, enum GPIOStatus oe);
void tlmm_mode(u16 port, enum GPIODir mode);

#endif // GPIO_H_
