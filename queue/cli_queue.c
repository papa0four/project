#include "../include/client_queue.h"

static cli_queue * queue_init(size_t max_size)
{
    if (0 >= max_size)
    {
        errno = EINVAL;
        return NULL;
    }
    cli_queue * p_queue = calloc(1, sizeof(cli_queue));
    if (NULL == p_queue)
    {
        errno = ENOMEM;
        return NULL;
    }
    p_queue->cur_size = 0;
    p_queue->q_size = max_size;
    return p_queue;
}

static bool enqueue(cli_queue * p_queue, int client_data)
{
    if (NULL == p_queue)
    {
        errno = EINVAL;
        return false;
    }
    if (0 >= client_data)
    {
        errno = EBADFD;
        return false;
    }
    queue_node * p_node = calloc(1, sizeof(queue_node));
    if (NULL == p_node)
    {
        errno = ENOMEM;
        return false;
    }
    p_node->cli_fd = client_data;
    p_node->next = NULL;
    if (NULL == p_queue->q_head)
    {
        p_queue->q_head = p_node;
    }
    else
    {
        if (b_isfull(p_queue))
        {
            fprintf(stderr, "queue is full and is not accepting new clients\n");
            clear_memory(p_node);
            return false;
        }
        queue_node * p_current = p_queue->q_head;
        while (p_current->next)
        {
            p_current = p_current->next;
        }
        p_current->next = p_node;
    }
    p_queue->cur_size++;
    return true;
}

static bool b_isfull(cli_queue * p_queue)
{
    if (NULL == p_queue)
    {
        errno = EINVAL;
        return false;
    }
    return (MAX_CLIENTS == p_queue->cur_size);
}

static bool b_isempty(cli_queue * p_queue)
{
    if (NULL == p_queue)
    {
        errno = EINVAL;
        return false;
    }
    return (0 == p_queue->cur_size);
}

static void clear_memory(void * mem_obj)
{
    if (NULL == mem_obj)
    {
        return;
    }
    free(mem_obj);
    mem_obj = NULL;
}