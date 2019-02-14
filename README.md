# mkjson
[![The MIT License](https://img.shields.io/badge/license-MIT-orange.svg?style=flat-square)](http://opensource.org/licenses/MIT)
[![Travis CI](https://img.shields.io/travis/Jacajack/mkjson/master.svg?style=flat-square)](https://travis-ci.org/Jacajack/mkjson)
<br>A super-simple C JSON encoder.

`mkjson` is a single-function C library intended to make building  JSON strings in C easier. Making use of variable length argument lists, it allows you to create complex JSON objects in just one line.

## Example
```cpp
char *json = mkjson( MKJSON_OBJ, 3,
		MKJSON_STRING,    "this",    "is really simple!",
		MKJSON_INT,       "myint",   42,
		
		MKJSON_JSON_FREE, "object",  mkjson( MKJSON_ARR, 3, 
				MKJSON_SCI_DOUBLE, 4.75e-2,
				MKJSON_NULL,
				MKJSON_BOOL,       1
				)
		);
```

This example produces following JSON string
```json
{"this": "is really simple!", "myint": 42, "object": [4.750000e-02, null, true]}

```

## How to use?
`mkjson`'s working principle is incredibly simple. It only takes `otype` value indicating whether the data is going to be an object (`MKJSON_OBJ`) or an array (`MKJSON_ARR`), the `count` of data values to follow and the actual data to encode. The function returns an automatically allocated JSON string - when no longer needed, it should be passed to `free()`.

```cpp
char *mkjson( enum mkjson_container_type otype, int length, ... )
```

As you may have noticed on the example above, each data entry consists of two or three elements, depending on whether it's an array or an object:<br>
 - **`type`**  - an enum value indicating the type of JSON data to follow.<br>
 - **`key`** - a `const char*` used as key in JSON object - **this should skipped when an array is generated**.<br>
 - **`value`** - the value written into the array or object. Its type must match with the one indicated by `type`. Please see the table below for the full list of supported data types.

### Data type specifiers
|Data type|Expected&nbsp;value&nbsp;type|Description|
|:---|:---|:---|
|`MKJSON_STRING`|`const char*`|Embeds a null-terminated string. When null pointer is passed, the string is replaced with JSON `null` value.|
|`MKJSON_STRING_FREE`|`char*`|Behaves like `MKJSON_STRING`, but **the pointer is freed**.|
|`MKJSON_JSON`|`const char*`|Embeds JSON data. Works like `MKJSON_STRING`, but without quotes. When null pointer is passed, the JSON string is replaced with `null` value.|
|`MKJSON_JSON_FREE`|`char*`|Behaves like `MKJSON_JSON`, but **the pointer is freed**. This allows nested `mkjson` calls.|
|`MKJSON_INT`|`int`|An integer|
|`MKJSON_LLINT`|`long long int`|A long integer|
|`MKJSON_DOUBLE`|`double`|A double-precision floating-point number|
|`MKJSON_LDOUBLE`|`long double`|A long double-precision floating-point number|
|`MKJSON_SCI_DOUBLE`|`double`|Same as `MKJSON_DOUBLE`, but scientific notation is used.|
|`MKJSON_SCI_LDOUBLE`|`long double`|Same as `MKJSON_LDOUBLE`, but scientific notation is used.|
|`MKJSON_BOOL`|`int`|A boolean value - translated to either `true` or `false`|
|`MKJSON_NULL`|*nothing*|A JSON `null` value. **Here, the `value` argument shall be skipped!**|

Additionally, following data type specifiers causing a data entry to be skipped are supported: `MKJSON_IGN_STRING`, `MKJSON_IGN_STRING_FREE`, `MKJSON_IGN_JSON`, `MKJSON_IGN_JSON_FREE`, `MKJSON_IGN_INT`, `MKJSON_IGN_LLINT`, `MKJSON_IGN_DOUBLE`, `MKJSON_IGN_LDOUBLE`, `MKJSON_IGN_BOOL`, `MKJSON_IGN_NULL`.

The `MKJSON_IGN_STRING_FREE` and `MKJSON_IGN_JSON_FREE` cause the pointer to be passed to `free()`.

The 'ignore' specifiers may be useful in cases like this one:
```c
mkjson( MKJSON_OBJ, 2,
	dummy % 2 ? MKJSON_INT : MKJSON_IGN_INT,   "something",  777,
	dummy % 2 ? MKJSON_IGN_NULL : MKJSON_NULL, "something"
)
```
