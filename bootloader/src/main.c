#include "spmi.h"
#include "log.h"
#include "stdint.h"
#include "task.h"
#include "utils.h"
#include "gpio.h"
#include "qtimer.h"
#include "interrupts.h"
#include "gic.h"
#include "fb.h"

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

void edl_reboot() {
    asm volatile("msr daifset, #15" ::: "memory");

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

    /* // TODO: better delay */
    /* for (volatile int i = 0; i < 2000000; i++); */

    pmic_reg_write(0, 8, 90, 1);
    pmic_reg_write(2, 8, 90, 1);

    /* pmic_reg_write(0, 8, 91, 1 << 7); */
    /* pmic_reg_write(2, 8, 91, 1 << 7); */

    // наделал какую-то дичь, но теперь оно стабильно перезагружается
    __asm__ volatile ("isb");
    __asm__ volatile ("dsb sy");

    writeu32(0x193d100, 1);
    writeu32(0x004AB000, 0);

    writeu32(0x004AB000, 0);

    __asm__ volatile ("isb");
    __asm__ volatile ("dsb sy");

    while (1) { asm volatile("wfi"); };
}

void fb_put_hex(u64 num, u32 chars);

void timer_handler(u32 irq, struct registers *regs) {
    fb_put_char('t');
    start_timer(1000);
}

void spmi_handler(u32 irq, struct registers *regs) {
    fb_put_char('i');

    u32 irq_sts = readu32(PMIC_ARB_IRQ_STATUS(0x2f));

    fb_put_hex(irq_sts, 9);

    /* pmic_arb_write(0, 0x815, 1, (u8[]){ 0x03 }); */
    /* asm volatile("dsb sy\nisb" ::: "memory"); */

    writeu32(PMIC_ARB_IRQ_CLEAR(0x2f), 1);

    asm volatile("dsb sy\nisb" ::: "memory");
}

void main() {
    fb_init();
    fb_init_addres((void*)0x90001000);

    set_vector_table(&vector_table);
    gic_init();

    /* gic_unmask_interrupt(240); */
    /* set_irq_handler(240, &tlmm_handler); */

    gic_unmask_interrupt(SPMI_IRQ);
    set_irq_handler(SPMI_IRQ, &spmi_handler);

    gic_unmask_interrupt(QTMR_IRQ);
    set_irq_handler(QTMR_IRQ, &timer_handler);

    tlmm_cfg(93, GPIO_NO_PULL, GPIO_FUNC_GPIO, GPIO_2MA, GPIO_OUTPUT);
    tlmm_set_mode(93, GPIO_LOW);

    pmic_arb_init();

    /* // 1. Устанавливаем срабатывание по фронту (Edge) для обеих кнопок */
    /* // (бит 0 = PWR, бит 1 = Volume Down) */
    /* pmic_arb_write(0, 0x811, 1, (u8[]){ 0x00 }); */
    /* // 2. Настраиваем полярность: например, 0x03 для сработки при отпускании */
    /* // или 0x00 при нажатии (так как active low) */
    /* pmic_arb_write(0, 0x812, 1, (u8[]){ 0x03 }); */
    /* // 3. Сбрасываем старые зависшие прерывания (пишем 1 в сбрасываемые биты) */
    /* pmic_arb_write(0, 0x815, 1, (u8[]){ 0x03 }); */
    /* // 4. Включаем прерывания для PWR и Volume Down (бит 0 и бит 1) */
    /* pmic_arb_write(0, 0x813, 1, (u8[]){ 0x03 }); */
    /* writeu32(PMIC_ARB_IRQ_ENABLE(0x2f), 1); */

    pmic_arb_write(0, 0x811, 1, (u8[]){ 0x1 });
    pmic_arb_write(0, 0x812, 1, (u8[]){ 0x1 });
    pmic_arb_write(0, 0x813, 1, (u8[]){ 0x1 });
    pmic_arb_write(0, 0x815, 1, (u8[]){ 0x1 });
    writeu32(PMIC_ARB_IRQ_ENABLE(0x2f), 1);
    /* writeu32(PMIC_ARB_IRQ_CLEAR(0x2f), 1); */

    asm volatile("msr daifclr, #15" ::: "memory");

    fb_put_char('\n');

    start_timer(1000);

    while (1) { asm volatile("wfi"); }

    /* while (1) { */
    /*     u32 status = 0; */
    /*     pmic_arb_read(0, 0x810, 1, (u8*)&status); */

    /*     tlmm_set_mode(93, status & 2 ? GPIO_HIGH : GPIO_LOW); // & 1 for power key */
    /*     for (volatile u32 j = 0; j < 10000; j++); */
    /* } */
}
