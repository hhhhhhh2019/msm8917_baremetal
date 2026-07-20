#ifndef GIC_H_
#define GIC_H_

// https://developer.arm.com/documentation/ihi0048/latest/

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

#define GICD_CTLR           (GICD_BASE + 0x000)
#define GICD_TYPER          (GICD_BASE + 0x004)
#define GICD_IIDR           (GICD_BASE + 0x008)
#define GICD_GROUPR(n)      (GICD_BASE + 0x080 + (n) * 4)
#define GICD_ENABLE_SET(n)  (GICD_BASE + 0x100 + (n) * 4)
#define GICD_ENABLE_CLR(n)  (GICD_BASE + 0x180 + (n) * 4)
#define GICD_PENDING_SET(n) (GICD_BASE + 0x200 + (n) * 4)
#define GICD_PENDING_CLR(n) (GICD_BASE + 0x280 + (n) * 4)
#define GICD_ACTIVE_SET(n)  (GICD_BASE + 0x300 + (n) * 4)
#define GICD_ACTIVE_CLR(n)  (GICD_BASE + 0x380 + (n) * 4)
#define GICD_PRIORITY(n)    (GICD_BASE + 0x400 + (n) * 4)
#define GICD_TARGET(n)      (GICD_BASE + 0x800 + (n) * 4)
#define GICD_CONFIG(n)      (GICD_BASE + 0xc00 + (n) * 4)
#define GICD_SOFTINT(n)     (GICD_BASE + 0xf00 + (n) * 4)

/* #define GIC_PPI_START 16 */
/* #define GIC_SPI_START 32 */

void gic_init();

#endif // GIC_H_
