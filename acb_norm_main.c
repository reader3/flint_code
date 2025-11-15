/*
    Author: Randall L. Rathbun
    Date: Tue 11 Nov 2025 09:51:26 AM PST

    This file is intended to become part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>
#include "acb_set_str.h"
#include "autoset_bin_prec.h"
#include "acb_norm.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_string>\n", argv[0]);
        return 1;
    }
    printf("Raw: |%s|\n",argv[1]);

    int error = 0;

    arb_t norm;
    arb_init(norm);

    acb_t result;
    acb_init(result);

    // try to auto-set the precision
    slong bit_precision = autoset_bin_prec(argv[1]);
    if (bit_precision < 128 ) { bit_precision = 128; }
    double d_digits = bit_precision;
    d_digits = d_digits / 3.321928094887362;
    slong digits = d_digits; // Example precision

    printf("Using bit precision = %d decimal digits: %d\n",bit_precision, digits);

    // creating the acb_t complex number
    error = acb_set_str(result, argv[1], bit_precision);
    if (error) {
        fprintf(stderr, "Error setting complex number from string.\n");
    } else {
        printf("Complex number: [");
        arb_printd(acb_realref(result), 20); // Print real part
        printf("] + [");
        arb_printd(acb_imagref(result), 20); // Print imaginary part
        printf("]*i\n");
    }

    // finding the norm(C)
    printf("Testing norm function:\n");
    acb_norm(norm, result, bit_precision);
    printf("Complex modulus or norm: ");
    arb_printd(norm, digits);
    printf("\n");

    arb_clear(norm);
    acb_clear(result);
    return 0;
}
