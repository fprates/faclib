//    faclista: biblioteca de lista ligada.
//    Copyright (C) 2007-2008 Francisco de Assis Prates
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//
//    faclista, Copyright (C) 2007-2008 Francisco de Assis Prates
//    faclista comes with ABSOLUTELY NO WARRANTY.

#include "../include/libfac.h"
#include "iterator.h"
#include <stdlib.h>
#include <stdint.h>

struct s_item {
    const void *ptr_obj;
    struct s_item *ptr_prox_obj;
};

struct fac_list {
    unsigned int size;
    struct s_item *primeiro_item, *itens;
};

/**
 * Cria uma lista
 */
struct fac_list *fac_list_ini()
{
    struct fac_list *list = malloc(sizeof(*list));

    list->size = 0;
    list->itens = NULL;
    list->primeiro_item = NULL;

    return list;
}

/**
 * Inclui item na lista
 */
void fac_list_add(struct fac_list *lista, const void *ptr_obj)
{
    if (lista->itens == NULL) {
        lista->itens = malloc(sizeof(*lista->itens));
        if (lista->size == 0)
            lista->primeiro_item = lista->itens;
    }

    lista->itens->ptr_obj = ptr_obj;
    lista->itens->ptr_prox_obj = malloc(sizeof(*lista->itens->ptr_prox_obj));
    lista->itens = lista->itens->ptr_prox_obj;
    lista->size++;
}

void fac_list_item_rm(struct fac_list *lista, const void *ptr_obj)
{
    struct s_item *itens;
    struct s_item *prox_itens;
    enum fac_bool ret;
    enum fac_bool item_enc;
    unsigned int qt;
    unsigned int k;

    if ((lista->size < 1) || (!fac_list_exists(lista, ptr_obj)))
        return;

    ret = FAC_FALSE;
    item_enc = FAC_FALSE;
    prox_itens = NULL;
    itens = lista->primeiro_item;
    qt = 1;

    /*
     * libera recurso da lista
     */
    while (!ret) {
        /*
         * se item encontrado, destroi entrada da lista
         */
        if (itens->ptr_obj == ptr_obj) {
            if (qt == 1)
                lista->primeiro_item = itens->ptr_prox_obj;
            else
                prox_itens = itens->ptr_prox_obj;

            free(itens);

            ret = FAC_TRUE;
            item_enc = FAC_TRUE;

            continue;
        }

        qt++;

        /*
         * verifica se atingiu o fim da lista.
         */
        if (qt <= lista->size)
            itens = itens->ptr_prox_obj;
        else
            ret = FAC_TRUE;
    }

    if (!item_enc)
        return;

    lista->size--;

    if (lista->size == 0)
        return;

    itens = lista->primeiro_item;

    /*
     * religa a entrada posterior da lista removida
     * à entrada anterior.
     */
    for (k = 1; k < qt; k++) {
        if (k == (qt - 1)) {
            itens->ptr_prox_obj = prox_itens;
            break;
        } else
            itens = itens->ptr_prox_obj;
    }
}

unsigned int fac_list_size(struct fac_list *lista)
{
    struct fac_list *lista_ = lista;

    return lista_->size;
}

/**
 * Libera recursos da lista
 */
void fac_list_rm(struct fac_list *lista)
{
    struct s_item *itens;
    struct s_item *prox_itens;
    unsigned int k;
    struct fac_list *lst_ = lista;

    itens = lst_->primeiro_item;

    for (k = 0; k < lst_->size; k++) {
        prox_itens = itens->ptr_prox_obj;
        free(itens);
        itens = prox_itens;
    }

    free(itens);
    free(lst_);
    lst_ = NULL;
}

enum fac_bool fac_list_exists(struct fac_list *lista, const void *objeto)
{
    struct fac_iterator *it = fac_list_iterator(lista);

    while(fac_iterator_has_next(it)) {
        if(fac_iterator_next(it) != objeto)
            continue;

        fac_iterator_rm(it);
        return FAC_TRUE;
    }

    return FAC_FALSE;
}

struct fac_iterator *fac_list_iterator(struct fac_list *list)
{
    struct s_item *itens, *prox_itens;
    unsigned int k;
    uintptr_t *values, *mark;
    size_t size = sizeof(*values);

    values = malloc(size * list->size);
    itens = list->primeiro_item;

    for (k = 0; k < list->size; k++) {
        prox_itens = itens->ptr_prox_obj;
        mark = (uintptr_t *)(((uintptr_t)values) + (size * k));
        *mark = (uintptr_t)itens->ptr_obj;
        itens = prox_itens;
    }

    return iterator_ini(values, list->size);
}

void *fac_list_get(struct fac_list *list, unsigned int nr)
{
    struct fac_iterator *it;
    void *item = NULL;
    unsigned int c = 0;

    it = fac_list_iterator(list);
    while (fac_iterator_has_next(it)) {
        item = fac_iterator_next(it);
        if (c++ == nr)
            break;

        item = NULL;
    }

    fac_iterator_rm(it);

    return item;
}
