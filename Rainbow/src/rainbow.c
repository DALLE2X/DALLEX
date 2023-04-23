#include "rainbow.h"



ReqX                g_ReqX;
RespX               g_RespX;

int                 ENABLE_HTTPS = 0;
char                *g_context = NULL;
CURL                *g_curl = NULL;
CURLcode            g_res = CURLE_OK;
struct curl_slist   *g_slist = NULL;

size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userp)
{
    printf("->read_callback\n");
    ReqX *pooh = (ReqX *)userp;
    const char *data;
    if(size * nmemb < 1)
    { return REQ_LEN_LOW; }
    data = pooh->request[pooh->counter];
    if(data)
    {
        size_t len = pooh->size;
        if(size * nmemb < len)
        {
            return REQ_LEN_HIGH;
        }
        memcpy(ptr, data, len);
        pooh->counter++; /* advance pointer */
        printf("->ptr:%s\n", ptr);
        return len;
    }
    return OK;
}
size_t write_callback(void *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    RespX *mem = (RespX *)clientp;
    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL)
    { return RESP_OUT_MEM; }  /* out of memory! */
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
    return realsize;
}


int init_cURL()
{
    if(curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    {
        return CURL_GLOBAL_FAILED;
    }
    g_curl = curl_easy_init();
    if(!g_curl)
    {
        curl_global_cleanup();
        return CURL_EASY_FAILED;
    }
    return OK;
}
void cURL_setopt(const char *url, char type, struct curl_slist    *slist)
{
    curl_easy_setopt(g_curl, CURLOPT_URL, url);
    if(curl_easy_setopt(g_curl, CURLOPT_SSL_VERIFYPEER, 0L) == CURLE_OK) { ENABLE_HTTPS = 1; }
    switch(type)
    {
        case TYPE_POST:
            curl_easy_setopt(g_curl, CURLOPT_POST, 1L);
            break;
        case TYPE_GET:
            curl_easy_setopt(g_curl, CURLOPT_HTTPGET, 1L);
            break;
        case TYPE_PUT:
            curl_easy_setopt(g_curl, CURLOPT_PUT, 1L);
            break;
        default:
            break;
    }
    curl_easy_setopt(g_curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(g_curl, CURLOPT_READDATA, (void *)&g_ReqX);
    curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(g_curl, CURLOPT_WRITEDATA, (void *)&g_RespX);
    #ifdef HTTPVERBOSE
    curl_easy_setopt(g_curl, CURLOPT_VERBOSE, 1L);
    #endif
    curl_easy_setopt(g_curl, CURLOPT_HTTPHEADER, slist);
}
void reset_cURL_ReqX(char *handle)
{
    if(g_ReqX.request[0])
    {
        free(g_ReqX.request[0]);
        g_ReqX.request[0] = NULL;
    }
    g_ReqX.counter = 0;
    g_ReqX.size = strlen(handle);
    g_ReqX.request[0] = (char *)malloc(g_ReqX.size);
    int i;
    for(i = 1; i < REQUEST_MAX; i++)
    { g_ReqX.request[i] = NULL; }
    memset(g_ReqX.request[0], 0, g_ReqX.size);
    memcpy(g_ReqX.request[0], handle, g_ReqX.size);
}
void reset_cURL_RespX()
{
    if(g_RespX.response)
    {
        free(g_RespX.response);
        g_RespX.response = NULL;
    }
    memset(&g_RespX, 0, sizeof(g_RespX));
}
void destory_cURL()
{
    curl_easy_cleanup(g_curl);
    curl_global_cleanup();
}
