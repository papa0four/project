#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>

#define OK              42
#define UPLOAD          100
#define DOWNLOAD        200
#define READY           300
#define SENDING         400
#define FILE_NOT_FOUND  404
#define ERROR           500

/* server preprocessor defines */
#define SERV_ADDR       "127.0.0.1"
#define SERV_PORT       31337
#define MAX_STR_LEN     1024

/**
 * @brief - if file pointer passed is a valid file,
 *          determine the length of the file. Can be used 
 *          when downloading/uploading files between the client
 *          and server
 * @param p_file - a pointer to the file descriptor for the file
 * @return - upon success, returns the length of the file in bytes,
 *           if the file pointer passed is not valid, return *FILE_NOT_FOUND*
 *           if an error occurs, return -1
 */
static long get_file_len ( FILE * p_file );

/**
 * @brief - a boolean check to determine whether or not a file exists
 * @param p_file - a pointer to the file 
 * @return - true if file passed is valid/exists
 *           false upon errors or if file does not exists
 */
static bool b_isfile ( FILE * p_file );

/**
 * @brief - handles the incoming connections from each client
 *          will eventually call the server functionality the performs 
 *          the given operation requested by the client(s)
 * @param sockfd - the client's incoming socket file descriptor
 * @return - N/A
 */
static void handle_incoming_clients ( int sockfd );

/**
 * @brief - handles the clients ability to upload files to the server;
 *          within the function, the server will check for the UPLOAD (100)
 *          call from the client before performing this operation
 * @param p_file - a pointer to the file descriptor for the file
 * @return - upon success, the function call will return OK (42),
 *           signifying that the file was successfully uploaded
 *           if an error occurs, the function call will return ERROR (500)
 */
static uint16_t upload_file ( FILE * p_file );

/**
 * @brief - handles the clients ability to download a valid file from the server;
 *          within the function, the server will check for the DOWNLOAD (200)
 *          call from the client before performing this operation
 * @param p_file - a pointer to the file descriptor of the file
 * @return - upon success, the function call will return OK (42),
 *           signifying that the file was successfully sent to the client
 *           if an error occurs, the function call will return ERROR (500)
 *           if the client tries to download a file not currently on the server,
 *           the function call will return FILE_NOT_FOUND (404)
 */
static uint16_t download_file ( FILE * p_file );

/**
 * @brief - allows the client to send a command to the server and display
 *          a list of files with their permissions to the client; if the client
 *          chooses to use the cmdline only functionality, this call will end the connection
 *          if the client is using communicating interactively with the server, it will not
 * @param - N/A
 * @return - prints a list of files with permissions to the client
 */
static void display_files();

/**
 * @brief - helper function when calling free and setting objects to NULL
 * @param mem_obj - a pointer to the memory object to be free'd
 * @return - N/A
 */
static void clear_memory ( void * mem_obj );

/**
 * @brief - handles a ctrl+c call made on the server - if there are clients
 *          connected, ctrl+c will not terminate the program. This allows any
 *          client interaction to be sustained until all clients have closed
 *          their connections. If no clients are connected, then ctrl+c will
 *          work as intended
 * @param sig_no - the integer value of the SIGINT signal
 * @return - N/A
 */
static void handle_SIGINT ( int sig_no );

/**
 * @brief - boolean check to determine that a passed ipv4 address is
 *          valid and assignable
 * @param p_addr - a pointer to the ipv4 address in string format i.e. "127.0.0.1"
 * @return - true if ipv4 address is valid, false if it is not
 */
static bool b_valid_ip ( char * p_addr );

/**
 * @brief - boolean check to determine that a passed ipv4 6 is
 *          valid and assignable
 * @param p_addr - a pointer to the ipv6 address in string format i.e. "::01"
 * @return - true if ipv6 address is valid, false if it is not
 */
static bool b_valid_ip6 ( char * p_addr );

#endif

/*** end server.h ***/
