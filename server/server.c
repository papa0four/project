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

static void handle_SIGINT(int sig_no)
{
    if (0 >= sig_no)
    {
        return;
    }

    char * p_msg = "\nSIGINT (ctrl + c) caught by handler and ignored.\n";

    if (SIGINT == sig_no)
    {
        printf("%s", p_msg);
    }
}

static void handle_incoming_clients(int sockfd)
{
    struct  sigaction ig_SIGINT = { 0 };
    int     bytes_recv;
    char  * buffer              = calloc(MAX_STR_LEN, sizeof(char));
    if (NULL == buffer)
    {
        errno = ENOMEM;
        fprintf(stderr, "handle_incoming_clients() - could not allocate memory for buffer- exiting ... \n");
        return;
    }

    while (true)
    {
        bytes_recv = recv(sockfd, buffer, MAX_STR_LEN, 0);
        if (-1 == bytes_recv)
        {
            errno = EBADF;
            fprintf(stderr, "no message received from client\n");
            break;
        }
        else if ( 0 == bytes_recv)
        {
            printf("Client has ended the connection ... \n");
            break;
        }
        else
        {
            printf("Received from client: %s\n", buffer);
        }
        ig_SIGINT.sa_handler = handle_SIGINT;
        sigfillset(&ig_SIGINT.sa_mask);
        ig_SIGINT.sa_flags = 0;
        sigaction(SIGINT, &ig_SIGINT, NULL);
    }
}

static bool b_valid_ip(char * p_addr)
{
    if (NULL == p_addr)
    {
        return false;
    }
    struct sockaddr_in sock_addr;
    int result = inet_pton(AF_INET, p_addr, &(sock_addr.sin_addr));
    return result != false;
}

static bool b_valid_ip6(char * p_addr)
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
