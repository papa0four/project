#include "../include/server.h"

static void clear_memory(void * mem_obj)
{
    if (NULL == mem_obj)
    {
        return;
    }
    free(mem_obj);
    mem_obj = NULL;
}

/*** end server.c ***/
