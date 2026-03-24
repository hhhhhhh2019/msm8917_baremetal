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

void add_task(void (*start)(void* data)) {

}
