/*
 * iterator.c
 *
 *  Created on: 02/06/2013
 *      Author: francisco.prates
 */

#include "iterator.h"
#include <stdlib.h>
#include <stdint.h>

struct fac_iterator {
    unsigned int idx;
    unsigned int size;
    void *itens;
};


/**
 * Retorna cópia da lista para consulta.
 */
struct fac_iterator *iterator_ini(void *values, unsigned int size)
{
    struct fac_iterator *it = malloc(sizeof(*it));

    it->idx = 0;
    it->size = size;
    it->itens = values;

    return it;
}

void fac_iterator_reset(struct fac_iterator *it)
{
    it->idx = 0;
}

/**
 * retorna quantidade de itens de um iterator.
 */
unsigned int fac_iterator_size(struct fac_iterator *it)
{
    return it->size;
}

/**
 * Libera recursos do iterator
 */
void fac_iterator_rm(struct fac_iterator *it)
{
    if (it == NULL)
        return;

    free(it->itens);
    free(it);

    it = NULL;
}

/**
 * Verifica se há próximo item do iterator
 */
enum fac_bool fac_iterator_has_next(struct fac_iterator *it)
{
    return (it->idx < it->size)? FAC_TRUE : FAC_FALSE;
}

/**
 * Retorna próximo item da lista
 */
void *fac_iterator_next(struct fac_iterator *it)
{
    void *item;
    uintptr_t pos;
    size_t size;

    if (fac_iterator_has_next(it)) {
        size = sizeof(pos);
        pos = ((uintptr_t)it->itens) + (size * it->idx);
        item = (void *)*((uintptr_t *)pos);
        it->idx++;
        return item;
    }

    return NULL;
}
