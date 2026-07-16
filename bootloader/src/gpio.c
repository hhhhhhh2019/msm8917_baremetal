#include "gpio.h"
#include "utils.h"


void tlmm_cfg(u16 port, enum GPIOPull pull, enum GPIOFunc func,
              enum GPIODStr drv, enum GPIODir oe) {
    writeu32(GPIO_CTL(port), (pull << 0) | (func << 2) | (drv << 6) | (oe << 9));
}

void tlmm_set_mode(u16 port, enum GPIOStatus mode) {
    writeu32(GPIO_IO(port), (mode & 1) << 1);
}

u32 tlmm_get_mode(u16 port) {
    return readu32(GPIO_IO(port)) & 1;
}
