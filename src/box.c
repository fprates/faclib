/*
 * box.c
 *
 *  Created on: 05/06/2013
 *      Author: francisco.prates
 */

#include "../include/libfac.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct fac_box {
    enum e_types type;
    union {
        char _char;
        unsigned _uchar;
        short _short;
        unsigned short _ushort;
        int _int;
        unsigned int _uint;
        long _long;
        unsigned long _ulong;
        float _float;
        double _double;
    } value;
};

static struct fac_box *box_ini(enum e_types box_type)
{
    struct fac_box *box = malloc(sizeof(*box));
    box->type = box_type;

    return box;
}

struct fac_box *fac_box_char(char value)
{
    struct fac_box *box = box_ini(CHAR);
    box->value._char = value;

    return box;
}

struct fac_box *fac_box_double(double value)
{
    struct fac_box *box = box_ini(DOUBLE);
    box->value._double = value;

    return box;
}

struct fac_box *fac_box_float(float value)
{
    struct fac_box *box = box_ini(FLOAT);
    box->value._float = value;

    return box;
}

struct fac_box *fac_box_int(int value)
{
    struct fac_box *box = box_ini(INT);
    box->value._int = value;

    return box;
}

struct fac_box *fac_box_long(long value)
{
    struct fac_box *box = box_ini(LONG);
    box->value._long = value;

    return box;
}

struct fac_box *fac_box_short(short value)
{
    struct fac_box *box = box_ini(SHORT);
    box->value._short = value;

    return box;
}

void fac_box_rm(struct fac_box *box)
{
    free(box);
    box = NULL;
}

struct fac_box *fac_box_uchar(unsigned char value)
{
    struct fac_box *box = box_ini(UCHAR);
    box->value._uchar = value;

    return box;
}

struct fac_box *fac_box_uint(unsigned int value)
{
    struct fac_box *box = box_ini(UINT);
    box->value._uint = value;

    return box;
}

struct fac_box *fac_box_ulong(unsigned long value)
{
    struct fac_box *box = box_ini(ULONG);
    box->value._ulong = value;

    return box;
}

struct fac_box *fac_box_ushort(unsigned short value)
{
    struct fac_box *box = box_ini(USHORT);
    box->value._ushort = value;

    return box;
}

char *fac_box_value_char(struct fac_box *box)
{
    unsigned int len;
    char *value;

    switch (box->type) {
    case CHAR:
        len = log10(box->value._char) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%c", box->value._char);
        return value;
    case FLOAT:
        len = log10(box->value._float) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%f", box->value._float);
        return value;
    case INT:
        len = log10(box->value._int) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%d", box->value._int);
        return value;
    case LONG:
        len = log10(box->value._long) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%ld", box->value._long);
        return value;
    case SHORT:
        len = log10(box->value._short) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%d", box->value._short);
        return value;
    case UCHAR:
        len = log10(box->value._uchar) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%u", box->value._uchar);
        return value;
    case UINT:
        len = log10(box->value._uint) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%u", box->value._uint);
        return value;
    case ULONG:
        len = log10(box->value._ulong) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%lu", box->value._ulong);
        return value;
    case USHORT:
        len = log10(box->value._ushort) + 2;
        value = malloc(sizeof(len));
        snprintf(value, len, "%u", box->value._ushort);
        return value;
    default:
        return NULL;
    }
}

void *fac_box_value_get(struct fac_box *box)
{
    return &box->value;
}
