#include "shared.h"

int main_msgq_id, p2_pid, p3_pid, p4_pid;

void cleanup() {
    msgctl(main_msgq_id, IPC_RMID, NULL);
}

void signal_handler(int signum) {
    printf("Caught signal %d. Cleaning up and exiting.\n", signum);
    cleanup();
    exit(1);
}

int send_task(int pid, int task_id, int data) {
    message_t msg;
    msg.mtype = TASK_START;
    task_data_t task_data = {task_id, pid, data};
    memcpy(msg.mtext, &task_data, sizeof(task_data_t));
    return msgsnd(main_msgq_id, &msg, sizeof(task_data_t), 0);
}

int main() {
    key_t key = ftok(".", 'a');
    main_msgq_id = msgget(key, IPC_CREAT | 0666);
    if (main_msgq_id == -1) {
        perror("msgget");
        exit(1);
    }

    signal(SIGINT, signal_handler);
    atexit(cleanup);

    p2_pid = fork();
    if (p2_pid == 0) {
        execl("./worker", "worker", "2", NULL);
        perror("execl worker 2");
        exit(1);
    }

    p3_pid = fork();
    if (p3_pid == 0) {
        execl("./worker", "worker", "3", NULL);
        perror("execl worker 3");
        exit(1);
    }

    p4_pid = fork();
    if (p4_pid == 0) {
        execl("./verifier", "verifier", NULL);
        perror("execl verifier");
        exit(1);
    }
    int current_task = TASK_INCREMENT;
    int current_process = 2;

    while (1) {
        printf("Initiating task %d for P%d\n", current_task, current_process);
        if (send_task(current_process, current_task, 0) == -1) {
            perror("send_task");
            break;
        }
        message_t msg;
        if (msgrcv(main_msgq_id, &msg, sizeof(task_data_t), TASK_VERIFIED, 0) == -1) {
            perror("msgrcv");
            break;
        }
        task_data_t* verified_task = (task_data_t*)msg.mtext;
        printf("Task %d completed and verified for P%d\n", verified_task->task_id, verified_task->process_id);
        current_task = (current_task % 3) + 1;
        current_process = (current_process == 2) ? 3 : 2;
    }

    return 0;
}