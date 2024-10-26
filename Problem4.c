#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 1000

/**
 * Converts a string to uppercase
 * @param str The input string to be converted
 */
void convertToUpperCase(char* str) {
    if (!str) return;
    
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

/**
 * @param input The input string
 * @param output The buffer to store the reversed string
 * @return 0 on success, -1 on failure
 */
int reverseWords(const char* input, char* output) {
    if (!input || !output) return -1;

    char temp[MAX_STRING_LENGTH];
    strncpy(temp, input, MAX_STRING_LENGTH - 1);
    temp[MAX_STRING_LENGTH - 1] = '\0';

    int wordCount = 0;
    char* countPtr = strtok(temp, " ");
    while (countPtr) {
        wordCount++;
        countPtr = strtok(NULL, " ");
    }

    char** words = (char**)malloc(wordCount * sizeof(char*));
    if (!words) {
        printf("Memory allocation failed!\n");
        return -1;
    }
    strncpy(temp, input, MAX_STRING_LENGTH - 1);
    int index = 0;
    char* ptr = strtok(temp, " ");
    while (ptr && index < wordCount) {
        words[index] = strdup(ptr);
        if (!words[index]) {
            for (int i = 0; i < index; i++) {
                free(words[i]);
            }
            free(words);
            return -1;
        }
        index++;
        ptr = strtok(NULL, " ");
    }

    output[0] = '\0';
    for (int i = wordCount - 1; i >= 0; i--) {
        strcat(output, words[i]);
        if (i > 0) {
            strcat(output, " ");
        }
    }
    convertToUpperCase(output);
    for (int i = 0; i < wordCount; i++) {
        free(words[i]);
    }
    free(words);
    
    return 0;
}

/**
 * @param str The input string to validate
 * @return 1 if valid, 0 if invalid
 */
int validateInput(const char* str) {
    if (!str || strlen(str) == 0 || strlen(str) >= MAX_STRING_LENGTH) {
        return 0;
    }
    for (int i = 0; str[i]; i++) {
        if (!isalpha((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
            return 0;
        }
    }
    
    return 1;
}

int main() {
    const char* input = "Use the Force Luke";
    char output[MAX_STRING_LENGTH];
    
    if (!validateInput(input)) {
        printf("Error: Invalid input string!\n");
        return 1;
    }

    if (reverseWords(input, output) != 0) {
        printf("Error: Failed to process string!\n");
        return 1;
    }
    
    printf("Original string: %s\n", input);
    printf("Transformed string: %s\n", output);
    
    return 0;
}