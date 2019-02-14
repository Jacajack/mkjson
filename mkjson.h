/* mkjson.h - a part of mkjson library
 *
 * Copyright (C) 2018 Jacek Wieczorek
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.	See the LICENSE file for details.
 */

#ifndef MKJSON_H
#define MKJSON_H

// JSON container type macros
// MKJSON_ARR - an array
// MKJSON_OBJ - an object (hash, or whatever...)
#define MKJSON_ARR 0
#define MKJSON_OBJ 1

// JSON data type macros
#define MKJSON_STRING            (int)('s') // const char* - String data
#define MKJSON_STRING_FREE       (int)('f') // char* - String data, but pointer is freed
#define MKJSON_JSON              (int)('r') // const char* - JSON data (like string, but no quotes)
#define MKJSON_JSON_FREE         (int)('j') // char* - JSON data, but pointer is freed
#define MKJSON_INT               (int)('i') // int - An integer
#define MKJSON_LLINT             (int)('I') // long long int - A long integer
#define MKJSON_DOUBLE            (int)('d') // double - A double
#define MKJSON_LDOUBLE           (int)('D') // long double - A long double
#define MKJSON_SCI_DOUBLE        (int)('e') // double - A double with scientific notation
#define MKJSON_SCI_LDOUBLE       (int)('E') // long double - A long double with scientific notation
#define MKJSON_BOOL              (int)('b') // int - A boolean value
#define MKJSON_NULL              (int)('n') // -- - JSON null value

// Cause one argument of certain type to be ignored
#define MKJSON_IGN_STRING        (-MKJSON_STRING)
#define MKJSON_IGN_STRING_FREE   (-MKJSON_STRING_FREE)
#define MKJSON_IGN_JSON          (-MKJSON_JSON)
#define MKJSON_IGN_JSON_FREE     (-MKJSON_JSON_FREE)
#define MKJSON_IGN_INT           (-MKJSON_INT)
#define MKJSON_IGN_LLINT         (-MKJSON_LLINT)
#define MKJSON_IGN_DOUBLE        (-MKJSON_DOUBLE)
#define MKJSON_IGN_LDOUBLE       (-MKJSON_LDOUBLE)
#define MKJSON_IGN_BOOL          (-MKJSON_BOOL)
#define MKJSON_IGN_NULL          (-MKJSON_NULL)

extern char *mkjson( int otype, int count, ... );

#endif
