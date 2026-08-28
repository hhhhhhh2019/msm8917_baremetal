#include "devices/gpio.h"

void tlmm_cfg(u16 port, enum GPIOPull pull, enum GPIOFunc func,
              enum GPIODStr drv, enum GPIODir oe) {
    *GPIO_CTL(port) = (pull << 0) | (func << 2) | (drv << 6) | (oe << 9);
}

void tlmm_set_mode(u16 port, enum GPIOStatus mode) {
    *GPIO_IO(port) = (mode & 1) << 1;
}

u32 tlmm_get_mode(u16 port) {
    return *GPIO_IO(port) & 1;
}

void tlmm_int_cfg(u16 port, bool enable, enum GPIOIntMode active,
                  enum GPIODetectionControl det, bool keep, u8 cpuid) {
    *GPIO_INTR_STATUS(port) = 0;
    *GPIO_INTR_CFG(port) =
        (enable << 0) | (active << 1) | (det << 2) | (keep << 4) | (cpuid << 5);
}

u32 tlmm_get_status(u16 port) {
    return *GPIO_INTR_STATUS(port);
}

void tlmm_set_status(u16 port, u32 status) {
    *GPIO_INTR_STATUS(port) = status;
}
