#ifndef GIC_H_
#define GIC_H_

// https://developer.arm.com/documentation/ddi0500/j/Generic-Interrupt-Controller-CPU-Interface/GIC-programmers-model/CPU-interface-register-summary?lang=en

#define GICC_BASE   0xb000000

#define GICC_CTLR   (GICC_BASE + 0x0000)
#define GICC_PMR    (GICC_BASE + 0x0004)
#define GICC_BPR    (GICC_BASE + 0x0008)
#define GICC_IAR    (GICC_BASE + 0x000C)
#define GICC_EOIR   (GICC_BASE + 0x0010)
#define GICC_RPR    (GICC_BASE + 0x0014)
#define GICC_HPPIR  (GICC_BASE + 0x0018)
#define GICC_ABPR   (GICC_BASE + 0x001C)
#define GICC_AIAR   (GICC_BASE + 0x0020)
#define GICC_AEOIR  (GICC_BASE + 0x0024)
#define GICC_AHPPIR (GICC_BASE + 0x0028)
#define GICC_APR0   (GICC_BASE + 0x00D0)
#define GICC_NSAPR0 (GICC_BASE + 0x00E0)
#define GICC_IIDR   (GICC_BASE + 0x00FC)
#define GICC_DIR    (GICC_BASE + 0x1000)

#endif // GIC_H_
