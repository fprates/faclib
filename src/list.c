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

#include "../include/faclib.h"
#include <stdlib.h>

struct s_item {
    void *ptr_obj;
    struct s_item *ptr_prox_obj;
};

struct fac_lista {
    unsigned int qt;
    struct s_item *primeiro_item;
    struct s_item *itens;
};

struct fac_iterador {
    unsigned int idx;
    unsigned int qt;
    void *ini;
    struct s_item *itens;
};

/**
 * Cria uma lista
 */
struct fac_lista *fac_ini_lista()
{
    struct fac_lista *lst = malloc(sizeof(*lst));

    lst->qt = 0;
    lst->itens = NULL;
    lst->primeiro_item = NULL;

    return lst;
}

/**
 * Inclui item na lista
 */
void fac_inc_item(struct fac_lista *lista, void *ptr_obj)
{
    if (lista->itens == NULL) {
        lista->itens = malloc(sizeof(*lista->itens));
        if (lista->qt == 0)
            lista->primeiro_item = lista->itens;
    }

    lista->itens->ptr_obj = ptr_obj;
    lista->itens->ptr_prox_obj = malloc(sizeof(*lista->itens->ptr_prox_obj));
    lista->itens = lista->itens->ptr_prox_obj;
    lista->qt++;
}

void fac_rm_item(struct fac_lista *lista, void *ptr_obj)
{
    struct s_item *itens;
    struct s_item *prox_itens;
    enum fac_bool ret;
    enum fac_bool item_enc;
    unsigned int qt;
    unsigned int k;

    if ((lista->qt < 1) || (!fac_existe_item(lista, ptr_obj)))
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
        if (qt <= lista->qt)
            itens = itens->ptr_prox_obj;
        else
            ret = FAC_TRUE;
    }

    if (!item_enc)
        return;

    lista->qt--;

    if (lista->qt == 0)
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

/**
 * Retorna cópia da lista para consulta.
 */
struct fac_iterador *fac_ini_iterador(struct fac_lista *lista)
{
    struct fac_lista *lst = lista;
    struct fac_iterador *it_ = malloc(sizeof(*it_));

    it_->idx = 0;
    it_->qt = lst->qt;
    it_->itens = lst->primeiro_item;
    it_->ini = lst->primeiro_item;

    return it_;
}

void fac_rst_iterador(struct fac_iterador *it)
{
    it->idx = 0;
    it->itens = it->ini;
}

unsigned int fac_qt_itens(struct fac_lista *lista)
{
    struct fac_lista *lista_ = lista;

    return lista_->qt;
}

/**
 * retorna quantidade de itens de um iterador.
 */
unsigned int fac_qt_itens_it(struct fac_iterador *it)
{
    return it->qt;
}

/**
 * Libera recursos do iterador
 */
void fac_rm_iterador(struct fac_iterador *it)
{
    if (it == NULL)
        return;

    free(it);

    it = NULL;
}

/**
 * Libera recursos da lista
 */
void fac_rm_lista(struct fac_lista *lista)
{
    struct s_item *itens;
    struct s_item *prox_itens;
    unsigned int k;
    struct fac_lista *lst_ = lista;

    itens = lst_->primeiro_item;

    for (k = 0; k < lst_->qt; k++) {
        prox_itens = itens->ptr_prox_obj;
        free(itens);
        itens = prox_itens;
    }

    free(lst_);
    lst_ = NULL;
}

/**
 * Verifica se há próximo item do iterador
 */
enum fac_bool fac_existe_prox(struct fac_iterador *it)
{
    return (it->idx < it->qt)?FAC_TRUE:FAC_FALSE;
}

/**
 * Retorna próximo item da lista
 */
void *fac_proximo(struct fac_iterador *it)
{
    struct s_item *itens;

    if (fac_existe_prox(it)) {
        it->idx++;
        itens = it->itens;
        it->itens = it->itens->ptr_prox_obj;

        return itens->ptr_obj;
    }

    return NULL;
}

enum fac_bool fac_existe_item(struct fac_lista *lista, void *objeto)
{
    struct fac_iterador *it = fac_ini_iterador(lista);

    while(fac_existe_prox(it)) {
        if(fac_proximo(it) == objeto) {
            fac_rm_iterador(it);

            return FAC_TRUE;
        }
    }

    return FAC_FALSE;
}

void *fac_ret_item(struct fac_lista *lista, unsigned int nr)
{
    struct fac_iterador *it;
    void *item = NULL;
    unsigned int c = 0;

    if (nr == 0)
        return NULL;

    it = fac_ini_iterador(lista);

    while (fac_existe_prox(it)) {
        item = fac_proximo(it);
        if (++c == nr)
            break;

        item = NULL;
    }

    fac_rm_iterador(it);

    return item;
}
