#include <time.h>
#include "../include/client_queue.h"
#include "../queue/cli_queue.c"

static cli_queue * p_queue;

static int gen_random_number()
{
    int lower   = 1;
    int upper   = 100;

    srand(time(0));
    return ((rand() % (upper - lower + 1)) + lower);
}

static void is_null_check(void * obj)
{
    if (NULL == obj)
    {
        printf("passed\n");
        return;
    }
    printf("FAILED\n");
}

static void is_not_null_check(void * obj)
{
    if (NULL == obj)
    {
        printf("FAILED\n");
        return;
    }
    printf("passed\n");
}

static void test_is_empty(void * obj)
{
    if (NULL == obj)
    {
        fprintf(stderr, " test_is_empty() - parameter passed is NULL\n");
        return;
    }
    bool b_is_empty = false;
    b_is_empty = b_isempty((cli_queue *) obj);
    if (true == b_is_empty)
    {
        printf("passed\n");
        return;
    }
    printf("FAILED\n");
}

static void test_max_clients(void * obj)
{
    if (NULL == obj)
    {
        fprintf(stderr, " test_max_clients() - parameter passed is NULL\n");
        return;
    }
    cli_queue * p_queue = (cli_queue *) obj;
    if (MAX_CLIENTS == p_queue->q_size)
    {
        printf("passed\n");
        return;
    }
    printf("FAILED\n");
}

static void clear_sizes(cli_queue * p_queue)
{
    if (NULL == p_queue)
    {
        errno = EINVAL;
        return;
    }
    p_queue->cur_size   = 0;
    p_queue->q_size     = 0;
}

static void print_queue(cli_queue * p_queue)
{
    if ((NULL == p_queue) || (NULL == p_queue->q_head))
    {
        fprintf(stderr, "print_queue() - queue or queue head is NULL\n");
        return;
    }
    int iter = 1;
    if (b_isempty(p_queue))
    {
        printf("Current Queue: (NULL)\n");
        return;
    }
    queue_node * p_current = p_queue->q_head;
    printf("Current Queue: ");
    while (p_current)
    {
        printf("Client %d. -> %d ", iter, p_current->cli_fd);
        p_current = p_current->next;
        iter++;
    }
    printf("\n");
}

static void test_queue_init()
{
    printf("test queue init ...\n");
    printf("\t test 0 passed as max size ... ");
    p_queue = queue_init(0);
    is_null_check(p_queue);
    clear_memory(p_queue);

    printf("\t test initial construct of queue ... ");
    p_queue = queue_init(MAX_CLIENTS);
    is_not_null_check(p_queue);

    printf("\t test initial queue size before enqueue ... ");
    test_is_empty(p_queue);

    printf("\t test max client queue size ... ");
    test_max_clients(p_queue);

    printf("\t test head of queue is NULL ... ");
    is_null_check(p_queue->q_head);

    /* cleanup */
    clear_sizes(p_queue);
    clear_memory(p_queue->q_head);
    clear_memory(p_queue);
}

static void test_enqueue_on_empty()
{
    int cli_fd = gen_random_number();
    printf("test enqueue to empty queue ... \n");
    p_queue = queue_init(MAX_CLIENTS);
    if (NULL == p_queue)
    {
        fprintf(stderr, "test_enqueue_on_empty() - error occurred initializing queue ... exiting\n");
        return;
    }

    printf("\t testing size of queue container == 1 ... ");
    bool b_enqueued = enqueue(p_queue, cli_fd);
    if (1 == p_queue->cur_size)
    {
        printf("passed\n");
    }
    else
    {
        printf("current queue size != 1 ... FAILED\n");
    }

    printf("\t testing if client was successfully added to queue ... \n");
    if (false == b_enqueued)
    {
        fprintf(stderr, "\t\tcould not enqueue file descriptor: %d ... FAILED\n", cli_fd);
    }
    else
    {
        printf("\t\t added %d to the client queue ... passed\n", p_queue->q_head->cli_fd);
    }

    printf("\t testing queue head node pointer is not null ... ");
    is_not_null_check(p_queue->q_head);
    printf("\t Current Queue: Client 1. -> %d\n", p_queue->q_head->cli_fd);

    /* cleanup */
    clear_sizes(p_queue);
    clear_memory(p_queue->q_head);
    clear_memory(p_queue);
}

static void test_enqueue_on_full()
{
    bool b_enqueued;
    int cli_fd;

    printf("test enqueue to full queue ... \n");
    p_queue = queue_init(MAX_CLIENTS);
    if (NULL == p_queue)
    {
        fprintf(stderr, "test_enqueue_on_full() - error occurred initializing queue ... exiting\n");
        return;
    }
    for (size_t i = 0; i < MAX_CLIENTS; i++)
    {
        cli_fd = gen_random_number();
        b_enqueued = enqueue(p_queue, cli_fd);
        if (false == b_enqueued)
        {
            fprintf(stderr, "could not enqueue %d\n", cli_fd);
            break;
        }
        usleep(1000000);
    }
    printf("\t testing add 1 additional client to full queue ... ");
    cli_fd = gen_random_number();
    b_enqueued = enqueue(p_queue, cli_fd);
    if (false == b_enqueued)
    {
        printf("passed\n");
    }
    else
    {
        printf("FAILED\n");
    }
    queue_node * p_current = p_queue->q_head;
    queue_node * p_temp = NULL;
    printf("\t\t");
    print_queue(p_queue);
    while (p_current)
    {
        p_temp = p_current;
        p_current = p_current->next;
        clear_memory(p_temp);
        p_queue->cur_size--;
    }
    clear_sizes(p_queue);
    clear_memory(p_queue);
}

int main (void)
{
    test_queue_init();
    test_enqueue_on_empty();
    test_enqueue_on_full();
    return EXIT_SUCCESS;
}