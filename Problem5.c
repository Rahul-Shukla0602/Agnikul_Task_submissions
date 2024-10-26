#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HASH_SIZE 256  // 8-bit hash (2^8)
#define MAX_NAME_LENGTH 50

typedef struct HashNode {
    char* name;
    int age;
    struct HashNode* next; 
} HashNode;

typedef struct {
    HashNode* table[HASH_SIZE];
    size_t size;
} HashTable;

/**
 * @param str Input string to hash
 * @return 8-bit hash value
 */
uint8_t hash_function(const char* str) {
    uint8_t hash = 0;
    
    while (*str) {
        hash = (hash * 33) + (unsigned char)*str;
        str++;
    }
    
    return hash;
}

/**
 * @return Pointer to initialized hash table
 */
HashTable* create_hash_table(void) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) {
        return NULL;
    }
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->table[i] = NULL;
    }
    ht->size = 0;
    
    return ht;
}

/**
 * @param name Name string
 * @param age Age value
 * @return Pointer to new node
 */
HashNode* create_node(const char* name, int age) {
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (!node) {
        return NULL;
    }
    
    node->name = strdup(name);
    if (!node->name) {
        free(node);
        return NULL;
    }
    
    node->age = age;
    node->next = NULL;
    return node;
}

/**
 * @param ht Hash table
 * @param name Name string
 * @param age Age value
 * @return 0 on success, -1 on failure
 */
int insert(HashTable* ht, const char* name, int age) {
    if (!ht || !name) {
        return -1;
    }
    
    uint8_t index = hash_function(name);
    HashNode* node = create_node(name, age);
    if (!node) {
        return -1;
    }

    if (ht->table[index] == NULL) {
        ht->table[index] = node;
    } else {
        HashNode* current = ht->table[index];
        while (current) {
            if (strcmp(current->name, name) == 0) {
                current->age = age;
                free(node->name);
                free(node);
                return 0;
            }
            current = current->next;
        }
        node->next = ht->table[index];
        ht->table[index] = node;
    }
    
    ht->size++;
    return 0;
}

/**
 * @param ht Hash table
 * @param name Name to search for
 * @param age Pointer to store retrieved age
 * @return 0 on success, -1 if not found
 */
int get_age(HashTable* ht, const char* name, int* age) {
    if (!ht || !name || !age) {
        return -1;
    }
    
    uint8_t index = hash_function(name);
    HashNode* current = ht->table[index];
    
    while (current) {
        if (strcmp(current->name, name) == 0) {
            *age = current->age;
            return 0;
        }
        current = current->next;
    }
    
    return -1;
}

/**
 * 
 * @param ht Hash table to free
 */
void free_hash_table(HashTable* ht) {
    if (!ht) {
        return;
    }
    
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* current = ht->table[i];
        while (current) {
            HashNode* next = current->next;
            free(current->name);
            free(current);
            current = next;
        }
    }
    
    free(ht);
}

/**
 * 
 * @param ht Hash table to print
 */
void print_hash_table(HashTable* ht) {
    if (!ht) {
        return;
    }
    
    printf("\nHash Table Contents:\n");
    printf("-------------------\n");
    
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* current = ht->table[i];
        if (current) {
            printf("Index %3d: ", i);
            while (current) {
                printf("[%s: %d] ", current->name, current->age);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main() {
    const char* names[] = {
        "Dylan Felix", "Rodney Briggs", "Alia Dejesus", "Rio Case",
        "Cleo Perry", "Waylon Cummings", "Nylah Melton", "Lennon Ross",
        "Peyton Horne", "Zev Galvan"
    };
    const int ages[] = {10, 21, 43, 32, 54, 102, 1, 2, 1, 23};
    const int data_size = sizeof(names) / sizeof(names[0]);
    
    HashTable* ht = create_hash_table();
    if (!ht) {
        printf("Failed to create hash table!\n");
        return 1;
    }
    
    printf("Inserting data into hash table...\n");
    for (int i = 0; i < data_size; i++) {
        if (insert(ht, names[i], ages[i]) != 0) {
            printf("Failed to insert %s!\n", names[i]);
            free_hash_table(ht);
            return 1;
        }
    }

    print_hash_table(ht);
    
    printf("\nRetrieving ages for all names:\n");
    printf("------------------------------\n");
    for (int i = 0; i < data_size; i++) {
        int age;
        if (get_age(ht, names[i], &age) == 0) {
            printf("%s is %d years old\n", names[i], age);
        } else {
            printf("Failed to retrieve age for %s\n", names[i]);
        }
    }
    
    printf("\nTesting non-existent name lookup:\n");
    int age;
    if (get_age(ht, "John Doe", &age) == -1) {
        printf("John Doe not found in hash table (as expected)\n");
    }
    free_hash_table(ht);
    
    return 0;
}