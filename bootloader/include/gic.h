#ifndef GIC_H_
#define GIC_H_

// https://developer.arm.com/documentation/ddi0500/j/Generic-Interrupt-Controller-CPU-Interface/GIC-programmers-model/CPU-interface-register-summary?lang=en

#define GICD_BASE 0xb000000
#define GICC_BASE 0xb002000

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

#define GICD_CTLR        (GICD_BASE + 0x000)
#define GICD_CTR         (GICD_BASE + 0x004)
#define GICD_ENABLE_SET  (GICD_BASE + 0x100)
#define GICD_ENABLE_CLR  (GICD_BASE + 0x180)
#define GICD_PENDING_SET (GICD_BASE + 0x200)
#define GICD_PENDING_CLR (GICD_BASE + 0x280)
#define GICD_ACTIVE      (GICD_BASE + 0x300)
#define GICD_PRIORITY    (GICD_BASE + 0x400)
#define GICD_TARGET      (GICD_BASE + 0x800)
#define GICD_CONFIG      (GICD_BASE + 0xc00)
#define GICD_SOFTINT     (GICD_BASE + 0xf00)

/* #define GIC_PPI_START 16 */
/* #define GIC_SPI_START 32 */

void gic_init();

#endif // GIC_H_
