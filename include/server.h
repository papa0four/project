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
#include <pthread.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>

#define UPLOAD          100
#define DOWNLOAD        200
#define READY           300
#define SENDING         400
#define FILE_NOT_FOUND  404
#define ERROR           500

static long     get_file_len ( FILE * p_file );
static bool     b_isfile ( FILE * p_file );
static uint8_t  upload_file ( FILE * p_file );
static uint8_t  download_file ( FILE * p_file );
static void     clear_memory ( void * mem_obj );
static void     handle_SIGINT ( int sig_no );

#endif

/*** end server.h ***/
