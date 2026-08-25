#include "devices/qtimer.h"

void qtimer_enable() {
    u32 ctrl = *QTMR_V1_CNTP_CTL;

    ctrl |= QTMR_TIMER_CTRL_ENABLE;
    ctrl &= ~QTMR_TIMER_CTRL_INT_MASK;

    *QTMR_V1_CNTP_CTL = ctrl;
}

void qtimer_disable() {
    u32 ctrl = *QTMR_V1_CNTP_CTL;

    ctrl &= ~QTMR_TIMER_CTRL_ENABLE;
    ctrl |= QTMR_TIMER_CTRL_INT_MASK;

    *QTMR_V1_CNTP_CTL = ctrl;
}

void start_timer(u64 ms) {
    qtimer_disable();
    asm volatile("dsb sy" ::
                     : "memory");
    *QTMR_V1_CNTP_TVAL = QTIMER_FREQ;
    asm volatile("dsb sy" ::
                     : "memory");
    qtimer_enable();
}
