#include "gic.h"
#include "spmi.h"
#include "log.h"
#include "utils.h"
#include "gpio.h"

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

void edl_reboot() {
    register u64 r0 __asm__("x0") = 2181039362;
    register u64 r1 __asm__("x1") = 2;
    register u64 r2 __asm__("x2") = 0x193D100;
    register u64 r3 __asm__("x3") = 1;
    register u64 r4 __asm__("x4") = 0;
    register u64 r5 __asm__("x5") = 0;
    register u64 r6 __asm__("x6") = 0;

    do {
        __asm__ volatile(
            __asmeq("%0", "x0")
            __asmeq("%1", "x1")
            __asmeq("%2", "x2")
            __asmeq("%3", "x3")
            __asmeq("%4", "x0")
            __asmeq("%5", "x1")
            __asmeq("%6", "x2")
            __asmeq("%7", "x3")
            __asmeq("%8", "x4")
            __asmeq("%9", "x5")
            __asmeq("%10", "x6")
            "smc    #0\n"
            : "=r"(r0), "=r"(r1), "=r"(r2), "=r"(r3)
            : "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5), "r"(r6));
    } while (r0 == 1);


    /* log(LOG_INFO, "edl_reboot(): smc done"); */

    pmic_reg_write(0, 8, 87, 0);

    /* log(LOG_INFO, "edl_reboot(): PMIC_WD_RESET_S2_CTL2 done"); */

    pmic_reg_write(0, 8, 91, 0);
    pmic_reg_write(2, 8, 91, 0);

    // TODO: better delay
    for (volatile int i = 0; i < 1000000; i++);

    pmic_reg_write(0, 8, 90, 1);
    pmic_reg_write(2, 8, 90, 1);

    pmic_reg_write(0, 8, 91, 1 << 7);
    pmic_reg_write(2, 8, 91, 1 << 7);

    /* log(LOG_INFO, "edl_reboot(): pmic_reset_configure done"); */

    writeu32(0x004AB000, 0);

    // судя по логам, выполнение доходит даже до сюда

    /* log(LOG_INFO, "edl_reboot(): MPM2_MPM_PS_HOLD done"); */
}

#define APPS_SS_BASE                       0x0B000000
#define APPS_SS_END                        0x0B200000

#define MSM_GIC_DIST_BASE                  APPS_SS_BASE
#define MSM_GIC_CPU_BASE                   (APPS_SS_BASE + 0x2000)
#define APPS_APCS_QTMR_AC_BASE             (APPS_SS_BASE + 0x00020000)
#define APPS_APCS_F0_QTMR_V1_BASE          (APPS_SS_BASE + 0x00021000)
#define QTMR_BASE                          APPS_APCS_F0_QTMR_V1_BASE
#define APCS_ALIAS1_IPC_INTERRUPT_1        (APPS_SS_BASE + 0x00011008)
#define APCS_ALIAS0_IPC_INTERRUPT_2        (APPS_SS_BASE + 0x00111008)
#define APCS_ALIAS0_IPC_INTERRUPT          platform_get_apcs_ipc_base()

#define GIC_DIST_REG(off)           (MSM_GIC_DIST_BASE + (off))

#define GIC_DIST_CTRL               GIC_DIST_REG(0x000)
#define GIC_DIST_CTR                GIC_DIST_REG(0x004)
#define GIC_DIST_ENABLE_SET         GIC_DIST_REG(0x100)
#define GIC_DIST_ENABLE_CLEAR       GIC_DIST_REG(0x180)
#define GIC_DIST_PENDING_SET        GIC_DIST_REG(0x200)
#define GIC_DIST_PENDING_CLEAR      GIC_DIST_REG(0x280)
#define GIC_DIST_ACTIVE_BIT         GIC_DIST_REG(0x300)
#define GIC_DIST_PRI                GIC_DIST_REG(0x400)
#define GIC_DIST_TARGET             GIC_DIST_REG(0x800)
#define GIC_DIST_CONFIG             GIC_DIST_REG(0xc00)
#define GIC_DIST_SOFTINT            GIC_DIST_REG(0xf00)

#define GIC_CPU_REG(off)            (MSM_GIC_CPU_BASE  + (off))

#define GIC_CPU_CTRL                GIC_CPU_REG(0x00)
#define GIC_CPU_PRIMASK             GIC_CPU_REG(0x04)
#define GIC_CPU_BINPOINT            GIC_CPU_REG(0x08)
#define GIC_CPU_INTACK              GIC_CPU_REG(0x0c)
#define GIC_CPU_EOI                 GIC_CPU_REG(0x10)
#define GIC_CPU_RUNNINGPRI          GIC_CPU_REG(0x14)
#define GIC_CPU_HIGHPRI             GIC_CPU_REG(0x18)


void print_cpu_info() {
    u64 midr;
    asm("mrs %0, MIDR_EL1" : "=r"(midr));
    logf(LOG_INFO, "MIDR Revision: %x", bits(midr, 3, 0));
    logf(LOG_INFO, "MIDR PartNum: %x", bits(midr, 15, 4));
    logf(LOG_INFO, "MIDR Architecture: %x", bits(midr, 19, 16));
    logf(LOG_INFO, "MIDR Variant: %x", bits(midr, 23, 20));
    logf(LOG_INFO, "MIDR Implementer: %x", bits(midr, 31, 24));

    u32 iidr = readu32(GICC_IIDR);
    logf(LOG_INFO, "IIDR Implementer: %x", bits(iidr, 11, 0));
    logf(LOG_INFO, "IIDR Revision: %x", bits(iidr, 15, 12));
    logf(LOG_INFO, "IIDR Architecture version: %x", bits(iidr, 19, 16));
    logf(LOG_INFO, "IIDR ProductID: %x", bits(iidr, 31, 20));

    u32 revidr;
    asm("mrs %0, REVIDR_EL1" : "=r"(revidr));
    logf(LOG_INFO, "REVIDR_EL1: %d", revidr);

    u64 feats0;
    asm("mrs %0, ID_AA64PFR0_EL1" : "=r"(feats0));
    logf(LOG_INFO, "EL0 handling: %d", bits(feats0, 3, 0));
    logf(LOG_INFO, "EL1 handling: %d", bits(feats0, 7, 4));
    logf(LOG_INFO, "EL2 handling: %d", bits(feats0, 11, 8));
    logf(LOG_INFO, "EL3 handling: %d", bits(feats0, 15, 12));
    logf(LOG_INFO, "FP: %d", bits(feats0, 19, 16));
    logf(LOG_INFO, "AdvSIMD: %d", bits(feats0, 23, 20));
    logf(LOG_INFO, "GIC: %d", bits(feats0, 27, 24));

    u64 cbar;
    asm("mrs %0, S3_1_C15_C3_0" : "=r"(cbar));
    logf(LOG_INFO, "PERIPHBASE: %x", cbar);
}


void main() {
    clear_debug_buffer();

    log(LOG_INFO, "hello from main");

    print_cpu_info();

    u64 current_el;
    asm("mrs %0, CurrentEL" : "=r"(current_el));
    logf(LOG_INFO, "current EL: %z", current_el >> 2);

    /* logf(LOG_INFO, "gic ctl: %x", readu32(GIC_DIST_CTRL)); */

    /* writeu32(GIC_DIST_CTRL, 0); */

    /* u32 cpumask = 0; */

    /* for (u32 i = 0; i < 32; i += 4) { */
    /*     cpumask = readu32(GIC_DIST_TARGET + i); */
    /*     cpumask |= cpumask >> 16; */
    /*     cpumask |= cpumask >> 8; */
    /*     if (cpumask) */
    /*         break; */
    /* } */

    /* cpumask |= cpumask << 8; */
    /* cpumask |= cpumask << 16; */

    /* logf(LOG_INFO, "cpumask: %x", cpumask); */

    /* u32 num_irq = readu32(GIC_DIST_CTR) & 0x1f; */
    /* logf(LOG_INFO, "num_irq: %d", num_irq); */

    edl_reboot();

    while (1);
}
