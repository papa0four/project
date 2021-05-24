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

static bool b_is_valid_ip(char * p_addr)
{
    if (NULL == p_addr)
    {
        return false;
    }
    struct sockaddr_in sock_addr;
    int result = inet_pton(AF_INET, p_addr, &(sock_addr.sin_addr));
    return result != false;
}

static bool b_is_valid_ip6(char * p_addr)
{
    if (NULL == p_addr)
    {
        return false;
    }
    struct sockaddr_in6 sock_addr;
    int result = inet_pton(AF_INET6, p_addr, &(sock_addr.sin6_addr));
    return result != false;
}

/*** end server.c ***/
