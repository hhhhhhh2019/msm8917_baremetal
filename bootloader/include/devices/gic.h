#ifndef _GIC_H
#define _GIC_H

// https://developer.arm.com/documentation/ihi0048/latest/

#define GICD_BASE 0xb000000ULL
#define GICC_BASE 0xb002000ULL

#define GICC_CTLR   ((u32*)(GICC_BASE + 0x0000))
#define GICC_PMR    ((u32*)(GICC_BASE + 0x0004))
#define GICC_BPR    ((u32*)(GICC_BASE + 0x0008))
#define GICC_IAR    ((u32*)(GICC_BASE + 0x000C))
#define GICC_EOIR   ((u32*)(GICC_BASE + 0x0010))
#define GICC_RPR    ((u32*)(GICC_BASE + 0x0014))
#define GICC_HPPIR  ((u32*)(GICC_BASE + 0x0018))
#define GICC_ABPR   ((u32*)(GICC_BASE + 0x001C))
#define GICC_AIAR   ((u32*)(GICC_BASE + 0x0020))
#define GICC_AEOIR  ((u32*)(GICC_BASE + 0x0024))
#define GICC_AHPPIR ((u32*)(GICC_BASE + 0x0028))
#define GICC_APR0   ((u32*)(GICC_BASE + 0x00D0))
#define GICC_NSAPR0 ((u32*)(GICC_BASE + 0x00E0))
#define GICC_IIDR   ((u32*)(GICC_BASE + 0x00FC))
#define GICC_DIR    ((u32*)(GICC_BASE + 0x1000))

#define GICD_CTLR           ((u32*)(GICD_BASE + 0x000))
#define GICD_TYPER          ((u32*)(GICD_BASE + 0x004))
#define GICD_IIDR           ((u32*)(GICD_BASE + 0x008))
#define GICD_GROUPR(n)      ((u32*)(GICD_BASE + 0x080 + (n) * 4))
#define GICD_ENABLE_SET(n)  ((u32*)(GICD_BASE + 0x100 + (n) * 4))
#define GICD_ENABLE_CLR(n)  ((u32*)(GICD_BASE + 0x180 + (n) * 4))
#define GICD_PENDING_SET(n) ((u32*)(GICD_BASE + 0x200 + (n) * 4))
#define GICD_PENDING_CLR(n) ((u32*)(GICD_BASE + 0x280 + (n) * 4))
#define GICD_ACTIVE_SET(n)  ((u32*)(GICD_BASE + 0x300 + (n) * 4))
#define GICD_ACTIVE_CLR(n)  ((u32*)(GICD_BASE + 0x380 + (n) * 4))
#define GICD_PRIORITY(n)    ((u32*)(GICD_BASE + 0x400 + (n) * 4))
#define GICD_TARGET(n)      ((u32*)(GICD_BASE + 0x800 + (n) * 4))
#define GICD_CONFIG(n)      ((u32*)(GICD_BASE + 0xc00 + (n) * 4))
#define GICD_SOFTINT(n)     ((u32*)(GICD_BASE + 0xf00 + (n) * 4))

/* #define GIC_PPI_START 16 */
/* #define GIC_SPI_START 32 */

void gic_init();
void gic_unmask_interrupt(u32 num);
void gic_mask_interrupt(u32 num);

#endif // _GIC_H
