/*
 * hashtable.c
 *
 *  Created on: 13/04/2013
 *      Author: francisco.prates
 */

#include "../include/libfac.h"
#include <stdlib.h>

struct fac_set {
	fac_map_t *map;
};

unsigned int fac_set_size(fac_set_t *set)
{
    return fac_map_size(set->map);
}

char fac_set_contains(fac_set_t *set, const char *key)
{
	return fac_map_contains(set->map, key);
}

void fac_set_add(fac_set_t *set, const char *key)
{
	fac_map_put(set->map, key, NULL);
}

void fac_set_rm(fac_set_t *set)
{
	fac_map_rm(set->map);
	free(set);
}

struct fac_set *fac_set_ini(void)
{
    struct fac_set *set = malloc(sizeof(*set));
    set->map = fac_map_ini();
    return set;
}

struct fac_iterator *fac_set_iterator(struct fac_set *set)
{
    return fac_map_iterator(set->map);
}

void fac_set_item_rm(fac_set_t *set, const char *key)
{
	fac_map_item_rm(set->map, key);
}
