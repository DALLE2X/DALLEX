#include "Ai.h"



const char *REST_APIs[] = {"https://api.stability.ai/v1/user/account",
                           "https://api.stability.ai/v1/user/balance",
                           "https://api.stability.ai/v1/engines/list",

                           NULL
                          };


extern ReqX                 g_ReqX;
extern RespX                g_RespX;

extern int                  ENABLE_HTTPS;
extern char                 *g_context;
extern CURL                 *g_curl;
extern CURLcode             g_res;
extern struct curl_slist    *g_slist;



void gettimestamp(char *timestamp)
{
    if(!timestamp)
    { return; }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    sprintf(timestamp, "%ld", tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
size_t tvusec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_usec;
}
char *hex2image(const char *base64, size_t size)
{
    unsigned char *image = NULL;
    size_t image_size = 0;
    image = b64_decode(base64, size, &image_size);
    log_debug(log_level, "image_size:%d", image_size);
    FILE *fp = NULL;
    char filename[18] = {0};
    gettimestamp(filename);
    sprintf(filename + 13, "%s", ".png");
    fp = fopen(filename, "wb");
    fwrite(image, 1, image_size, fp);
    fclose(fp);
    return image;
}


cJSON *str2json(const char *str)
{
    cJSON *json;
    json = cJSON_Parse(str);
    if(!json)
    {
        return NULL;
    }
    return json;
}
char *json2str(cJSON *json)
{
    char *str;
    str = cJSON_Print(json);
    if(!str)
    {
        return NULL;
    }
    return str;
}


int account()
{
    const char *list[] = {"Accept: application/json", "Authorization: Bearer sk-KnrxyliQ9NoDvWV9YkGE1B5hvCEQHpASPaSTC3xpF7OLafBe"};
    struct curl_slist   *slist = NULL;
    int i;
    for(i = 0; i < sizeof(list) / sizeof(list)[0]; i++)
    { slist = curl_slist_append(slist, list[i]); }
    if(!slist)
    {
        curl_easy_cleanup(g_curl);
        curl_global_cleanup();
        return CURL_LIST_FAILED;
    }
    cURL_setopt(REST_APIs[0], TYPE_GET, slist);
    reset_cURL_RespX();
    g_res = curl_easy_perform(g_curl);
    log_debug(log_level, "%s", g_RespX.response);
    cJSON *temp = str2json(g_RespX.response);
    json2sql(temp, ACCOUNT_INFO, 0);
    cJSON_Delete(temp);
    free(g_RespX.response);
    g_RespX.response = NULL;
    return OK;
}
int balance()
{
    const char *list[] = {"Content-Type: application/json", "Authorization: Bearer sk-KnrxyliQ9NoDvWV9YkGE1B5hvCEQHpASPaSTC3xpF7OLafBe"};
    struct curl_slist   *slist = NULL;
    int i;
    for(i = 0; i < sizeof(list) / sizeof(list)[0]; i++)
    { slist = curl_slist_append(slist, list[i]); }
    if(!slist)
    {
        curl_easy_cleanup(g_curl);
        curl_global_cleanup();
        return CURL_LIST_FAILED;
    }
    cURL_setopt(REST_APIs[1], TYPE_GET, slist);
    reset_cURL_RespX();
    g_res = curl_easy_perform(g_curl);
    log_debug(log_level, "%s", g_RespX.response);
    cJSON *temp = str2json(g_RespX.response);
    json2sql(temp, ACCOUNT_INFO, 0);
    cJSON_Delete(temp);
    free(g_RespX.response);
    g_RespX.response = NULL;
    return OK;
}

int engines()
{
    const char *list[] = {"Accept: application/json", "Authorization: Bearer sk-KnrxyliQ9NoDvWV9YkGE1B5hvCEQHpASPaSTC3xpF7OLafBe"};
    struct curl_slist   *slist = NULL;
    int i;
    for(i = 0; i < sizeof(list) / sizeof(list)[0]; i++)
    { slist = curl_slist_append(slist, list[i]); }
    if(!slist)
    {
        curl_easy_cleanup(g_curl);
        curl_global_cleanup();
        return CURL_LIST_FAILED;
    }
    cURL_setopt(REST_APIs[2], TYPE_GET, slist);
    reset_cURL_RespX();
    g_res = curl_easy_perform(g_curl);
    log_debug(log_level, "%s", g_RespX.response);
    cJSON *temp = str2json(g_RespX.response);
    json2sql(temp, ENGINES_MODEL, 0);
    cJSON_Delete(temp);
    free(g_RespX.response);
    g_RespX.response = NULL;
    return OK;
}
int text2image()
{
    const char *list[] = {"Transfer-Encoding: chunked", "Accept: application/json", "Content-Type: application/json", "Authorization: Bearer sk-KnrxyliQ9NoDvWV9YkGE1B5hvCEQHpASPaSTC3xpF7OLafBe"};
    struct curl_slist   *slist = NULL;
    int i;
    for(i = 0; i < sizeof(list) / sizeof(list)[0]; i++)
    { slist = curl_slist_append(slist, list[i]); }
    if(!slist)
    {
        curl_easy_cleanup(g_curl);
        curl_global_cleanup();
        return CURL_LIST_FAILED;
    }
    const char *engine_id = "https://api.stability.ai/v1/generation/stable-diffusion-768-v2-1/text-to-image";
    cURL_setopt(engine_id, TYPE_POST, slist);
    cJSON *tet2img = cJSON_CreateObject();
    cJSON *text_prompts = cJSON_CreateArray();
    cJSON *item = cJSON_CreateObject();
    cJSON_AddStringToObject(item, "text", "Whose smile will be transformed into the light of tomorrow, love, even if separated by thousands of miles, pride, but also still believe in the limited encounter, pulling us in, tied together in the red thread, whether in the dark night or in the stormy morning, only you are the only real.");
    //cJSON_AddNumberToObject(item,"weight",0.5);
    cJSON_AddItemToArray(text_prompts, item);
    cJSON_AddItemToObject(tet2img, "text_prompts", text_prompts);
    cJSON_AddNumberToObject(tet2img, "height", 512);
    cJSON_AddNumberToObject(tet2img, "width", 1152);
    cJSON_AddNumberToObject(tet2img, "cfg_scale", 18); //[0,35] default 7
    cJSON_AddNumberToObject(tet2img, "steps", 150);
    char *temp = json2str(tet2img);
    reset_cURL_ReqX(temp);
    reset_cURL_RespX();
    g_res = curl_easy_perform(g_curl);
    //log_debug(log_level,"%s",g_RespX.response);
    cJSON *resp = str2json(g_RespX.response);
    if(cJSON_HasObjectItem(resp, "artifacts"))
    {
        cJSON *artifacts = cJSON_GetObjectItem(resp, "artifacts");
        cJSON *img2base64 = cJSON_GetArrayItem(artifacts, 0);
        char *result = cJSON_GetObjectItem(img2base64, "base64")->valuestring;
        unsigned char *image = NULL;
        size_t image_size = 0;
        image = b64_decode(result, strlen(result), &image_size);
        log_debug(log_level, "image_size:%d", image_size);
        FILE *fp2 = NULL;
        char filename1[18] = {0};
        gettimestamp(filename1);
        sprintf(filename1 + 13, "%s", ".png");
        fp2 = fopen(filename1, "wb");
        fwrite(image, 1, image_size, fp2);
        fclose(fp2);
        FILE *fp1 = NULL;
        sprintf(filename1 + 13, "%s", ".txt");
        fp1 = fopen(filename1, "wb");
        fwrite(result, 1, strlen(result), fp1);
        fclose(fp1);
        free(image);
    }
    cJSON_Delete(resp);
    FILE *fp = NULL;
    char filename[18] = {0};
    gettimestamp(filename);
    sprintf(filename + 13, "%s", ".json");
    fp = fopen(filename, "wb");
    fwrite(g_RespX.response, 1, strlen(g_RespX.response), fp);
    fclose(fp);
    free(temp);
    free(g_RespX.response);
    temp = NULL;
    g_RespX.response = NULL;
}
void *json2sql(cJSON *json, int tableNum, char **outsql)
{
    if(!json)
    {
        return NULL;
    }
    static int initSQL = 0;
    static char *outsqlstr = NULL;
    if(!initSQL)
    {
        outsqlstr = (char *)malloc(OUTSQL_LEN);
        log_debug(log_level, "->initSQL:%p", outsqlstr);
        initSQL = 1;
    }
    int         index_table1[] = {EMAIL, USER_ID, CREDITS, ORGANIZATIONS_ID, IS_DEFAULT, ORGANIZATIONS_NAME, ROLE, PROFILE_PICTURE};
    const char *iname1[] = {"EMAIL", "USER_ID", "CREDITS", "ORGANIZATIONS_ID", "IS_DEFAULT", "ORGANIZATIONS_NAME", "ROLE", "PROFILE_PICTURE"};
    const char *account_cols[] = {"email", "id", "credits", "organizations", "is_default", "name", "role", "profile_picture"};
    static int flag[2] = {0}, flag1 = 0;
    switch(tableNum)
    {
        case 0x1:
            if(cJSON_HasObjectItem(json, "credits"))
            {
                cJSON *item = cJSON_GetObjectItem(json, "credits");
                double  str = item->valuedouble;
                ppMapx[CREDITS]->index = CREDITS;
                memcpy(ppMapx[CREDITS]->name, &str, sizeof(str));
                log_debug(log_level, "CREDITS:%d,%f", ppMapx[CREDITS]->index, *(double *)ppMapx[CREDITS]->name);
                flag[0] = 1;
            }
            if(cJSON_HasObjectItem(json, "email"))
            {
                cJSON *item = cJSON_GetObjectItem(json, "email");
                char  *str = item->valuestring;
                ppMapx[EMAIL]->index = EMAIL;
                memcpy(ppMapx[EMAIL]->name, str, strlen(str));
                log_debug(log_level, "EMAIL:%d,%s", ppMapx[EMAIL]->index, ppMapx[EMAIL]->name);
            }
            if(cJSON_HasObjectItem(json, "id"))
            {
                cJSON *item = cJSON_GetObjectItem(json, "id");
                char  *str = item->valuestring;
                ppMapx[USER_ID]->index = USER_ID;
                memcpy(ppMapx[USER_ID]->name, str, strlen(str));
                log_debug(log_level, "USER_ID:%d,%s", ppMapx[USER_ID]->index, ppMapx[USER_ID]->name);
            }
            if(cJSON_HasObjectItem(json, "organizations"))
            {
                cJSON *item = cJSON_GetObjectItem(json, "organizations");
                int array_size = 1;
                array_size = cJSON_GetArraySize(item);
                log_debug(log_level, "array_size:%d", array_size);
                int index;
                cJSON *elem[1] = {NULL};
                for(index = 0; index < array_size; index++)
                {
                    elem[index] = cJSON_GetArrayItem(item, index);
                }
                if(elem[0])
                {
                    if(cJSON_HasObjectItem(elem[0], "id"))
                    {
                        cJSON *item = cJSON_GetObjectItem(elem[0], "id");
                        char  *str = item->valuestring;
                        ppMapx[ORGANIZATIONS_ID]->index = ORGANIZATIONS_ID;
                        memcpy(ppMapx[ORGANIZATIONS_ID]->name, str, strlen(str));
                        log_debug(log_level, "ORGANIZATIONS_ID:%d,%s", ppMapx[ORGANIZATIONS_ID]->index, ppMapx[ORGANIZATIONS_ID]->name);
                    }
                    if(cJSON_HasObjectItem(elem[0], "is_default"))
                    {
                        cJSON *item = cJSON_GetObjectItem(elem[0], "is_default");
                        int  str = item->valueint;
                        ppMapx[IS_DEFAULT]->index = IS_DEFAULT;
                        memcpy(ppMapx[IS_DEFAULT]->name, &str, sizeof(str));
                        log_debug(log_level, "IS_DEFAULT:%d,%d", ppMapx[IS_DEFAULT]->index, *(int *)ppMapx[IS_DEFAULT]->name);
                    }
                    if(cJSON_HasObjectItem(elem[0], "name"))
                    {
                        cJSON *item = cJSON_GetObjectItem(elem[0], "name");
                        char  *str = item->valuestring;
                        ppMapx[ORGANIZATIONS_NAME]->index = ORGANIZATIONS_NAME;
                        memcpy(ppMapx[ORGANIZATIONS_NAME]->name, str, strlen(str));
                        log_debug(log_level, "ORGANIZATIONS_NAME:%d,%s", ppMapx[ORGANIZATIONS_NAME]->index, ppMapx[ORGANIZATIONS_NAME]->name);
                    }
                    if(cJSON_HasObjectItem(elem[0], "role"))
                    {
                        cJSON *item = cJSON_GetObjectItem(elem[0], "role");
                        char  *str = item->valuestring;
                        ppMapx[ROLE]->index = ROLE;
                        memcpy(ppMapx[ROLE]->name, str, strlen(str));
                        log_debug(log_level, "ROLE:%d,%s", ppMapx[ROLE]->index, ppMapx[ROLE]->name);
                    }
                }
            }
            if(cJSON_HasObjectItem(json, "profile_picture"))
            {
                cJSON *item = cJSON_GetObjectItem(json, "profile_picture");
                char  *str = item->valuestring;
                ppMapx[PROFILE_PICTURE]->index = PROFILE_PICTURE;
                memcpy(ppMapx[PROFILE_PICTURE]->name, str, strlen(str));
                flag[1] = 1;
                log_debug(log_level, "PROFILE_PICTURE:%d,%s", ppMapx[PROFILE_PICTURE]->index, ppMapx[PROFILE_PICTURE]->name);
            }
            if(flag[0] && flag[1])
            {
                memset(outsqlstr, 0, OUTSQL_LEN);
                sprintf(outsqlstr, "insert into account_info (email,user_id,credits,organizations_id,is_default,organizations_name,role,profile_picture) values(\"%s\",\"%s\",%f,\"%s\",%d,\"%s\",\"%s\",\"%s\");",
                        ppMapx[index_table1[0]]->name, ppMapx[index_table1[1]]->name, *(double *)ppMapx[index_table1[2]]->name,
                        ppMapx[index_table1[3]]->name, *(int *)ppMapx[index_table1[4]]->name, ppMapx[index_table1[5]]->name,
                        ppMapx[index_table1[6]]->name, ppMapx[index_table1[7]]->name);
                log_debug(log_level, "->OUTSQL:%s", outsqlstr);
                int rc = insert_table(outsqlstr);
                if(rc != SQLITE_OK)
                {
                    memset(outsqlstr, 0, OUTSQL_LEN);
                    sprintf(outsqlstr, "UPDATE account_info SET email = '%s',user_id = '%s',credits = %f,organizations_id = '%s',is_default = %d,organizations_name = '%s',role = '%s',profile_picture = '%s' WHERE user_id = '%s';",
                            ppMapx[index_table1[0]]->name, ppMapx[index_table1[1]]->name, *(double *)ppMapx[index_table1[2]]->name,
                            ppMapx[index_table1[3]]->name, *(int *)ppMapx[index_table1[4]]->name, ppMapx[index_table1[5]]->name,
                            ppMapx[index_table1[6]]->name, ppMapx[index_table1[7]]->name, ppMapx[index_table1[1]]->name);
                    log_debug(log_level, "->OUTSQL:%s", outsqlstr);
                    upadte_table(outsqlstr);
                }
                flag[0] = 0;
                flag[1] = 0;
            }
            break;
        case 0x2:
            if(cJSON_IsArray(json))
            {
                int array_size;
                array_size = cJSON_GetArraySize(json);
                int index_table[] = {DESCRIPTION_ENGINE, ID_ENGINE, NAME_ENGINE, TYPE_ENGINE};
                const char *iname[] = {"DESCRIPTION_ENGINE", "ID_ENGINE", "NAME_ENGINE", "TYPE_ENGINE"};
                const char *engines_name[] = {"description", "id", "name", "type"};
                log_debug(log_level, "array_size:%d,itable:%d", array_size, sizeof(index_table) / sizeof(int));
                int index, index2;;
                cJSON *elem, *item;
                int count = 0;
                for(index = 0; index < array_size; index++)
                {
                    elem = cJSON_GetArrayItem(json, index);
                    for(index2 = 0; index2 < sizeof(index_table) / sizeof(int); index2++)
                    {
                        item = cJSON_GetObjectItem(elem, engines_name[index2]);
                        char  *str = item->valuestring;
                        ppMapx[index_table[index2]]->index = index_table[index2];
                        memcpy(ppMapx[index_table[index2]]->name, str, strlen(str));
                        //log_debug(log_level,"No.%d->%s:%d,%s",count++%4+1,iname[index2],ppMapx[index_table[index2]]->index,ppMapx[index_table[index2]]->name);
                    }
                    memset(outsqlstr, 0, OUTSQL_LEN);
                    sprintf(outsqlstr, "insert into engines_model (description,id,name,type) values(\"%s\",\"%s\",\"%s\",\"%s\")",
                            ppMapx[index_table[0]]->name, ppMapx[index_table[1]]->name,
                            ppMapx[index_table[2]]->name, ppMapx[index_table[3]]->name);
                    log_debug(log_level, "->OUTSQL:%s", outsqlstr);
                    insert_table(outsqlstr);
                }
            }
            break;
        default :
            break;
    }
    return NULL;
}

void create_TableX(TableX **head)
{
    if(!head)
    {
        log_warn(log_level, "head==NULL,can not create.");
        return;
    }
    (*head) = (TableX *)malloc(sizeof(TableX));
    (*head)->node = NULL;
    (*head)->next = NULL;
    log_info(log_level, "create_TableX tables:%p", *head);
}
void create_RecordX(RecordX **head)
{
    if(!head)
    {
        log_warn(log_level, "head==NULL,can not create.");
        return;
    }
    (*head) = (RecordX *)malloc(sizeof(RecordX));
    (*head)->next = NULL;
    log_debug(log_level, "create_RecordX head:%p", *head);
}

void gen_record(RecordX *head, RecordX *col)
{
    if(!head)
    {
        log_warn(log_level, "gen_record failed.");
        return;
    }
    RecordX *begin = head;
    RecordX *end = head;
    RecordX *next = (RecordX *)malloc(sizeof(RecordX));
    col->next = NULL;
    while(begin)
    {
        end = begin;
        begin = begin->next;
    }
    memcpy(end, col, sizeof(RecordX));
    end->next = next;
}
void add_record(TableX *head, RecordX *record, TableX **newer)
{
    if(!record || !head)
    {
        log_warn(log_level, "add_record failed.");
        return;
    }
    TableX *begin = head;
    TableX *end = head;
    TableX *next = (TableX *)malloc(sizeof(TableX));
    next->node = NULL;
    next->next = NULL;
    while(begin)
    {
        end = begin;
        begin = begin->next;
    }
    end->next = next;
    end->node = record;
    head->total++;
    *newer = end;
    log_debug(log_level, "record name-value:%s,%s", record->colName, record->colValue);
}




void init_MapX()
{
    if(ppMapx[0] == NULL)
    {
        int i;
        int size = sizeof(MapX);
        for(i = 0; i < ENUM_ACCOUNT_LEN; i++)
        {
            ppMapx[i] = (pMapX)malloc(size);
            memset(ppMapx[i], 0, size);
        }
    }
}
int insert_table(const char *outsqlstr)
{
    int rc;
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, outsqlstr, NULL, NULL, &zErrMsg);
    if(rc != SQLITE_OK)
    {
        log_err(log_level, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    }
    else
    {
        log_debug(log_level, "insert into table succ!");
        return SQLITE_OK;
    }
}
int upadte_table(const char *outsqlstr)
{
    int rc;
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, outsqlstr, NULL, NULL, &zErrMsg);
    if(rc != SQLITE_OK)
    {
        log_err(log_level, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    }
    else
    {
        log_debug(log_level, "update table succ!");
        return SQLITE_OK;
    }
}
int select_table(const char *outsqlstr)
{
    int rc;
    char *zErrMsg = 0;
    TableX *tables;
    create_TableX(&tables);
    rc = sqlite3_exec(db, outsqlstr, callback, (void *)tables, &zErrMsg);
    log_info(log_level, "record total:%d", tables->total);
    if(rc != SQLITE_OK)
    {
        log_err(log_level, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    }
    else
    {
        log_debug(log_level, "select table succ!");
        show_table(tables);
        destory_TableX(tables);
        return SQLITE_OK;
    }
}
void destory_TableX(TableX *head)
{
    static int n = 1;
	TableX* temp2=head;
    if(head)
    {
        RecordX *record;
        TableX *temp1;
        while(head->node)
        {
            record = head->node;
            RecordX *temp;
            while(record->next)
            {
                temp = record;
                record = record->next;
                log_debug(log_level, "%d->delete poniter:%p", n++, temp);
                free(temp);
                temp = NULL;
            }
	    free(record);
            temp1 = head;
            head = head->next;
            free(temp1);
            temp1 = NULL;
        }
    }
	log_info(log_level,"destory_TableX ->tables:%p",temp2);
}

void show_table(TableX *head)
{
    if(head)
    {
        RecordX *record;
        log_debug(log_level, "Show all records of a table");
        while(head->node)
        {
            record = head->node;
            while(record->next)
            {
                log_debug(log_level, "%s<->%s\t", record->colName, record->colValue);
                record = record->next;
            }
            head = head->next;
        }
    }
}

static int callback(void *tables, int argc, char **argv, char **azColName)
{

	log_debug(log_level,"ready insert a record of table:%p",(TableX*)tables);
    int i;
    log_debug(log_level, "argc:%d", argc);
    RecordX col;
    RecordX *record;
    create_RecordX(&record);
    for(i = 0; i < argc; i++)
    {
        char *ColName = azColName[i];
        char *ColValue = argv[i] ? argv[i] : NULL;
        int  colValueSzie = strlen(ColValue);
        int  colNameSize = strlen(ColName);
        log_debug(log_level, "colNameSize:%d,colValueSzie:%d", colNameSize, colValueSzie);
        log_debug(log_level, "%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
        col.colName = (char *)malloc(colNameSize);
        col.colValue = (char *)malloc(colValueSzie);
        memcpy(col.colName, ColName, strlen(ColName));
        if(ColValue)
        {
            memcpy(col.colValue, ColValue, strlen(ColValue));
        }
        log_debug(log_level, "gen_record to record:%p", record);
        gen_record(record, &col);
    }
    TableX *newer;
    add_record((TableX *)tables, record, &newer);
    return 0;
}

int main()
{
    log_level = LOG_LEVEL_INFO;
    init_cURL();
    init_MapX();
    int rc;
    rc = sqlite3_open("dreamAi", &db);
	/*char *zErrMsg = 0;
	char sql[]="create table engines_model(ID_INDEX INTEGER PRIMARY KEY AUTOINCREMENT,description text not null,id text not null UNIQUE,name text not null,type text not null);";
	rc=sqlite3_exec(db,sql,NULL,NULL, &zErrMsg);*/

//	engines();
if(1){
    select_table("select  * from engines_model;");
	
	usleep(1000*1000);

	sqlite3_close(db);
}
    //  testX();
    return 0;
    //  sqlite3T();
    //balance();
    //account();
    //  engines();
    //text2image();
    
    return 0;
}
