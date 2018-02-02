/* mkjson.h - a part of mkjson library
 *
 * Copyright (C) 2018 Jacek Wieczorek
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.	See the LICENSE file for details.
 */

#ifndef MKJSON_H
#define MKJSON_H

//JSON data type macros
//MKJSON_ARR - an array
//MKJSON_OBJ - an object (hash, or whatever...)
#define MKJSON_ARR 0
#define MKJSON_OBJ 1

extern char *mkjson( int otype, int count, ... );

#endif
