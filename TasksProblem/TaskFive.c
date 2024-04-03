#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "TaskFive.h"
#include "Input.h"

int main(int argc, char *argv[])
{
    // Open file for appending (creates the file if it doesn't exist)
    file = fopen(argv[1], "a");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int fd;

    // Install signal handler for SIGTERM
    signal(SIGTERM, sigterm_handler);
    printf("\n[TASK_FIVE] Task 5 has started!\n");
    fprintf(file, "[TASK_FIVE] Task 5 has started!\n");
 
    // FIFO file path
    char* myfifo = "/tmp/myfifo";
    // Open FIFO for write only
    fd = open(myfifo, O_RDONLY);
    while (running)
    {
        SerializedData serializedData;
        // Read and deserialize data from the named pipe
        if (read(fd, &serializedData, sizeof(SerializedData)) > 0) 
        {
            if (serializedData.type == INTEGER) 
            {
                printf("\n[TASK_FIVE] Received integer data: %d\n", serializedData.data.integerData);
                fprintf(file, "[TASK_FIVE_INTEGER] Received integer data: %d\n", serializedData.data.integerData);
            }
            else if (serializedData.type == FLOAT) 
            {
                printf("\n[TASK_FIVE] Received float data: %f\n", serializedData.data.floatData);
                fprintf(file, "[TASK_FIVE_FLOAT] Received float data: %f\n", serializedData.data.floatData);
            }
            else if (serializedData.type == STRING) 
            {
                printf("\n[TASK_FIVE] Received string data: %s\n", serializedData.data.stringData);
                fprintf(file, "[TASK_FIVE_STRING] Received string data: %s\n", serializedData.data.stringData);
            } 
            else {
                printf("Unknown data type received\n");
            }
        }
    }
    
    // Close the named pipe
    printf("\n[TASK_FIVE] closing file");
    fclose(file);
    close(fd);
    printf("\n[TASK_FIVE] Terminating...\n");
    exit(EXIT_SUCCESS);
}

void sigterm_handler() 
{
    printf("\n[TASK_FIVE] Received SIGTERM signal\n");
    running = 0;
}