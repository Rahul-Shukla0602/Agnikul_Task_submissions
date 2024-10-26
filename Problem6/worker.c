#include "shared.h"

int main_msgq_id, process_id;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment_thread(void* arg) {
    int* shared_var = (int*)arg;
    for (int i = 0; i < 50; i++) {
        pthread_mutex_lock(&mutex);
        (*shared_var)++;
        printf("Process %d - Thread %ld: %d\n", process_id, pthread_self(), *shared_var);
        pthread_mutex_unlock(&mutex);
        usleep(10000); // 10ms delay
    }
    return NULL;
}

void task_increment() {
    int shared_var = 0;
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, increment_thread, &shared_var);
    pthread_create(&thread2, NULL, increment_thread, &shared_var);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Process %d: Increment task completed. Final value: %d\n", process_id, shared_var);
}

void task_message_queue() {
    key_t key = ftok(".", 'b');
    int msgq_id = msgget(key, IPC_CREAT | 0666);
    if (msgq_id == -1) {
        perror("msgget");
        return;
    }

    // Send message
    message_t send_msg = {1, "Hello from worker"};
    if (msgsnd(msgq_id, &send_msg, strlen(send_msg.mtext) + 1, 0) == -1) {
        perror("msgsnd");
        return;
    }

    // Receive message
    message_t recv_msg;
    if (msgrcv(msgq_id, &recv_msg, MAX_MSG_SIZE, 1, 0) == -1) {
        perror("msgrcv");
        return;
    }

    printf("Process %d: Received message: %s\n", process_id, recv_msg.mtext);
    msgctl(msgq_id, IPC_RMID, NULL);
}

int base4_to_decimal(const char* base4) {
    int decimal = 0;
    int power = 1;
    for (int i = strlen(base4) - 1; i >= 0; i--) {
        decimal += (base4[i] - '0') * power;
        power *= 4;
    }
    return decimal;
}

void task_base_conversion() {
    char base4_input[10];
    printf("Process %d: Enter a base-4 number: ", process_id);
    scanf("%s", base4_input);
    int decimal = base4_to_decimal(base4_input);
    printf("Process %d: Base-4 %s is decimal %d\n", process_id, base4_input, decimal);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <process_id>\n", argv[0]);
        exit(1);
    }

    process_id = atoi(argv[1]);
    key_t key = ftok(".", 'a');
    main_msgq_id = msgget(key, 0666);
    if (main_msgq_id == -1) {
        perror("msgget");
        exit(1);
    }

    while (1) {
        message_t msg;
        if (msgrcv(main_msgq_id, &msg, sizeof(task_data_t), TASK_START, 0) == -1) {
            perror("msgrcv");
            break;
        }

        task_data_t* task = (task_data_t*)msg.mtext;
        if (task->process_id != process_id) {
            continue;
        }

        printf("Process %d: Starting task %d\n", process_id, task->task_id);

        switch (task->task_id) {
            case TASK_INCREMENT:
                task_increment();
                break;
            case TASK_MESSAGE_QUEUE:
                task_message_queue();
                break;
            case TASK_BASE_CONVERSION:
                task_base_conversion();
                break;
            default:
                fprintf(stderr, "Unknown task ID: %d\n", task->task_id);
                continue;
        }

        // Notify verifier
        msg.mtype = TASK_COMPLETE;
        task->data = 1; // Task completed successfully
        memcpy(msg.mtext, task, sizeof(task_data_t));
        if (msgsnd(main_msgq_id, &msg, sizeof(task_data_t), 0) == -1) {
            perror("msgsnd");
            break;
        }
    }

    return 0;
}