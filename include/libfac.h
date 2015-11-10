/*
 * faclib.h
 *
 *  Created on: 04/02/2009
 *      Author: teste
 */

#ifndef FACLIB_H_
#define FACLIB_H_

enum fac_bool {
  FAC_FALSE,
  FAC_TRUE
};

enum e_types {
    NONE,
    SHORT,
    USHORT,
    CHAR,
    UCHAR,
    STRING,
    INT,
    UINT,
    LONG,
    ULONG,
    FLOAT,
    DOUBLE
};

typedef struct fac_list fac_list_t;
typedef struct fac_iterator fac_iterator_t;
typedef struct fac_map fac_map_t;
typedef struct fac_box fac_box_t;
typedef struct fac_sb fac_sb_t;
typedef struct fac_set fac_set_t;

extern void *fac_ld_lib(char *);
extern void *fac_ret_proc_lib(void *, char *);
extern void fac_lib_term(void *);

extern void fac_iterator_reset(fac_iterator_t *iterator);
extern void fac_iterator_rm(fac_iterator_t *iterator);
extern enum fac_bool fac_iterator_has_next(fac_iterator_t *iterator);
extern void *fac_iterator_next(fac_iterator_t *iterator);
extern unsigned int fac_iterator_size(fac_iterator_t *iterator);

extern fac_list_t *fac_list_ini(void);
extern void fac_list_add(fac_list_t *list, const void *value);
extern void fac_list_item_rm(fac_list_t *list, const void *value);
extern unsigned int fac_list_size(fac_list_t *list);
extern void fac_list_rm(fac_list_t *list);
extern void *fac_list_get(fac_list_t *list, unsigned int index);
extern enum fac_bool fac_list_exists(fac_list_t *list, const void *value);
extern fac_iterator_t *fac_list_iterator(fac_list_t *list);

extern fac_map_t *fac_map_ini(void);
extern unsigned int fac_map_size(fac_map_t *map);
extern void *fac_map_get(fac_map_t *map, const char *key);
extern void fac_map_put(fac_map_t *map, const char *key, void *value);
extern void fac_map_rm(fac_map_t *map);
extern unsigned int fac_hashcode(const char *key);
extern fac_iterator_t *fac_map_iterator(fac_map_t *map);
extern char *fac_map_key(fac_map_t *map, unsigned int);
extern void fac_map_item_rm(fac_map_t *map, const char *key);
extern char fac_map_contains(fac_map_t *map, const char *key);

extern fac_box_t *fac_box_char(char value);
extern fac_box_t *fac_box_double(double value);
extern fac_box_t *fac_box_float(float value);
extern fac_box_t *fac_box_int(int value);
extern fac_box_t *fac_box_long(long value);
extern fac_box_t *fac_box_short(short value);
extern fac_box_t *fac_box_uchar(unsigned char value);
extern fac_box_t *fac_box_uint(unsigned int value);
extern fac_box_t *fac_box_ulong(unsigned long value);
extern fac_box_t *fac_box_ushort(unsigned short value);
extern void *fac_box_value_get(fac_box_t *box);
extern char *fac_box_value_char(fac_box_t *box);
extern void fac_box_rm(fac_box_t *box);

extern fac_sb_t *fac_sb_ini(void);
extern void fac_sb_addst(fac_sb_t *sb, const char *value);
extern void fac_sb_addui(fac_sb_t *sb, unsigned int value);
extern char *fac_sb_get(fac_sb_t *sb);
extern void fac_sb_rm(fac_sb_t *sb);
extern void fac_sb_term(fac_sb_t *sb);
extern void fac_sb_clear(fac_sb_t *sb);

extern unsigned int fac_set_size(fac_set_t *set);
extern char fac_set_contains(fac_set_t *set, const char *key);
extern void fac_set_add(fac_set_t *set, const char *key);
extern void fac_set_rm(fac_set_t *set);
extern struct fac_set *fac_set_ini(void);
extern struct fac_iterator *fac_set_iterator(struct fac_set *set);
extern void fac_set_item_rm(fac_set_t *set, const char *key);
extern char fac_set_contains(fac_set_t *set, const char *key);

#endif /* FACLIB_H_ */
