#ifndef QTIMER_H_
#define QTIMER_H_

#include "stdint.h"

#define QTIMER_FREQ 0x124f800

#define QTMR_BASE 0xb021000

#define QTMR_V1_CNTPCT_LO    (0x00000000 + QTMR_BASE)
#define QTMR_V1_CNTPCT_HI    (0x00000004 + QTMR_BASE)
#define QTMR_V1_CNTFRQ       (0x00000010 + QTMR_BASE)
#define QTMR_V1_CNTP_CVAL_LO (0x00000020 + QTMR_BASE)
#define QTMR_V1_CNTP_CVAL_HI (0x00000024 + QTMR_BASE)
#define QTMR_V1_CNTP_TVAL    (0x00000028 + QTMR_BASE)
#define QTMR_V1_CNTP_CTL     (0x0000002C + QTMR_BASE)

#define QTMR_TIMER_CTRL_ENABLE   (1 << 0)
#define QTMR_TIMER_CTRL_INT_MASK (1 << 1)

void qtimer_enable();
void qtimer_disable();
void start_timer(u64 ms);

#endif // QTIMER_H_
