#include <time.h>
#include "../include/client_queue.h"
#include "../queue/cli_queue.c"

#define SLEEPY_TIME 1000000

static cli_queue * p_queue;
static size_t      failed = 0;
static size_t      passed = 0;
static size_t      ran    = 0;


static int gen_random_container_size()
{
    int lower = 5;
    int upper = 15;

    srand(time(0));
    return ((rand() % (upper - lower + 1)) + lower);
}
static int gen_random_number()
{
    int lower   = 1;
    int upper   = 100;

    srand(time(0));
    return ((rand() % (upper - lower + 1)) + lower);
}

static void is_null_check(void * obj)
{
    ran += 1;
    if (NULL == obj)
    {
        passed += 1;
        printf("passed\n");
        return;
    }
    failed += 1;
    printf("FAILED\n");
}

static void is_not_null_check(void * obj)
{
    ran += 1;
    if (NULL == obj)
    {
        failed += 1;
        printf("FAILED\n");
        return;
    }
    passed += 1;
    printf("passed\n");
}

static void test_is_empty(void * obj)
{
    ran += 1;
    if (NULL == obj)
    {
        fprintf(stderr, " test_is_empty() - parameter passed is NULL\n");
        return;
    }
    bool b_is_empty = false;
    b_is_empty = b_isempty((cli_queue *) obj);
    if ((true == b_is_empty) || (0 == p_queue->cur_size))
    {
        passed += 1;
        printf("passed\n");
        return;
    }
    failed += 1;
    printf("FAILED\n");
}

static void test_max_clients(void * obj)
{
    ran += 1;
    if (NULL == obj)
    {
        fprintf(stderr, " test_max_clients() - parameter passed is NULL\n");
        return;
    }
    cli_queue * p_queue = (cli_queue *) obj;
    if ((MAX_CLIENTS == p_queue->q_size) || (b_isfull(p_queue)))
    {
        passed += 1;
        printf("passed\n");
        return;
    }
    failed += 1;
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
        if (NULL == p_current->next)
        {
            break;
        }
        printf("|| Client %d. | %d -> ", iter, p_current->cli_fd);
        p_current = p_current->next;
        iter++;
    }
    printf("|| Client %d. | %d --| (NULL)\n", iter, p_current->cli_fd);
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
    ran += 1;
    bool b_enqueued = enqueue(p_queue, cli_fd);
    if (1 == p_queue->cur_size)
    {
        passed += 1;
        printf("passed\n");
    }
    else
    {
        failed += 1;
        printf("current queue size != 1 ... FAILED\n");
    }

    printf("\t testing if client was successfully added to queue ... \n");
    ran += 1;
    if (false == b_enqueued)
    {
        failed += 1;
        fprintf(stderr, "\t\tcould not enqueue file descriptor: %d ... FAILED\n", cli_fd);
    }
    else
    {
        passed += 1;
        printf("\t\t added %d to the client queue ... passed\n", p_queue->q_head->cli_fd);
    }

    printf("\t testing queue head node pointer is not null ... ");
    is_not_null_check(p_queue->q_head);
    printf("\t Current Queue: || Client 1. | %d --| (NULL)\n", p_queue->q_head->cli_fd);

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

    /* populate queue to max size */
    for (size_t i = 0; i < MAX_CLIENTS; i++)
    {
        cli_fd = gen_random_number();
        b_enqueued = enqueue(p_queue, cli_fd);
        if (false == b_enqueued)
        {
            fprintf(stderr, "could not enqueue %d\n", cli_fd);
            break;
        }
        usleep(SLEEPY_TIME);
    }
    printf("\t\t Before invalid add ... \n");
    print_queue(p_queue);

    /* adding one more */
    printf("\t testing add 1 additional client to full queue ... ");
    ran += 1;
    cli_fd = gen_random_number();

    printf("\n\t trying to add %d ... \n", cli_fd);
    b_enqueued = enqueue(p_queue, cli_fd);
    printf("\t\t After add attempt ... ");
     if (false == b_enqueued)
    {
        passed += 1;
        printf("passed\n");
    }
    else
    {
        failed += 1;
        printf("FAILED\n");
    }
    print_queue(p_queue);


    /* cleanup */
    queue_node * p_current = p_queue->q_head;
    queue_node * p_temp = NULL;
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

static void test_enqueue_variable_size()
{
    int     cli_fd;
    bool    b_enqueued;

    size_t  why_not = gen_random_container_size();

    printf("test enqueue random amount of clients ... \n");
    p_queue = queue_init(why_not);
    if (NULL == p_queue)
    {
        fprintf(stderr, "test_enqueue_variable_size() - error occurred initializing queue - exiting ...\n");
        return;
    }

    /* populating queue */
    printf("\t testing enqueue %ld clients ... ", why_not);
    ran += 1;
    for (size_t i = 0; i < why_not; i++)
    {
        cli_fd = gen_random_number();
        b_enqueued = enqueue(p_queue, cli_fd);
        if (false == b_enqueued)
        {
            fprintf(stderr, "could not enqueue client: %d\n", cli_fd);
            break;
        }
        usleep(SLEEPY_TIME);
    }

    if ((false == b_enqueued) || (why_not != p_queue->cur_size))
    {
        failed += 1;
        printf("FAILED\n");
    }
    else if((true == b_enqueued) && (why_not == p_queue->cur_size))
    {
        passed += 1;
        printf("passed\n");
        print_queue(p_queue);
    }
    else
    {
        fprintf(stderr, "test_enqueue_variable_size() - something went wrong - exiting ...\n");
    }

    /* cleanup */
    queue_node * p_current = p_queue->q_head;
    queue_node * p_temp = NULL;
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

static void test_dequeue_on_empty()
{
    int dequeued;

    printf("test dequeue on empty ... \n");
    p_queue = queue_init(MAX_CLIENTS);
    if (NULL == p_queue)
    {
        fprintf(stderr, "test_dequeue_on_empty() - error occurred initializing queue ... exiting\n");
        return;
    }

    printf("\ttesting remove head of queue on empty container ... ");
    ran += 1;
    dequeued = dequeue(p_queue);
    if (-1 != dequeued)
    {
        failed += 1;
        printf("FAILED\n");
    }
    passed += 1;
    printf("passed\n");
    clear_sizes(p_queue);
    clear_memory(p_queue);
}

static void test_dequeue_on_null()
{
    printf("test dequeue on NULL queue ... \n");
    p_queue = queue_init(0);

    printf("\t testing dequeue with null container ... ");
    ran += 1;
    int cli_fd = dequeue(p_queue);
    if (-1 != cli_fd)
    {
        failed += 1;
        printf("FAILED\n");
    }
    else
    {
        passed += 1;
        printf("passed\n");
    }
    clear_memory(p_queue);

    p_queue = queue_init(MAX_CLIENTS);
    if (NULL == p_queue)
    {
        fprintf(stderr, "test_dequeue_on_null() - error occurred intializing queue - exiting...\n");
        return;
    }
    printf("\t testing dequeue with null head pointer ... ");
    ran += 1;
    cli_fd = dequeue(p_queue);
    if (-1 != cli_fd)
    {
        failed += 1;
        printf("FAILED\n");
    }
    else
    {
        passed += 1;
        printf("passed\n");
    }
    clear_sizes(p_queue);
    clear_memory(p_queue);    
}

static void test_dequeue_current_head()
{
    int     cli_fd;
    bool    b_enqueued;
    int     q_size = gen_random_container_size();

    p_queue = queue_init(q_size);
    if (NULL == p_queue)
    {
        fprintf(stderr, "test_dequeue_current_head() - error occurred initializing queue - exiting ...\n");
        return;
    }
    for (size_t i = 0; i < q_size; i++)
    {
        cli_fd = gen_random_number();
        b_enqueued = enqueue(p_queue, cli_fd);
        if (false == b_enqueued)
        {
            fprintf(stderr, "could not enqueue %d\n", cli_fd);
            break;
        }
        usleep(SLEEPY_TIME);
    }
    if ((false == b_enqueued) || (q_size != p_queue->cur_size))
    {
        fprintf(stderr, "test_dequeue_current_head() - queue population failed - exiting ... \n");
        goto CLEANUP;
    }
    else
    {
        printf("\t\t Before head removal ... \n");
        print_queue(p_queue);
    }

    printf("\t testing removal of head of queue ... ");
    ran += 1;
    cli_fd = dequeue(p_queue);
    if (-1 == cli_fd)
    {
        failed += 1;
        printf("FAILED\n");
    }
    else
    {
        passed += 1;
        printf("passed\n");
    }

    printf("\t testing if client was properly removed ... ");
    ran += 1;
    if ((p_queue->q_size - 1) == p_queue->cur_size)
    {
        passed += 1;
        printf("passed\n\t\t After head removal ... \n");
        print_queue(p_queue);
    }
    else
    {
        failed += 1;
        printf("FAILED\n\t\t After head removal ... \n");
    }

    /* cleanup */
    queue_node * p_current = p_queue->q_head;
    queue_node * p_temp = NULL;

CLEANUP:
    
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

static void print_summary()
{
    size_t total = ran;
    printf("\n\n--Run Summary:\tType\t\tTotal\tRan\tPassed\tFailed\n");
    printf("\t\ttests\t\t%ld\t%ld\t%ld\t%ld\n\n\n", total, ran, passed, failed);
}

int main (void)
{
    test_queue_init();
    test_enqueue_on_empty();
    test_enqueue_on_full();
    test_enqueue_variable_size();
    test_dequeue_on_empty();
    test_dequeue_on_null();
    test_dequeue_current_head();
    print_summary();
    return EXIT_SUCCESS;
}