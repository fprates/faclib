/*
 * hashtable.c
 *
 *  Created on: 13/04/2013
 *      Author: francisco.prates
 */

#include "../include/faclib.h"
#include <stdlib.h>
#include <string.h>

struct s_map_entry {
    char *key;
    void *value;
};

struct fac_map {
    unsigned int size, top, base;
    struct fac_lista *keys;
    struct s_map_entry *entries;
};

unsigned int fac_map_size(struct fac_map *map)
{
    return map->size;
}

void *fac_map_get(struct fac_map *map, char *key)
{
    unsigned int hash = fac_hashcode(key);
    unsigned int index = hash - map->base;
    struct s_map_entry *entry = map->entries+index;

    return entry->value;
}

static unsigned int get_map_pages(struct fac_map *map)
{
    unsigned int pages = map->top - map->base;
    pages /= 100;
    return pages + 1;
}

static void map_realloc(struct fac_map *map, unsigned int base)
{
    unsigned int _pages, alloclen, maplen, diflen;
    unsigned int entrylen = sizeof(*map->entries);
    int pagedif = map->base - base;
    unsigned int _pagedif = (pagedif < 0)? -pagedif : pagedif;

    // calcula realocação de memória
    diflen = entrylen * _pagedif;
    _pages = get_map_pages(map);
    maplen = entrylen * 100 * _pages;
    alloclen = diflen + maplen;
    map->entries = realloc(map->entries, alloclen);

    // move páginas se houver nova base
    if (pagedif > 0) {
        memcpy((map->entries+diflen), map->entries, maplen);
        bzero(map->entries, diflen);
        map->base -= (_pagedif * 100);
    } else {
        bzero((map->entries+maplen), diflen);
        map->top = map->base + _pagedif;
    }
}

void fac_map_put(struct fac_map *map, char *key, void *value)
{
    struct s_map_entry *entry;
    unsigned int index;
    unsigned int hash = fac_hashcode(key);
    unsigned int base = (hash/100)*100;

    if (map->base == 0) {
        map->base = base;
        map->top = base;
    } else {
        /*
         * não tenta realocar se há páginas
         * disponíveis na tabela.
         */
        if ((map->base != base) && (base > map->top))
            map_realloc(map, base);
    }

    index = hash - map->base;
    entry = map->entries+index;
    if (entry->key == NULL) {
        entry = malloc(sizeof(*entry));
        entry->key = key;
        entry->value = value;
        *(map->entries+index) = *entry;
        fac_inc_item(map->keys, key);
        map->size++;
    } else {
        entry->value = value;
    }
}

void fac_map_rm(struct fac_map *map)
{
    fac_rm_lista(map->keys);
    free(map->entries);
    free(map);
    map = NULL;
}

struct fac_map *fac_map_ini(void)
{
    struct fac_map *map = malloc(sizeof(*map));

    map->size = 0;
    map->base = 0;
    map->top = 0;
    map->entries = malloc(sizeof(*map->entries)*100);
    map->keys = fac_ini_lista();
    bzero(map->entries, sizeof(*(map->entries)));

    return map;
}

struct fac_iterador *fac_map_iterator(struct fac_map *map)
{
    return fac_ini_iterador(map->keys);
}
