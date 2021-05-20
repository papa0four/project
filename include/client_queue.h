#ifndef __CLIENT_QUEUE_H__
#define __CLIENT_QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#define MAX_CLIENTS 5

typedef struct _queue_node
{
    int                       cli_fd;   /* client's file descriptor */
    struct  _queue_node     * next;     /* a pointer to the next node */
} queue_node;

typedef struct _cli_queue
{
    queue_node    * q_head;             /* current head of the queue */
    size_t          q_size;             /* maximum queue size */
    size_t          cur_size;           /* current size of the queue */
} cli_queue;

/**
 * @brief - initialize empty queue container
 * @param num_of_clients - maximum size of client queue
 * @return - a pointer to the queue container
 */
static cli_queue * queue_init ( size_t max_size );

/**
 * @brief - add new client node to the end of the existing queue
 * @param p_queue - a pointer to the existing queue container
 * @param client_data - the clients file descriptor to store
 *                      into the queue node
 * @return - true upon successful addition, false on any errors
 *           to include trying to add to a full queue
 */
static bool enqueue ( cli_queue * p_queue, int client_data );

/**
 * @brief - remove the client's file descriptor that is
 *          at the front of the queue to pass to the server
 *          as a valid client
 * @param p_queue - a pointer to the existing queue container
 * @param fd_to_get - a pointer to the queue node containing
 *                    the clients file descriptor data
 * @return - the client's file descriptor
 */
static int dequeue ( cli_queue * p_queue, queue_node * fd_to_get );

/**
 * @brief - a boolean check to determine if the client queue is full
 * @param p_queue - a pointer to the existing queue container
 * @return - true if the queue contains the max amount of clients
 *           false upon errors or is the queue is not at max capacity
 */
static bool b_isfull ( cli_queue * p_queue );

/**
 * @brief - a boolean check to determine if the client queue is empty
 *          this is not a check to determine if the queue container exists
 *          this assumed that the queue has already been initialized but 
 *          contains no client data
 * @param p_queue - a pointer to the existing queue container
 * @return - true of the queue contains no clients
 *           false upon errors or if the existing queue contains
 *           client data
 */
static bool b_isempty ( cli_queue * p_queue );

/**
 * @brief - get the current length of the queue
 * @param p_queue - a pointer to the existing queue container
 * @return - the current number of clients contained in the queue
 */
static size_t queue_len ( cli_queue * p_queue );

/**
 * @brief - clear out any existing data nodes, free/nullify existing memory
 *          and destroy/free/nullify queue container
 * @param p_queue - a pointer to the existing queue container
 * @return - N/A
 */
static void destroy ( cli_queue * p_queue );

/**
 * @brief - a helper function for cleaning up allocated memory
 * @param mem_obj - a pointer to allocated memory obj to be free'd
 * @return - N/A
 */
static void clear_memory ( void * mem_obj );

#endif

/*** end client_queue.h ***/
