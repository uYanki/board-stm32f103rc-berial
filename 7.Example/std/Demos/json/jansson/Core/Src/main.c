#include "board.h"

#include "jansson.h"

#define TAG     "jansson"
#define DBG_LVL SDK_DEBUG

void add_2darray_to_json(json_t* obj, const char* name, const int* marr, size_t dim1, size_t dim2)
{
    size_t  i, j;
    json_t* jarr1 = json_array();

    for (i = 0; i < dim1; ++i) {
        json_t* jarr2 = json_array();

        for (j = 0; j < dim2; ++j) {
            int     val  = marr[i * dim2 + j];
            json_t* jval = json_integer(val);
            json_array_append_new(jarr2, jval);
        }
        json_array_append_new(jarr1, jarr2);
    }
    json_object_set_new(obj, name, jarr1);
    return;
}

int main(void)
{
    json_t* root;
    json_t* jdata;

    json_t* test_obj;
    json_t* Array_obj;
    json_t* item1_obj;
    json_t* item2_obj;

    json_t* test_obj1;
    json_t* Array_obj1;
    json_t* item1_obj1;
    json_t* item2_obj1;

    char* out;
    char* s;
    int   arr1[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };
    int arr2[4][4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    char buffer[4] = {'t', 'e', 's', 't'};

    int size;

    json_error_t error;

    board_init();

    /** json_pack(fmt,...)
     * @param fmt
     * -s: string
     * -i: integer
     * -b: bool
     */
    root = json_pack("{}");

    /** json_dumps()
     * @brief json->str
     */
    out  = json_dumps(root, JSON_ENCODE_ANY);
    println("out:%s", out);
    free(root);
    free(out);

    /* Build the JSON object {"foo": 42, "bar": 7} */
    root = json_pack("{sisi}", "foo", 42, "bar", 7);
    out  = json_dumps(root, JSON_ENCODE_ANY);
    println("out:%s", out);
    free(root);
    free(out);

    /* Like above, ':', ',' and whitespace are ignored */
    root = json_pack("{s:i, s:i}", "foo", 42, "bar", 7);
    out  = json_dumps(root, JSON_ENCODE_ANY);
    println("out:%s", out);
    free(root);
    free(out);

    /* Build the JSON array [[1, 2], {"cool": true}] */
    root = json_pack("[[i,i],{s:b}]", 1, 2, "cool", 1);
    out  = json_dumps(root, JSON_ENCODE_ANY);
    println("out:%s", out);
    free(root);
    free(out);

    /* Build a string from a non-null terminated buffer */
    root = json_pack("s#", buffer, 4);
    out  = json_dumps(root, JSON_ENCODE_ANY);
    println("out:%s", out);
    free(root);
    free(out);

    /* Concatenate strings together to build the JSON string "foobarbaz" */
    root = json_pack("s++", "foo", "bar", "baz");
    out  = json_dumps(root, JSON_ENCODE_ANY);
    println("out:%s", out);
    free(root);
    free(out);

    /* Create an empty object or array when optional members are missing */
    root = json_pack("{s:s*,s:o*,s:O*}", "foo", NULL, "bar", NULL, "baz", NULL);
    out  = json_dumps(root, JSON_ENCODE_ANY);
    println("out:%s", out);
    free(root);
    free(out);

    root = json_pack("[s*,o*,O*]", NULL, NULL, NULL);
    out  = json_dumps(root, JSON_ENCODE_ANY);
    println("out:%s", out);
    free(root);
    free(out);

    jdata = json_object();
    add_2darray_to_json(jdata, "arr1", &arr1[0][0], 2, 3);
    add_2darray_to_json(jdata, "arr2", &arr2[0][0], 4, 4);
    s = json_dumps(jdata, 0);
    println("out:%s", s);
    free(s);

    // decrease ref 减少引用计数(引用计数降为零时自动销毁)
    json_decref(jdata);

    // destroy
    json_decref(jdata);

    println("----------------");

    // new object
    test_obj  = json_object();
    item1_obj = json_object();
    item2_obj = json_object();

    // string
    json_object_set_new(test_obj, "Title", json_string("jansson test"));
    json_object_set_new(test_obj, "name", json_string("test_obj"));

    // bool
    json_object_set_new(test_obj, "v1", json_true());
    json_object_set_new(test_obj, "v2", json_false());

    // number
    json_object_set_new(test_obj, "v3", json_integer(1));
    json_object_set_new(test_obj, "v4", json_integer(2));
    json_object_set_new(test_obj, "v5", json_real(1.65));

    // string
    json_object_set_new(item1_obj, "name", json_string("item1_obj"));
    json_object_set_new(item2_obj, "name", json_string("item2_obj"));

    // new array
    Array_obj = json_array();

    // add elements
    json_array_append_new(Array_obj, item1_obj);
    json_array_append_new(Array_obj, item2_obj);

    // add child
    json_object_set_new(test_obj, "Children1", Array_obj);

    // format to string
    out = json_dumps(test_obj, JSON_ENCODE_ANY);

    // print
    println("out:%s", out);

    // remove top will auto remove child
    json_decref(test_obj);

    // parse JSON
    test_obj1  = json_loads(out, 0, &error);
    Array_obj1 = json_object_get(test_obj1, "Children1");
    size       = json_array_size(Array_obj1);
    println("Array_obj1 size : %d", size);

    //
    item1_obj = json_object_get(test_obj1, "v1");
    println("test_obj1 v1 type is %d: val:  %d,%f", json_typeof(item1_obj), json_boolean_value(item1_obj), json_number_value(item1_obj));
    item1_obj = json_object_get(test_obj1, "v2");
    println("test_obj1 v2 type is %d: val:  %d,%f", json_typeof(item1_obj), json_boolean_value(item1_obj), json_number_value(item1_obj));
    item1_obj = json_object_get(test_obj1, "v3");
    println("test_obj1 v3 type is %d: val:  %lld,%f", json_typeof(item1_obj), json_integer_value(item1_obj), json_number_value(item1_obj));
    item1_obj = json_object_get(test_obj1, "v4");
    println("test_obj1 v4 type is %d: val:  %lld,%f", json_typeof(item1_obj), json_integer_value(item1_obj), json_number_value(item1_obj));
    item1_obj = json_object_get(test_obj1, "v5");
    println("test_obj1 v5 type is %d: val:  %f,%f", json_typeof(item1_obj), json_real_value(item1_obj), json_number_value(item1_obj));

    // get child
    item1_obj1 = json_array_get(Array_obj1, 0);
    item2_obj1 = json_array_get(Array_obj1, 1);

    // print
    item1_obj  = json_object_get(item1_obj1, "name");
    println("item1_obj1 name : %s", json_string_value(item1_obj));
    item1_obj = json_object_get(item2_obj1, "name");
    println("item2_obj1 name : %s", json_string_value(item1_obj));

    // free
    free(out);

    // destroy (不建议直接用 json_delete)
    json_decref(test_obj1);

    while (1) {}

}
