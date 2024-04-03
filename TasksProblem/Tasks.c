#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>

#include "Tasks.h"
#include "Sem.h"

#define PROCESSES_NUM   (int) 3

int main()
{
    uint8_t running = 1;

    // Just in case
    const char* const sem_name = "lock.sem";
    sem_unlink(sem_name);

    // Remember which process has been already started
    int started_processes[PROCESSES_NUM] = {0, 0, 0};
    sem_t * sema;
    //If semaphore with name does not exist, then create it with VALUE
    printf("\n[TASK_ONE] Open or Create a named semaphore, %s, its value is %d\n", sem_name, 0);
    sema = sem_open(sem_name, O_CREAT, 0666, 1);
    if (sema == SEM_FAILED)
    {
        perror("sem_open/Tasks");
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        // Unsigned, because the options are always positive values
        // Char, because the options go up to 7 - no need for a bigger number
        unsigned char selected_option = 0;
        int value = 0;
        sem_getvalue(sema, &value);
        printf("\n[TASK_ONE] Tasks sema value before - %d\n", value);
        sem_wait(sema);
        print_menu();
        // hhu - Specifying that the input will be unsigned char
        scanf("%hhu", &selected_option);
        sem_post(sema);
        sem_getvalue(sema, &value);
        printf("\n[TASK_ONE] Tasks sema value after - %d\n", value);

        execute_selected_option(selected_option, &running, started_processes);
        
    }

    clear_running_processes(started_processes, PROCESSES_NUM);

    sem_close(sema);
    sem_unlink(sem_name);
    return 0;
}

void print_menu()
{
    printf("\nMENU\n");
    printf("===============\n");
    printf("1. Run Task #2\n");
    printf("2. Run Task #3\n");
    printf("3. Run Task #4\n");
    printf("===============\n");
    printf("4. Stop Task #2\n");
    printf("5. Stop Task #3\n");
    printf("6. Stop Task #4\n");
    printf("======================\n");
    printf("7. Sleep for 3 seconds\n");
    printf("8. Exit\n");
    printf("======================\n");
    printf("Select:\n");
}

int8_t execute_selected_option(unsigned char selected_option, uint8_t* running, int* started_processes)
{
    uint8_t res = 0;
    uint8_t running_process = 0;
    pid_t pid;
    switch(selected_option) {
        // --- Start processes ---
        case 1:
            if (started_processes[TASK_2])
            {
                printf("\n[TASK_ONE] Task 2 already started");
                break;
            }
            
            running_process = check_running_processes(started_processes, PROCESSES_NUM);
            if (0 != running_process) 
            {
                printf("[TASK_ONE] Task [%d] is already running. Cannot start another", running_process);
                break;
            }

            pid = fork();
            if (pid < 0) 
            {
                // Fork failed
                fprintf(stderr, "Fork failed\n");
                return 1;
            } 
            else if (pid == 0) 
            {
                exec_task_2();
            } 
            else 
            {
                // The parent process obtains the pid of the child
                started_processes[TASK_2] = pid;
            }
            break;
        case 2:
            if (started_processes[TASK_3])
            {
                printf("\n[TASK_ONE] Task 3 already started");
                break;
            }

            running_process = check_running_processes(started_processes, PROCESSES_NUM);
            if (0 != running_process) 
            {
                printf("[TASK_ONE] Task [%d] is already running. Cannot start another", running_process);
                break;
            }

            pid = fork();
            if (pid < 0) 
            {
                // Fork failed
                fprintf(stderr, "Fork failed\n");
                return 1;
            } 
            else if (pid == 0) 
            {
                exec_task_3();
            } 
            else 
            {
                // The parent process obtains the pid of the child
                started_processes[TASK_3] = pid;
            }
            break;
        case 3:
            if (started_processes[TASK_4])
            {
                printf("Task 4 already started");
                break;
            }
            
            running_process = check_running_processes(started_processes, PROCESSES_NUM);
            if (0 != running_process) 
            {
                printf("[TASK_ONE] Process [%d] is already running. Cannot start another", running_process);
                break;
            }

            pid = fork();
            if (pid < 0) 
            {
                // Fork failed
                fprintf(stderr, "Fork failed\n");
                return 1;
            } 
            else if (pid == 0) 
            {
                exec_task_4();
            } 
            else 
            {
                // The parent process obtains the pid of the child
                started_processes[TASK_4] = pid;
            }
            break;
        // --- Kill processes ---
        case 4:
            printf("\n\nStop task 2\n\n");
            printf("\n[TASK_ONE] started_processes[TASK_2]: %d\n", started_processes[TASK_2]);
            if (started_processes[TASK_2] > 0)
            {
                printf("\n[TASK_ONE] Will kill task 2\n");
                kill(started_processes[TASK_2], SIGTERM);          

                // Stop
                started_processes[TASK_2] = 0;
            }
            else
            {
                // Not yet started
                printf("\n[TASK_ONE] Task 2 not yet started");
                break;
            }
            break;
        case 5:
            printf("\n\nStop task 3\n\n");
            printf("\n[TASK_ONE] started_processes[TASK_3]: %d\n", started_processes[TASK_3]);
            if (started_processes[TASK_3] > 0)
            {
                printf("\n[TASK_ONE] Will kill task 3\n");
                kill(started_processes[TASK_3], SIGTERM);          

                // Stop
                started_processes[TASK_3] = 0;
            }
            else
            {
                // Not yet started
                printf("\n[TASK_ONE] Task 3 not yet started");
                break;
            }
            break;
        case 6:
            printf("\n\nStop task 4\n\n");
            printf("\n[TASK_ONE] started_processes[TASK_4]: %d\n", started_processes[TASK_4]);
            if (started_processes[TASK_4] > 0)
            {
                printf("\n[TASK_ONE] Will kill task 4\n");
                kill(started_processes[TASK_4], SIGTERM);          

                // Stop
                started_processes[TASK_4] = 0;
            }
            else
            {
                // Not yet started
                printf("\n[TASK_ONE] Task 4 not yet started");
                break;
            }
            break;
            break;
        case 7:
            printf("\n[TASK_ONE] Sleeping for 3s\n");
            sleep(3);
            break;
        case 8:
            printf("\n[TASK_ONE] Exit\n");

            // Stops the main while loop
            *running = 0;
            break;
        default:
            printf("\n[TASK_ONE] Value is out of range.\n");
    }

    return res;
}


void exec_task_2() 
{
    // Change the program executed by the child process
    if (execl("./TaskTwo", "TaskTwo", "32", "47", "1000", (char *)NULL))
    {
        perror("execl");
        return;
    }
}

void exec_task_3() 
{
    // Change the program executed by the child process
    if (execl("./TaskThree", "TaskThree", "33", "44", "3000", (char *)NULL))
    {
        perror("execl");
        return;
    }
}

void exec_task_4() 
{
    // Change the program executed by the child process
    if (execl("./TaskFour", "TaskFour", "37", "42", "1000", (char *)NULL))
    {
        perror("execl");
        return;
    }
}

void clear_running_processes(int* started_processes, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (started_processes[i] > 0)
        {
            printf("\n[TASK_ONE] Will kill task %d\n", i+2);
            kill(started_processes[i], SIGTERM);  
        }
    }
}

uint8_t check_running_processes(int* started_processes, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (started_processes[i] > 0)
        {
            return i+2;
        }
    }

    return 0;
}