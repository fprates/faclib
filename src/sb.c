/*
 * sb.c
 *
 *  Created on: 13 de out de 2015
 *      Author: francisco.prates
 */

#include "../include/libfac.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct fac_sb {
	fac_list_t *tokens;
	size_t size;
};


fac_sb_t *fac_sb_ini(void)
{
	struct fac_sb *sb = malloc(sizeof(*sb));
	sb->tokens = fac_list_ini();
	sb->size = 0;
	return sb;
}

void fac_sb_addst(fac_sb_t *sb, const char *value)
{
	if (value == NULL)
		return;
	fac_list_add(sb->tokens, value);
	sb->size += strlen(value);
}

void fac_sb_addui(fac_sb_t *sb, unsigned int value)
{
	int digits = (value == 0)? 2 : floor(log10(value)) + 2;
	char *string = malloc(digits);
	sprintf(string, "%d", value);
	fac_sb_addst(sb, string);
}

char *fac_sb_get(fac_sb_t *sb)
{
	char *token;
	char *string = malloc(sb->size + 1);

	*((char *)string) = '\0';
	fac_iterator_t *it = fac_list_iterator(sb->tokens);
	while (fac_iterator_has_next(it)) {
		token = fac_iterator_next(it);
		strcat(string, token);
	}
	fac_iterator_rm(it);
	return string;
}

void fac_sb_rm(fac_sb_t *sb)
{
	fac_list_rm(sb->tokens);
	free(sb);
}

void fac_sb_term(fac_sb_t *sb)
{
	char *token;
	fac_iterator_t *it = fac_list_iterator(sb->tokens);
	while (fac_iterator_has_next(it)) {
		token = fac_iterator_next(it);
		free(token);
	}
	fac_iterator_rm(it);
	fac_sb_rm(sb);
}
void fac_sb_clear(fac_sb_t *sb)
{
	fac_list_rm(sb->tokens);
	sb->size = 0;
	sb->tokens = fac_list_ini();
}
