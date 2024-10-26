#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define MAX_MSG_SIZE 100
#define SHM_SIZE 1024

// Message types
#define TASK_START 1
#define TASK_COMPLETE 2
#define TASK_VERIFY 3
#define TASK_VERIFIED 4

// Message types and task IDs as defined constants
#define TASK_INCREMENT 1
#define TASK_MESSAGE_QUEUE 2
#define TASK_BASE_CONVERSION 3


// A message structure (message_t) for inter-process communication
typedef struct {
    long mtype;
    char mtext[MAX_MSG_SIZE];
} message_t;


// A task data structure (task_data_t) to pass task information between processes
typedef struct {
    int task_id;
    int process_id;
    int data;
} task_data_t;

#endif // SHARED_H