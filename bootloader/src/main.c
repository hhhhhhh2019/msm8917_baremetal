#include "spmi.h"
#include "utils.h"

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

void edl_reboot() {
    /* scm_arg.x0 = MAKE_SIP_SCM_CMD(SCM_SVC_IO, SCM_IO_WRITE); */
    /* scm_arg.x1 = 2; */
    /* scm_arg.x2 = TCSR_BOOT_MISC_DETECT; */
    /* scm_arg.x3 = SCM_EDLOAD_MODE; */
    /* scm_arg.atomic = true; */
    /* ret = scm_call2(&scm_arg, &scm_ret); */

    /* uint32_t *indir_arg = NULL; */
    /* uint32_t x5; */
    /* int i; */
    /* uint32_t rc; */

    /* arg->x0 |= SCM_ATOMIC_BIT; */
    /* x5 = 0; */

    /* rc = scm_call_a32(arg->x0, arg->x1, arg->x2, arg->x3, arg->x4, x5, ret); */

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



    pmic_reg_write(0, 8, 87, 0);

    pmic_reg_write(0, 8, 91, 0);
    pmic_reg_write(2, 8, 91, 0);

    // TODO: better delay
    for (volatile int i = 0; i < 1000000; i++);

    pmic_reg_write(0, 8, 90, 1);
    pmic_reg_write(2, 8, 90, 1);

    pmic_reg_write(0, 8, 91, 1 << 7);
    pmic_reg_write(2, 8, 91, 1 << 7);

    writeu32(0x004AB000, 0);
}

void main() {
    writeu32(0x80000000, 0x123456);

    edl_reboot();

    while (1);
}
