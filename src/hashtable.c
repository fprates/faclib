/*
 * hashtable.c
 *
 *  Created on: 13/04/2013
 *      Author: francisco.prates
 */

#include "../include/libfac.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAP_PAGE_ENTRIES 100

struct s_map_entry {
    const char *key;
    void *value;
    struct fac_list *list;
};

struct s_map_page {
    struct s_map_entry *entries[MAP_PAGE_ENTRIES];
};

struct fac_map {
    unsigned int size, top, base;
    struct fac_list *keys;
    uintptr_t *pages;
    size_t pagesize;
};

unsigned int fac_map_size(fac_map_t *map)
{
    return map->size;
}

static struct s_map_entry *map_entry_get(fac_map_t *map, const char *key)
{
    unsigned int index;
    uintptr_t pos;
    struct s_map_page *page;
    struct s_map_entry *entry;
    struct fac_iterator *it;
    unsigned int hash = fac_hashcode(key);
    unsigned int base = (hash / MAP_PAGE_ENTRIES) * MAP_PAGE_ENTRIES;

    if (base < map->base || base > map->top)
        return NULL;

    index = hash - base;
    pos = ((uintptr_t)map->pages) + (map->pagesize * (base - map->base));
    page = (struct s_map_page *)*((uintptr_t *)pos);
    if (page == NULL)
        return NULL;

    entry = page->entries[index];
    if (entry == NULL)
        return NULL;

    if (strcmp(entry->key, key) == 0)
        return entry;

    /* busca de chaves com colisões */
    if (entry->list == NULL)
        return NULL;

    it = fac_list_iterator(entry->list);
    while (fac_iterator_has_next(it)) {
        entry = fac_iterator_next(it);
        if (strcmp(entry->key, key) == 0)
            break;
        entry = NULL;
    }

    fac_iterator_rm(it);
    return entry;
}

char fac_map_contains(fac_map_t *map, const char *key)
{
    struct s_map_entry *entry = map_entry_get(map, key);
    return (entry != NULL);
}

void *fac_map_get(fac_map_t *map, const char *key)
{
    struct s_map_entry *entry = map_entry_get(map, key);
    return (entry == NULL)? NULL : entry->value;
}

static void map_realloc(fac_map_t *map, unsigned int base)
{
    uintptr_t *map_dest;
    uintptr_t basepos;
    size_t allocsize;
    unsigned long pagedif;

    /*
     * alocação de memória
     * movimento de dados
     * inicialização de espaço alocado
     */
    if (base < map->base) {
        /* base = novo map->base */
        pagedif = map->top - base;
        allocsize = map->pagesize * (pagedif + 1);
        map->pages = realloc(map->pages, allocsize);

        basepos = (uintptr_t)map->pages;
        pagedif = map->pagesize * (map->base - base);
        /*
         *  evita superposição de memória.
         *
         *  alocação original:
         *  B         T
         *  1         2
         *  01234567890
         *  |---------|
         *  ABC
         *
         *  realocação:
         *            B         T
         *  0         1         2
         *  012345678901234567890
         *  |---------|---------|
         *  ABC
         *
         *  reposicionamento:
         *  B                   T
         *  0         1         2
         *  012345678901234567890
         *  |---------|---------|
         *            ABC
         */
        map_dest = (uintptr_t *)(basepos + pagedif);

        memmove(map_dest, map->pages, allocsize - pagedif);
        bzero(map->pages, pagedif);
        map->base = base;
    } else {
        /* base = novo map->top */
        pagedif = base - map->base;
        allocsize = map->pagesize * (pagedif + 1);
        map->pages = realloc(map->pages, allocsize);

        basepos = (uintptr_t)map->pages;
        basepos += ((map->top - map->base + 1) * map->pagesize);
        map_dest = (uintptr_t *)basepos;

        allocsize -= ((map->top - map->base + 1) * map->pagesize);
        bzero(map_dest, allocsize);
        map->top = base;
    }
}

static struct s_map_page *map_pages_init()
{
    struct s_map_page *page;
    size_t pagesize = sizeof(*page);

    page = malloc(pagesize);
    bzero(page, pagesize);
    return page;
}

static struct s_map_entry *map_entry(const char *key, void *value)
{
    struct s_map_entry *entry = malloc(sizeof(*entry));

    entry->key = key;
    entry->value = value;
    entry->list = NULL;
    return entry;
}

char *fac_map_key(fac_map_t *map, unsigned int index)
{
    return fac_list_get(map->keys, index);
}

void fac_map_put(fac_map_t *map, const char *key, void *value)
{
    uintptr_t pos;
    struct s_map_entry *entry;
    unsigned int index;
    struct s_map_page *page;
    unsigned int hash = fac_hashcode(key);
    unsigned int base = (hash / MAP_PAGE_ENTRIES) * MAP_PAGE_ENTRIES;

    if (map->base == 0) {
        map->base = base;
        map->top = base;
        page = (struct s_map_page *)*(map->pages);
    } else {
        pos = (base - map->base) * map->pagesize;

        /*
         * não tenta realocar se há páginas
         * disponíveis na tabela.
         */
        if ((base < map->base) || (base > map->top)) {
            map_realloc(map, base);
            if (base == map->base)
                pos = 0;

            page = map_pages_init();
            pos += (uintptr_t)map->pages;
            *((uintptr_t *)pos) = (uintptr_t)page;
        } else {
            pos += (uintptr_t)map->pages;
            page = (struct s_map_page *)*((uintptr_t *)pos);

            if (page == NULL) {
                page = map_pages_init();
                *((uintptr_t *)pos) = (uintptr_t)page;
            }
        }
    }

    index = hash - base;
    entry = page->entries[index];
    if (entry == NULL) {
        page->entries[index] = map_entry(key, value);
        fac_list_add(map->keys, key);
        map->size++;
    } else {
        if (entry->key == NULL) {
            entry->key = key;
            entry->value = value;
            fac_list_add(map->keys, key);
            return;
        }

        /* se houver colisão */
        if (strcmp(entry->key, key) != 0) {
            if (entry->list == NULL)
                entry->list = fac_list_ini();
            fac_list_add(entry->list, map_entry(key, value));
            fac_list_add(map->keys, key);
        } else {
            entry->value = value;
        }
    }
}

void fac_map_rm(struct fac_map *map)
{
    uintptr_t pos;
    unsigned long i, j;
    struct s_map_page *page;
    struct s_map_entry *entry;

    fac_list_rm(map->keys);

    for (i = map->base; i <= map->top; i++) {
        pos = (uintptr_t)*(map->pages+(i - map->base));
        if (pos == 0)
            continue;

        page = (struct s_map_page *)pos;
        for (j = 0; j < MAP_PAGE_ENTRIES; j++) {
            entry = page->entries[j];
            if (entry == NULL)
                continue;

            free(entry);
        }

        free(page->entries);
    }

    free(map->pages);
    free(map);

    map = NULL;
}

struct fac_map *fac_map_ini(void)
{
    struct fac_map *map = malloc(sizeof(*map));
    unsigned int size = sizeof(*map->pages);

    map->size = 0;
    map->base = 0;
    map->top = 0;
    map->pages = malloc(size);
    *map->pages = (uintptr_t)map_pages_init();
    map->keys = fac_list_ini();
    map->pagesize = sizeof(*map->pages);

    return map;
}

struct fac_iterator *fac_map_iterator(struct fac_map *map)
{
    return fac_list_iterator(map->keys);
}

void fac_map_item_rm(fac_map_t *map, const char *key)
{
    uintptr_t pos;
    struct s_map_page *page;
    unsigned int index;
    struct s_map_entry *entry;
    unsigned int hash = fac_hashcode(key);
    unsigned int base = (hash / MAP_PAGE_ENTRIES) * MAP_PAGE_ENTRIES;

    if (base < map->base || base > map->top)
        return;

    pos = ((uintptr_t)map->pages) + (map->pagesize * (base - map->base));
    page = (struct s_map_page *)*((uintptr_t *)pos);
    index = hash - base;

    entry = page->entries[index];
    if (entry == NULL)
        return;

    if (entry->key == key) {
        entry->key = NULL;
        entry->value = NULL;
    }

    if (entry->list != NULL)
        fac_list_item_rm(entry->list, key);

    fac_list_item_rm(map->keys, key);
}
