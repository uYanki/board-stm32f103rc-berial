#include "board.h"
#include "cJSON.h"

#define TAG     "cjson"
#define DBG_LVL SDK_DEBUG

int main(void)
{
    board_init();

    {
        char* out;
        int   size;

        cJSON* objTest;
        cJSON* objArray;
        cJSON* objItem1;
        cJSON* objItem2;

        // new object
        objTest = cJSON_CreateObject();

        // add string
        cJSON_AddStringToObject(objTest, "Title", "CJSON Test");
        cJSON_AddStringToObject(objTest, "name", "objTest");

        // add bool
        cJSON_AddBoolToObject(objTest, "v1", 1);
        cJSON_AddBoolToObject(objTest, "v2", 0);

        // add number
        cJSON_AddNumberToObject(objTest, "v3", 1);
        cJSON_AddNumberToObject(objTest, "v4", 2);
        cJSON_AddNumberToObject(objTest, "v5", 1.55);

        // new object
        objItem1 = cJSON_CreateObject();
        objItem2 = cJSON_CreateObject();

        // add string
        cJSON_AddStringToObject(objItem1, "name", "objItem1");
        cJSON_AddStringToObject(objItem2, "name", "objItem2");

        // add array
        objArray = cJSON_CreateArray();
        cJSON_AddItemToArray(objArray, objItem1);
        cJSON_AddItemToArray(objArray, objItem2);

        // add object
        cJSON_AddItemToObject(objTest, "Children1", objArray);

        // format (格式化)
        out = cJSON_Print(objTest);

        // print
        println("%s", out);

        // delete (顶层删除自动删除底层)
        cJSON_Delete(objTest);

        // parse
        objTest  = cJSON_Parse(out);
        objArray = cJSON_GetObjectItem(objTest, "Children1");

        size     = cJSON_GetArraySize(objArray);
        println("objArray size : %d", size);

        // get value
        objItem1 = cJSON_GetObjectItem(objTest, "v1");
        println("objTest v1: %d,%f", objItem1->valueint, objItem1->valuedouble);
        objItem1 = cJSON_GetObjectItem(objTest, "v2");
        println("objTest v2: %d,%f", objItem1->valueint, objItem1->valuedouble);
        objItem1 = cJSON_GetObjectItem(objTest, "v3");
        println("objTest v3: %d,%f", objItem1->valueint, objItem1->valuedouble);
        objItem1 = cJSON_GetObjectItem(objTest, "v4");
        println("objTest v4: %d,%f", objItem1->valueint, objItem1->valuedouble);
        objItem1 = cJSON_GetObjectItem(objTest, "v5");
        println("objTest v5: %d,%f", objItem1->valueint, objItem1->valuedouble);

        // get subobj
        objItem1 = cJSON_GetArrayItem(objArray, 0);
        objItem2 = cJSON_GetArrayItem(objArray, 1);

        // print
        objItem1 = cJSON_GetObjectItem(objItem1, "name");
        println("objItem1 name : %s", objItem1->valuestring);
        objItem1 = cJSON_GetObjectItem(objItem2, "name");
        println("objItem2 name : %s", objItem1->valuestring);

        // free
        cJSON_free(out);

        // delete
        cJSON_Delete(objTest);
    }

    while (1) {
    }
}
