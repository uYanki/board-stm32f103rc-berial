#ifndef XML_H
#define XML_H

typedef struct xml_emt xml_emt_t; /* emt := EleMenT */

#ifdef __cplusplus
extern "C" {
#endif

xml_emt_t* xml_new(const char* root_name, const char* root_value);
xml_emt_t* xml_import(const char* xml_buf);
xml_emt_t* xml_import_from_file(const char* xml_file);
int        xml_export(xml_emt_t* pxml, char* buf, int max_size);
int        xml_export_to_file(xml_emt_t* pxml, const char* xml_file);
void       xml_destroy(xml_emt_t* pxml);

const char* xml_select(xml_emt_t* pxml, const char* xpath);
const char* xml_select_n(xml_emt_t* pxml, const char* xpath, int n);
int         xml_update(xml_emt_t* pxml, const char* xpath, const char* value);
int         xml_delete(xml_emt_t* pxml, const char* xpath);
int         xml_insert(xml_emt_t* pxml, const char* xpath, const char* value);

const char* xml_attr_select(xml_emt_t* pxml, const char* xpath, const char* attr_name);
const char* xml_attr_select_n(xml_emt_t* pxml, const char* xpath, const char* attr_name, int n);
int         xml_attr_update(xml_emt_t* pxml, const char* xpath, const char* attr_name, const char* value);
int         xml_attr_delete(xml_emt_t* pxml, const char* xpath, const char* attr_name);
int         xml_attr_insert(xml_emt_t* pxml, const char* xpath, const char* attr_name, const char* value);

#ifdef __cplusplus
}
#endif
#endif
