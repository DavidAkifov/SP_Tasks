#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>

enum process
{
    TASK_2,
    TASK_3,
    TASK_4
};

void print_menu();
int8_t execute_selected_option(unsigned char selected_option, uint8_t* running, int* started_processes);
void exec_task_2();
void exec_task_3();
void exec_task_4();
void clear_running_processes(int* started_processes, int size);
uint8_t check_running_processes(int* started_processes, int size);

#endif // TASKS_H