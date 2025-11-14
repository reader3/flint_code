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
#include "flint/acb.h"

#define ENABLE_DEBUG 0

int acb_set_str(acb_t res, const char * inp, slong prec)
{
    int error = 0;

    // check for NULL inp
    if ( inp == NULL ) {
       error = 1;
       fprintf(stderr, "Error: NULL string input\n");
       return error;
    }

    // debug test input
#if ENABLE_DEBUG > 0
    printf("inp = %s\n",inp);
#endif

    const unsigned int len = strlen(inp) + 1;

    char * buf;
    char * split;
    const int max_tokens = 6;
    char * tokens[max_tokens];
    int num_toks = 0;

    buf = flint_malloc(len);
    if (buf == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        error = 1;
        return error;
    }
    strcpy(buf, inp);

    // Tokenize input string (by spaces)
    split = strtok(buf, " ");
    while (split != NULL && num_toks < max_tokens) {
        tokens[num_toks++] = split;
        split = strtok(NULL, " ");
    }

    // debug test tokens
#if ENABLE_DEBUG > 0
    printf("num_toks: %d\n",num_toks);
    for ( int ix=0; ix<num_toks; ix++) {
        printf("token[%d] = %s\n",ix,tokens[ix]);
    }
#endif

    // Handle different cases based on the number of tokens
    switch (num_toks) {

        case 1:

            // Single token - one pure real number

            error = arb_set_str(acb_realref(res), tokens[0], prec);
            if (error) {
                fprintf(stderr,"Error: 1 token input |%s| has invalid format.\n",tokens[0]);
                break;
            }
            arb_zero(acb_imagref(res));  // Set imaginary part to zero
            break;

        case 2:

            // Two tokens - pure real + pure imag number

            error = arb_set_str(acb_realref(res), tokens[0], prec);
            if (error) {
                fprintf(stderr,"Error: 2 token input real part |%s| has invalid format.\n",tokens[0]);
                break;
            } else {
                error = arb_set_str(acb_imagref(res), tokens[1], prec);
                if (error) {
                    fprintf(stderr,"Error: 2 token input imag part |%s| has invalid format.\n",tokens[1]);
                    break;
                }
            }
            break;

        case 3:

            // Three tokens - (one real number +/- var with no imaginary part)

            if (strcmp(tokens[1], "+/-") == 0) {
                // this is strictly a real number +/- variance
                char * tmp = flint_malloc(len);
                if (tmp == NULL) {
                    flint_free(buf);
                    fprintf(stderr, "ERROR: 3-token memory allocation for tmp failed!\n");
                    error = 1;
                    return error;
                }
                memset(tmp,'\0',len);
                strcat(tmp,tokens[0]); strcat(tmp," "); strcat (tmp,tokens[1]); strcat(tmp," "); strcat(tmp,tokens[2]);
                // Set real part
                error = arb_set_str(acb_realref(res), tmp, prec);
                if (error) {
                   fprintf(stderr,"Error: 3 token input for real |%s| has invalid format.\n",tmp);
                   flint_free(tmp);
                   break;
                 }
                // set imag to zero
                arb_zero(acb_imagref(res));  // Set imaginary part to zero
                flint_free(tmp);
            } else {
                fprintf(stderr,"Error: 3 token input for real |%s| has invalid format.\n",inp);
                error = 1;
            }
            break;

        case 4:

            // Four tokens (one real number with variance and pure imaginary number
            //              OR pure real number and imaginary number with variance)

            // (real +/- var) + pure imag (no var)
            if (strcmp(tokens[1], "+/-") == 0) {
                // this is strictly a real number +/- variance
                char * tmp = flint_malloc(len);
                if (tmp == NULL) {
                    flint_free(buf);
                    fprintf(stderr, "ERROR: 4-token memory allocation for tmp failed!\n");
                    error = 1;
                    return error;
                }
                memset(tmp,'\0',len);
                strcat(tmp,tokens[0]); strcat(tmp," "); strcat (tmp,tokens[1]); strcat(tmp," "); strcat(tmp,tokens[2]);
                // Set real part
                error = arb_set_str(acb_realref(res), tmp, prec);
                if (error) {
                    fprintf(stderr,"Error: 4 token input for real part |%s| has invalid format.\n",tmp);
                    flint_free(tmp);
                    break;
                }
                flint_free(tmp);
                // set imag to pure value
                error = arb_set_str(acb_imagref(res), tokens[3], prec);
                if (error) {
                    fprintf(stderr,"Error: 4 token input for imag part |%s| has invalid format.\n",tokens[3]);
                    break;
                }

            // pure real + (imag +/- var)
            } else if (strcmp(tokens[2], "+/-") == 0) {
                // this is strictly a pure real number with imag +/- variance

                // Set real part
                error = arb_set_str(acb_realref(res), tokens[0], prec);
                if (error) {
                    fprintf(stderr,"Error: 4 token input for real part |%s| has invalid format.\n",tokens[0]);
                    break;
                }

                char * tmp = flint_malloc(len);
                if (tmp == NULL) {
                    flint_free(buf);
                    fprintf(stderr, "ERROR: 4-token memory allocation for tmp failed!\n");
                    error = 1;
                    return error;
                }
                // set (imag +/- var)
                memset(tmp,'\0',len);
                strcat (tmp,tokens[1]); strcat(tmp," "); strcat(tmp,tokens[2]); strcat(tmp," "); strcat(tmp,tokens[3]);
                error = arb_set_str(acb_imagref(res), tmp, prec);
                if (error) {
                    fprintf(stderr,"Error: 4 token input for imag part |%s| has invalid format.\n",tmp);
                    flint_free(tmp);
                    break;
                }
                flint_free(tmp);
            } else {
               fprintf(stderr,"Error: 4-token input |%s| has invalid format.\n",inp);
            	error = 1;
            }
            break;

        case 6:
            // six tokens; must have two "+/-"
            if (strcmp(tokens[1], "+/-") == 0 && strcmp(tokens[4], "+/-") == 0) {
                char * tmp = flint_malloc(len);
                if (tmp == NULL) {
                    flint_free(buf);
                    fprintf(stderr, "ERROR: 6-token memory allocation for tmp failed!\n");
                    error = 1;
                    return error;
                }
                memset(tmp,'\0',len);
                strcat(tmp,tokens[0]); strcat(tmp," "); strcat(tmp,tokens[1]); strcat(tmp," "); strcat(tmp,tokens[2]);
                // Set real part
                error = arb_set_str(acb_realref(res), tmp, prec);
                if (error) {
                    fprintf(stderr,"Error: 6-token input real part |%s| has invalid format.\n",tmp);
                    flint_free(tmp);
                    break;
                }
                // debug test real part
#if ENABLE_DEBUG > 0
                printf("real: ");
                arb_printd(acb_realref(res), 10); // Print real part
                printf("\n");
#endif
                // Set imaginary part
                memset(tmp,'\0',len);
                strcat(tmp,tokens[3]); strcat(tmp," "); strcat(tmp,tokens[4]); strcat(tmp," "); strcat(tmp,tokens[5]);
                error = arb_set_str(acb_imagref(res), tmp, prec);
                if (error) {
                    fprintf(stderr,"Error: 6-token input imag part |%s| has invalid format.\n",tmp);
                    flint_free(tmp);
                    break;
                }
                // debug test imag part
#if ENABLE_DEBUG > 0
                printf("imag: ");
                arb_printd(acb_imagref(res), 10); // Print imag part
                printf("\n");
#endif
                flint_free(tmp);
            } else {
               fprintf(stderr,"Error: 6-token input |%s| has invalid format.\n",inp);
               error = 1;
            }
            break;

        default:
            // Handle error for unsupported formats
            fprintf(stderr,"Error: %d-token input |%s| has unknown format.\n",num_toks,inp);
            error = 1;  // Set appropriate error code
    }

    flint_free(buf);
    return error;
}

