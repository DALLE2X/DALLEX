#ifndef AI_H
#define AI_H 1




#include "rainbow.h"

#define ENUM_ACCOUNT_LEN        14
#define MAP_LEN                 128


#define ID_INDEX                0
#define EMAIL                   1
#define USER_ID                 2
#define CREDITS                 3
#define ORGANIZATIONS_ID        4
#define IS_DEFAULT              5
#define ORGANIZATIONS_NAME      6
#define ROLE                    7
#define PROFILE_PICTURE         8

#define ID_INDEX_ENGINE         9 //only on database
#define DESCRIPTION_ENGINE      10
#define ID_ENGINE               11
#define NAME_ENGINE             12
#define TYPE_ENGINE             13



#define OUTSQL_LEN              1024*1024*4
#define ACCOUNT_INFO            0x1
#define ENGINES_MODEL           0x2

#define COLNAME_LEN             128

enum LOG_LEVEL log_level;


sqlite3 *db;

typedef struct _recordx
{

    size_t count;
    //char colName[COLNAME_LEN];
    char *colName;
    char *colValue;
    struct _recordx *next;
} RecordX, *pRecordX;

typedef struct _tablex
{

    int total;
    pRecordX node;
    struct _tablex *next;
} TableX, *pTableX;

typedef struct _record2table
{

    TableX *table;
    RecordX *record;
} Record2Table, *pRecord2Table;
typedef struct _mapx
{

    int index;
    char name[MAP_LEN];
} MapX, *pMapX;

pMapX ppMapx[ENUM_ACCOUNT_LEN] = {NULL};

static int callback(void *records, int argc, char **argv, char **azColName);

void *json2sql(cJSON *json, int tableNum, char **outsql);
void init_MapX();
void create_TableX(TableX **);
void destory_TableX(TableX *);
void show_table(TableX *head);

#endif
