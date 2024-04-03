#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <errno.h>

#include "TaskTwo.h"
#include "Input.h"

#define THOUSAND_MILLISECONDS   1000

int main(int argc, char *argv[])
{
    int text_color = atoi(argv[1]);
    int back_color = atoi(argv[2]);
    unsigned int num = atoi(argv[3]);
    float input = 0;

    // SEMAPHORE --------------------------------------
    const char* const sem_name = "lock.sem";
    sem_t * sema;
    sema = sem_open(sem_name, 0);
    if (sema == SEM_FAILED)
    {
        perror("[TASK_THREE] sem_open/TaskThree");
        exit(EXIT_FAILURE);
    }
    // ------------------------------------------------

    // Install signal handler for SIGTERM
    signal(SIGTERM, sigterm_handler);
    printf("\n\x1b[%d;%dm[TASK_THREE] Task 3 has started!\x1b[0m\n", text_color, back_color);

    // NAMED_PIPE ---------------------------------------------------------
    char* myfifo = "/tmp/myfifo";
    struct stat statBuffer;
    // Check if the FIFO exists
    if (stat(myfifo, &statBuffer) == 0) 
    {
        printf("[TASK_THREE] FIFO exists.\n");
    } 
    else 
    {
        if ((mkfifo(myfifo, S_IRWXU)) != 0) 
        {
            printf("Unable to create a fifo; errno=%d\n", errno);
            exit(EXIT_FAILURE);                     /* Print error message and return */
        }
    }
    // ---------------------------------------------------------------------

    // Start task 5
    pid = fork();
    if (pid < 0) 
    {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    } 
    else if (pid == 0) 
    {
        // Child process
        printf("\n[TASK_THREE] This is the child process (PID: %d)\n", getpid());
        exec_task_5();
    } 

    // Open the named pipe for writing
    int fd;
    // FIFO file path
    fd = open(myfifo, O_WRONLY);
    if (fd == -1) 
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        sem_wait(sema);
        printf("\n[TASK_THREE] Task 3 int input: ");
        scanf("%f", &input);
        printf("\n\x1b[%d;%dm[TASK_THREE] Number entered:%f!\x1b[0m\n", text_color, back_color, input);
        sem_post(sema);

        // Send the number to Task 5 
        SerializedData serializedData;
        // Serialize and write integer data to the named pipe
        serializedData.type = FLOAT;
        serializedData.data.floatData = input;

        // Use pipes
        write(fd, &serializedData, sizeof(SerializedData));

        usleep(num * THOUSAND_MILLISECONDS);
    }
    
    // Close the named pipe
    close(fd);
    sem_close(sema);
    printf("\n[TASK_THREE] Shutting down taks 3\n");
    exit(EXIT_SUCCESS);
}

void sigterm_handler() 
{
    printf("\n[TASK_THREE] Received SIGTERM signal. Terminating child process gracefully...\n");
    printf("\n[TASK_THREE] Will kill task 5\n");
    kill(pid, SIGTERM);  
    running = 0;
}

void exec_task_5() 
{
    // Change the program executed by the child process
    if (execl("./TaskFive", "TaskFive", "filename.txt", (char *)NULL))
    {
        perror("execl");
        return;
    }
}