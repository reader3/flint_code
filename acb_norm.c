/*
    Author: Randall L. Rathbun
    Date: Tue 11 Nov 2025 09:51:26 AM PST

    This file is intended to become part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/
#include "flint/acb.h"

int acb_norm(arb_t mod, const acb_t x, const slong prec)
{
    arb_t re, im, re_sq, im_sq, mod_sq;
    arb_init(re); arb_init(im);
    arb_init(re_sq); arb_init(im_sq);
    arb_init(mod_sq);

    // get real and imag parts
    arb_set(re, acb_realref(x));
    arb_set(im, acb_imagref(x));

    // find squares for each part
    arb_sqr(re_sq,re,prec);
    arb_sqr(im_sq,im,prec);
    arb_clear(re); arb_clear(im);

    // find square of the modulus
    arb_add(mod_sq,re_sq,im_sq,prec);
    arb_clear(re_sq); arb_clear(im_sq);
    
    arb_sqrt(mod, mod_sq, prec);
    arb_clear(mod_sq);

    int error = 0;
    return error;
}
