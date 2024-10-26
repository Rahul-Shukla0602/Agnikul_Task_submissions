#include "shared.h"

int main_msgq_id;

int main() {
    key_t key = ftok(".", 'a');
    main_msgq_id = msgget(key, 0666);
    if (main_msgq_id == -1) {
        perror("msgget");
        exit(1);
    }

    while (1) {
        message_t msg;
        if (msgrcv(main_msgq_id, &msg, sizeof(task_data_t), TASK_COMPLETE, 0) == -1) {
            perror("msgrcv");
            break;
        }

        task_data_t* completed_task = (task_data_t*)msg.mtext;
        printf("Verifier: Verifying task %d from P%d\n", completed_task->task_id, completed_task->process_id);

        // Simulate verification process
        sleep(1);

        // Send verification result to main process
        msg.mtype = TASK_VERIFIED;
        if (msgsnd(main_msgq_id, &msg, sizeof(task_data_t), 0) == -1) {
            perror("msgsnd");
            break;
        }

        printf("Verifier: Task %d from P%d verified\n", completed_task->task_id, completed_task->process_id);
    }

    return 0;
}