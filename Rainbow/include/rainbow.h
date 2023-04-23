#ifndef RAINBOW_H
#define RAINBOW_H 1

#define LOGGER_OPEN
#if 0
    #define HTTPVERBOSE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl.h>
#include <dlfcn.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "logger.h"
#include "b64.h"
#include "cJSON.h"
#include "sqlite3.h"

#define TYPE_POST           0xA
#define TYPE_GET            0xB
#define TYPE_PUT            0xC


#define NMEMB               16372
#define REQUEST_MAX         2

#define OK                  0
#define REQ_LEN_LOW         1<<1
#define REQ_LEN_HIGH        1<<2

#define RESP_OUT_MEM        1<<3

#define CURL_GLOBAL_FAILED  1<<4
#define CURL_EASY_FAILED    1<<5
#define CURL_LIST_FAILED    1<<6






typedef struct _request
{
    char *request[REQUEST_MAX];
    int  counter;
    size_t size;
} ReqX, *pReqX;

typedef struct _response
{
    char *response;
    size_t size;
} RespX, *pRespX;



size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userp);
size_t write_callback(void *data, size_t size, size_t nmemb, void *clientp);

int init_cURL();
void cURL_setopt(const char *url, char type, struct curl_slist    *slist);
void reset_cURL_ReqX(char *handle);
void reset_cURL_RespX();
void destory_cURL();
#endif
