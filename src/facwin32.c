#ifdef __WIN32__
#include "faclib.h"
#include <windows.h>
#include <stdlib.h>

void *fac_ld_lib(char *lib_path)
{
    return LoadLibrary(lib_path);
}

void *fac_ret_proc_lib(void *lib_hnd, char *proc)
{
    void *proc_addr;

    if (lib_hnd == NULL)
        return NULL;

    proc_addr = GetProcAddress(lib_hnd, proc);
    return proc_addr;
}

void fac_lib_term(void *lib_hnd)
{
    if ((lib_hnd) != NULL)
		FreeLibrary(lib_hnd);
}
#endif
