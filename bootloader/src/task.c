#include "task.h"
#include "interrupts.h"

static struct registers tasks[128];
static u32 current_task;
static u32 tasks_count;

void init_scheduler() {
    current_task = 0;
    tasks_count = 1;
}

void schedule(struct registers* regs) {
    tasks[current_task] = *regs;
    current_task = (current_task+1) % tasks_count;
    *regs = tasks[current_task];
}

void add_task(void (*start)(void* userdata), void* userdata) {
    struct registers* regs = &tasks[tasks_count++];
    regs->x0 = (u64)userdata;
    regs->sp = 0xa0000000 + 0x8000 * (tasks_count-1);
    regs->x29 = regs->sp;
    regs->x30 = (u64)start;
    regs->elr = (u64)start;
    regs->spsr = 0x80000005;
}
