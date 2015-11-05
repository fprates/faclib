/*
 * hash.c
 *
 *  Created on: 21/04/2011
 *      Author: francisco.prates
 */

#include "../include/libfac.h"
#include <string.h>

unsigned int fac_hashcode(const char *string)
{
    unsigned int i;
    size_t len = strlen(string);
    unsigned int hash = 0;

    for (i = 0; i < len; i++)
        hash += *(string+i)*5^(len-i-1);

    return hash;
}
