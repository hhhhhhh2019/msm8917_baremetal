#include "gpio.h"
#include "utils.h"


void tlmm_cfg(u16 port, enum GPIOPull pull, enum GPIOFunc func,
              enum GPIODStr drv, enum GPIOStatus oe) {
    writeu32(GPIO_CTL(port), (pull << 0) | (func << 2) | (drv << 6) | (oe << 9));
}

void tlmm_mode(u16 port, enum GPIODir mode) {
    writeu32(GPIO_IO(port), mode);
}
