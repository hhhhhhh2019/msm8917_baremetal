#include "spmi.h"
#include "log.h"
#include "utils.h"

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

#define GCC_BASE 0x1800000

#define TLMM_BASE 0x1000000
#define GPIO_CTL(n) (TLMM_BASE + (n) * 0x1000 + 0)
#define GPIO_IO(n) (TLMM_BASE + (n) * 0x1000 + 4)

// lk2nd
/* GPIO TLMM: Direction */
#define GPIO_INPUT      0
#define GPIO_OUTPUT     1

/* GPIO TLMM: Pullup/Pulldown */
#define GPIO_NO_PULL    0
#define GPIO_PULL_DOWN  1
#define GPIO_KEEPER     2
#define GPIO_PULL_UP    3

/* GPIO TLMM: Drive Strength */
#define GPIO_2MA        0
#define GPIO_4MA        1
#define GPIO_6MA        2
#define GPIO_8MA        3
#define GPIO_10MA       4
#define GPIO_12MA       5
#define GPIO_14MA       6
#define GPIO_16MA       7

/* GPIO TLMM: Status */
#define GPIO_ENABLE     0
#define GPIO_DISABLE    1

/* GPIO_IN_OUT register shifts. */
#define GPIO_IN         BIT(0)
#define GPIO_OUT        BIT(1)


#define CLK_CTL_BASE                       0x1800000
#define GPLL0_STATUS                       (CLK_CTL_BASE + 0x2101C)
#define GPLL2_STATUS                       (CLK_CTL_BASE + 0x4A01C)
#define GPLL0_MODE                         (CLK_CTL_BASE + 0x21000)
#define APCS_GPLL_ENA_VOTE                 (CLK_CTL_BASE + 0x45000)
#define APCS_CLOCK_BRANCH_ENA_VOTE         (CLK_CTL_BASE + 0x45004)
#define GPLL4_MODE                         (CLK_CTL_BASE + 0x24000)
#define GPLL4_STATUS                       (CLK_CTL_BASE + 0x24024)
#define GPLL6_STATUS                       (CLK_CTL_BASE + 0x3701C)
// ------------

void main() {
    clear_debug_buffer();

    log(LOG_INFO, "hello from main");

    u64 current_el;
    asm("mrs %0, CurrentEL" : "=r"(current_el));
    logf(LOG_INFO, "current EL: %z", current_el >> 2);

    logf(LOG_INFO, "GPLL0_STATUS: %x", readu32(GPLL0_STATUS));
    logf(LOG_INFO, "GPLL2_STATUS: %x", readu32(GPLL2_STATUS));
    logf(LOG_INFO, "GPLL0_MODE: %x", readu32(GPLL0_MODE));
    logf(LOG_INFO, "APCS_GPLL_ENA_VOTE: %x", readu32(APCS_GPLL_ENA_VOTE));
    logf(LOG_INFO, "APCS_CLOCK_BRANCH_ENA_VOTE: %x", readu32(APCS_CLOCK_BRANCH_ENA_VOTE));
    logf(LOG_INFO, "GPLL4_MODE: %x", readu32(GPLL4_MODE));
    logf(LOG_INFO, "GPLL4_STATUS: %x", readu32(GPLL4_STATUS));
    logf(LOG_INFO, "GPLL6_STATUS: %x", readu32(GPLL6_STATUS));

    edl_reboot();

    while (1);
}
