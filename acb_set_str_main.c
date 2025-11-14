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
#include <ctype.h>
#include "flint/acb.h"
#include "autoset_bin_prec.h"
#include "acb_set_str.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_string>\n", argv[0]);
        return 1;
    }

    int error = 0;

    arb_t real;
    arb_init(real);

    acb_t result;
    acb_init(result);

    slong precision = autoset_bin_prec(argv[1]);
    double dprec = precision; dprec = dprec / 3.321928094887362;
    slong digits = dprec; // Example precision
    printf("Using binary bits: %d digits: %d\n",precision,digits);

// NULL string test
/*
    printf("NULL check:\n");
    error = acb_set_str(result, NULL, precision);
*/
    error = acb_set_str(result, argv[1], precision);
    if (error) {
        fprintf(stderr, "Error setting complex number from string.\n");
    } else {
        printf("Complex number: [");
        arb_printd(acb_realref(result), 10); // Print real part
        printf("] + [");
        arb_printd(acb_imagref(result), 10); // Print imaginary part
        printf("]*i\n");
    }

    arb_clear(real);
    acb_clear(result);
    return 0;
}
