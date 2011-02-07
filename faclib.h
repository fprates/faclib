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

struct fac_lista;
struct fac_iterador;

extern void *fac_ld_lib(char *);
extern void *fac_ret_proc_lib(void *, char *);
extern void fac_lib_term(void *);
extern struct fac_lista *fac_ini_lista();
extern void fac_inc_item(struct fac_lista *, void *);
extern void fac_rm_item(struct fac_lista *, void *);
extern struct fac_iterador *fac_ini_iterador(struct fac_lista *);
extern void fac_rst_iterador(struct fac_iterador *);
extern unsigned int fac_qt_itens(struct fac_lista *);
extern unsigned int fac_qt_itens_it(struct fac_iterador *);
extern void fac_rm_iterador(struct fac_iterador *);
extern void fac_rm_lista(struct fac_lista *);
extern void *fac_ret_item(struct fac_lista *, unsigned int);
extern enum fac_bool fac_existe_prox(struct fac_iterador *);
extern enum fac_bool fac_existe_item(struct fac_lista *, void *);
extern void *fac_proximo(struct fac_iterador *);

#endif /* FACLIB_H_ */
