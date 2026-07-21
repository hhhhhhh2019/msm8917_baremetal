#ifndef GPIO_H_
#define GPIO_H_

#include "stdint.h"

#define TLMM_BASE 0x1000000
#define GPIO_CTL(n) (TLMM_BASE + (n) * 0x1000 + 0)
#define GPIO_IO(n) (TLMM_BASE + (n) * 0x1000 + 4)
#define GPIO_INTR_CFG(n) (TLMM_BASE + (n) * 0x1000 + 8)
#define GPIO_INTR_STATUS(n) (TLMM_BASE + (n) * 0x1000 + 12)

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
    GPIO_LOW  = 0,
    GPIO_HIGH = 1,
};

enum GPIOIntMode {
    GPIO_ACTIVE_LOW  = 0,
    GPIO_ACTIVE_HIGH = 1,
};

enum GPIODetectionControl {
    GPIO_DETECT_LEVEL = 0, // пока +
    GPIO_DETECT_PEDGE = 1, // - -> +
    GPIO_DETECT_NEDGE = 2, // + -> -
    GPIO_DETECT_DEDGE = 3, // dual edge
};

void tlmm_cfg(u16 port, enum GPIOPull pull, enum GPIOFunc func,
              enum GPIODStr drv, enum GPIODir oe);
void tlmm_set_mode(u16 port, enum GPIOStatus mode);
u32 tlmm_get_mode(u16 port);
void tlmm_int_cfg(u16 port, bool enable, enum GPIOIntMode active,
                  enum GPIODetectionControl det, bool keep, u8 cpuid);
u32 tlmm_get_status(u16 port);
void tlmm_set_status(u16 port, u32 status);

#endif // GPIO_H_
