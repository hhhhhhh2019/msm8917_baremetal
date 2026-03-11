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
#define GIC_DIST_CTRL               (GICC_BASE + 0x000)
#define GIC_DIST_CTR                (GICC_BASE + 0x004)
#define GIC_DIST_ENABLE_SET         (GICC_BASE + 0x100)
#define GIC_DIST_ENABLE_CLEAR       (GICC_BASE + 0x180)
#define GIC_DIST_PENDING_SET        (GICC_BASE + 0x200)
#define GIC_DIST_PENDING_CLEAR      (GICC_BASE + 0x280)
#define GIC_DIST_ACTIVE_BIT         (GICC_BASE + 0x300)
#define GIC_DIST_PRI                (GICC_BASE + 0x400)
#define GIC_DIST_TARGET             (GICC_BASE + 0x800)
#define GIC_DIST_CONFIG             (GICC_BASE + 0xc00)
#define GIC_DIST_SOFTINT            (GICC_BASE + 0xf00)

#define MSM_GIC_CPU_BASE            0xb002000
#define GIC_CPU_CTRL                (MSM_GIC_CPU_BASE + 0x00)
#define GIC_CPU_PRIMASK             (MSM_GIC_CPU_BASE + 0x04)
#define GIC_CPU_BINPOINT            (MSM_GIC_CPU_BASE + 0x08)
#define GIC_CPU_INTACK              (MSM_GIC_CPU_BASE + 0x0c)
#define GIC_CPU_EOI                 (MSM_GIC_CPU_BASE + 0x10)
#define GIC_CPU_RUNNINGPRI          (MSM_GIC_CPU_BASE + 0x14)
#define GIC_CPU_HIGHPRI             (MSM_GIC_CPU_BASE + 0x18)

#define GIC_PPI_START 16
#define GIC_SPI_START 32

#endif // GIC_H_
