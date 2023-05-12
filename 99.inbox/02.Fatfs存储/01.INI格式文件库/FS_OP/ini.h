/*1 ini.h
 *#	Header file for the {*INI*} parser.\n
 *#
 *#	This is a simple parser for {*.INI*} files.
 *#	It is based around the syntax described in the Wikipedia entry at
 *#	{_http://en.wikipedia.org/wiki/INI_file_}\n
 *#
 *# The API has these features:
 *{
 ** Use {{~~ini_read()}} to read an INI file from the disc into an {{~~ini_file}}
 *#   structure or create an empty {{~~ini_file}} structure.
 ** Use {{~~ini_get()}} to retrieve values from the {{~~ini_file}} structure.
 ** Use {{~~ini_put()}} and {{~~ini_putf()}} to set values in the {{~~ini_file}} structure.
 ** Use {{~~ini_write()}} to write the contents of the {{~~ini_file}} structure back to disc.
 ** Use {{~~ini_free()}} to deallocate the {{~~ini_file}} structure.
 ** {{~~ini_errstr()}} is used for reporting errors.
 *}
 *2 License
 *[
 *# Author: Werner Stoop
 *# This software is provided under the terms of the unlicense.
 *# See http://unlicense.org/ for more details.
 *]
 *2 API
 */
#ifndef INI_H
#define INI_H

#if defined(__cplusplus)
extern "C" {
#endif

/*
 *	Encapsulates a parameter-value pair in an INI section.
 *	Parameters are stored in a binary search tree, which attempts (but does not
 *	guarantee) O(log(n)) behaviour.
 */
typedef struct INI_PAIR {
    char* param; /* The parameter */
    char* value; /* Its value */

    /* Nodes in the tree */
    struct INI_PAIR *left, *right;
} ini_pair;

/*
 *	Encapsulates a section within a INI file.
 *	Sections are stored in a binary search tree, which attempts (but does not
 *	guarantee) O(log(n)) behaviour.
 */
typedef struct INI_SECTION {
    char*     name;   /* Name of the section */
    ini_pair* fields; /* Fields in the section */

    /* Nodes in the tree */
    struct INI_SECTION *left, *right;
} ini_section;

/*@ struct ##ini_file;
 *#	Structure to encapsulate an INI file.
 */
struct ini_file {
    ini_pair*    globals;
    ini_section* sections;
};

/*@ ini_file *##ini_read(const char *filename, int *err, int *line);
 *#	Reads an INI file named by {{filename}} and returns it as a {{~~ini_file}} object.
 *#	If {{filename}} is {{NULL}}, an empty {{ini_file}} object is created and returned.
 *#	Comments are discarded, so a later call to {{~~ini_write()}} will be
 *#	commentless.\n
 *# It returns {{NULL}} if the INI file couldn't be read, in which case {{err}} will contain the error code
 *# (see {{~~ini_errstr()}}) and {{line}} will contain the line in the file where the error occured.
 *# ({{err}} and {{line}} may be {{NULL}}).
 */
struct ini_file* ini_read(const char* filename, int* err, int* line);

/*@ ini_file *##ini_parse(const char *text, int *err, int *line);
 *# Parses a null-terminated string {{text}} as an INI file.\n
 *# It returns {{NULL}} if the INI file couldn't be read, in which case {{err}} will contain the error code
 *# (see {{~~ini_errstr()}}) and {{line}} will contain the line in the file where the error occured.
 *# ({{err}} and {{line}} may be {{NULL}}).
 */
struct ini_file* ini_parse(const char* text, int* err, int* line);

/*@ int ##ini_write(ini_file *ini, const char *fname);
 *#	Saves all the sections and parameters in an {{ini_file}} to a file.\n
 *#	{{fname}} is the file to which to save. If {{fname}} is {{NULL}}, it is written to
 *#	{{stdout}}.
 *# It returns 1 on success, an error code otherwise (see {{~~ini_errstr()}}).
 */
int ini_write(struct ini_file* ini, const char* fname);

/*@ void ##ini_free(ini_file *ini);
 *#	Free's all the memory allocated to a {{ini_file}} object created in {{~~ini_read()}}.\n
 *#	{{ini}} is the {{ini_file}} to free
 */
void ini_free(struct ini_file* ini);

/*@ int ini_has_section(struct ini_file *ini, const char *sec)
 *# Returns true if the ini file has a specific section.
 */
int ini_has_section(struct ini_file* ini, const char* sec);

/*@ const char *##ini_get(struct ini_file *ini, const char *sec, const char *par, const char *def);
 *#	Retrieves a parameter {{par}} from a section {{sec}} within the struct ini_file {{ini}}
 *#	and returns its value.\n
 *#	If {{sec}} is {{NULL}}, the global parameters in {{ini}} are searched.\n
 *#	If the value is not found, the default {{def}}, which may be {{NULL}}, is
 *#	returned.
 */
const char* ini_get(struct ini_file* ini,
                    const char*      sec,
                    const char*      par,
                    const char*      def);

/*@ int ##ini_put(struct ini_file *ini, const char *sec, const char *par, const char *val);
 *#	Sets a parameter {{par}} in section {{sec}}'s value to {{val}}, replacing the
 *#	current value if it already exists, or creates the section if it does not
 *#	exist.\n
 *#	If {{sec}} is {{NULL}}, the parameter is added to {{ini}}'s global parameters.\n
 *# It returns 1 on success, 0 on failure (which only happens if {{malloc()}} fails).
 */
int ini_put(struct ini_file* ini, const char* sec, const char* par, const char* val);

/*@ int ##ini_putf(struct ini_file *ini, const char *sec, const char *par, const char *fmt, ...);
 *#	{{~~ini_putf()}} takes a {{printf()}} style format string and uses vsnprintf() to
 *#	pass a value to {{~~ini_put()}}. This function is intended for placing
 *#	data types that are not strings into the {{ini_file}}
 *#
 *#	The other parameters are the same as those of {{ini_put()}}.
 */
int ini_putf(struct ini_file* ini,
             const char*      sec,
             const char*      par,
             const char*      fmt,
             ...);

/*@ const char *##ini_errstr(int err)
 *# Returns a textual description of an error code
 */
const char* ini_errstr(int err);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* INI_H */
