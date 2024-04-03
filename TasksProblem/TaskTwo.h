#ifndef TASK_TWO_H
#define TASK_TWO_H

#include <stdint.h>

uint8_t running = 1;
pid_t pid;

void sigterm_handler();
void exec_task_5();

#endif // TASK_TWO_H