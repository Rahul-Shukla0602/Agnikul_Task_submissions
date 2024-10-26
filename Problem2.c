#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct Node* insertNode(struct Node* head, int data, int position) {
    struct Node* newNode = createNode(data);
    if (position == 1) {
        newNode->next = head;
        return newNode;
    }
    struct Node* current = head;
    int currentPos = 1;
    while (current != NULL && currentPos < position - 1) {
        current = current->next;
        currentPos++;
    }
    if (current == NULL) {
        printf("Invalid position! Position exceeds list length.\n");
        free(newNode);
        return head;
    }
    newNode->next = current->next;
    current->next = newNode;
    return head;
}

struct Node* deleteNode(struct Node* head, int position) {
    if (head == NULL) {
        printf("List is empty!\n");
        return NULL;
    }
    struct Node* temp;
    if (position == 1) {
        temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    struct Node* current = head;
    int currentPos = 1;
    while (current != NULL && currentPos < position - 1) {
        current = current->next;
        currentPos++;
    }
    if (current == NULL || current->next == NULL) {
        printf("Invalid position! Position exceeds list length.\n");
        return head;
    }
    temp = current->next;
    current->next = temp->next;
    free(temp);
    
    return head;
}

void traverseNode(struct Node* head, void (*func)(int)) {
    struct Node* current = head;
    while (current != NULL) {
        func(current->data);
        current = current->next;
    }
}

int isPrime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void printSquare(int n) {
    printf("%d squared is: %d\n", n, n * n);
}

int main() {
    struct Node* head = NULL;

    printf("Creating list of prime numbers less than 20...\n");
    for (int i = 1; i < 20; i++) {
        if (isPrime(i)) {
            if (head == NULL) {
                head = createNode(i);
            } else {
                struct Node* current = head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = createNode(i);
            }
        }
    }
    
    printf("\nTraversing the list and printing squares:\n");
    traverseNode(head, printSquare);
    
    // Clean up memory
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
    
    return 0;
}