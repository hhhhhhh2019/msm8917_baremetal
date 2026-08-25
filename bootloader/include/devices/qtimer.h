#ifndef QTIMER_H_
#define QTIMER_H_

#define QTIMER_FREQ 0x124f800

#define QTMR_BASE 0xb021000ULL

#define QTMR_V1_CNTPCT_LO    ((u32*)(QTMR_BASE + 0x00000000))
#define QTMR_V1_CNTPCT_HI    ((u32*)(QTMR_BASE + 0x00000004))
#define QTMR_V1_CNTFRQ       ((u32*)(QTMR_BASE + 0x00000010))
#define QTMR_V1_CNTP_CVAL_LO ((u32*)(QTMR_BASE + 0x00000020))
#define QTMR_V1_CNTP_CVAL_HI ((u32*)(QTMR_BASE + 0x00000024))
#define QTMR_V1_CNTP_TVAL    ((u32*)(QTMR_BASE + 0x00000028))
#define QTMR_V1_CNTP_CTL     ((u32*)(QTMR_BASE + 0x0000002C))

#define QTMR_TIMER_CTRL_ENABLE   (1 << 0)
#define QTMR_TIMER_CTRL_INT_MASK (1 << 1)

#define QTIMER_IRQ 289

void qtimer_enable();
void qtimer_disable();
void start_timer(u64 ms);

#endif // QTIMER_H_
