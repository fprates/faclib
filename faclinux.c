#ifdef linux
#include "faclib.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

void *fac_ld_lib(char *lib_path)
{
    void *ret = dlopen(lib_path, RTLD_LAZY);
    if (ret == NULL)
        fprintf(stderr, "%s\n", dlerror());

    return ret;
}

void *fac_ret_proc_lib(void *lib_hnd, char *proc)
{
    void *ret;

    if (lib_hnd == NULL)
        return NULL;

    ret = dlsym(lib_hnd, proc);

    if (ret == NULL)
        fprintf(stderr, "%s\n", dlerror());

    return ret;
}

void fac_lib_term(void *lib_hnd)
{
    if (lib_hnd == NULL)
        return;

    dlclose(lib_hnd);
    lib_hnd = NULL;
}
#endif
