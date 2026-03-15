#include "qtimer.h"
#include "utils.h"

void qtimer_enable() {
    u32 ctrl = readu32(QTMR_V1_CNTP_CTL);

    ctrl |= QTMR_TIMER_CTRL_ENABLE;
    ctrl &= ~QTMR_TIMER_CTRL_INT_MASK;

    writeu32(QTMR_V1_CNTP_CTL, ctrl);

    asm("isb");
}

void qtimer_disable() {
    u32 ctrl = readu32(QTMR_V1_CNTP_CTL);

    ctrl &= ~QTMR_TIMER_CTRL_ENABLE;
    ctrl |= QTMR_TIMER_CTRL_INT_MASK;

    writeu32(QTMR_V1_CNTP_CTL, ctrl);

    asm("isb");
}

void start_timer(u64 ms) {
    qtimer_disable();
    writeu32(QTMR_V1_CNTP_TVAL, QTIMER_FREQ * ms / 1000);
    qtimer_enable();
}
