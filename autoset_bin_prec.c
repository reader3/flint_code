#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define ENABLE_DEBUG 0

// Function to parse and process the input string
long autoset_bin_prec(const char* inp) {

    // Step 1: Trim spaces

    while (isspace((unsigned char)*inp)) inp++;
    char *ptr = (char*)inp + strlen(inp) - 1;
    while (ptr > inp && isspace((unsigned char)*ptr)) ptr--;
    ptr[1] = '\0';  // Null-terminate at the end of trimmed string

    // Step 2: Remove brackets and substitute "+/-" with " Z "

    char *processed = (char*)malloc(strlen(inp) * 3 + 1);
    char *sub_ptr = processed;

    for (const char *src = inp; *src != '\0'; src++) {
        if (*src == '[' || *src == ']') continue;
        if (strncmp(src, "+/-", 3) == 0) {
            strcpy(sub_ptr, " Z ");
            sub_ptr += 3;
            src += 2; // Skip over "+/-"
        } else {
            *sub_ptr++ = *src;
        }
    }
    *sub_ptr = '\0';

    // Step 3: Normalize spaces

    char *cleaned = (char*)malloc(strlen(processed) + 1);
    char *clean_ptr = cleaned;
    int in_space = 0;

    for (const char *src = processed; *src != '\0'; src++) {
        if (isspace((unsigned char)*src)) {
            if (!in_space) {
                *clean_ptr++ = ' ';
                in_space = 1;
            }
        } else {
            *clean_ptr++ = *src;
            in_space = 0;
        }
    }
    *clean_ptr = '\0';
    free(processed);

    // Step 4: Find the longest token

    long longest_length = 0;
    char *longest_token = NULL;
    char *token = strtok(cleaned, " ");
    while (token) {
        long length = strlen(token);
        if (length > longest_length) {
            longest_length = length;
            longest_token = token;
        }
        token = strtok(NULL, " ");
    }

#if ENABLE_DEBUG > 0
    printf("Largest Token %s Length: %ld\n", longest_token, longest_length);
#endif

    free(cleaned);

    // Step 5: Calculate and return the precision

    return (long)ceil(longest_length * log2(10));
}
