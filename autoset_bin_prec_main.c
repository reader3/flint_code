#include <stdio.h>
#include <stdlib.h>
#include "autoset_bin_prec.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_string>\n", argv[0]);
        return 1;
    }
    printf("Raw: |%s|\n",argv[1]);
    long result = autoset_bin_prec(argv[1]);
    printf("Binary Precision: %ld\n", result);
    return 0;
}
