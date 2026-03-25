#ifndef TASK_H_
#define TASK_H_

#include "interrupts.h"

void init_scheduler();
void schedule(struct registers* regs);
void add_task(void (*start)(void* userdata), void* userdata);

#endif // TASK_H_
