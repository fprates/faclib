/*
 * hash.c
 *
 *  Created on: 21/04/2011
 *      Author: francisco.prates
 */

#include "../include/faclib.h"
#include <string.h>

unsigned int fac_hashcode(char *string)
{
    unsigned int i, len = strlen(string);
    unsigned int hash = 0;

    for (i = 0; i < len; i++)
        hash += *(string+i)*5^(len-i-1);

    return hash;
}
