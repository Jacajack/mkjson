# mkjson
[![The MIT License](https://img.shields.io/badge/license-MIT-orange.svg?style=flat-square)](http://opensource.org/licenses/MIT)
[![Travis CI](https://img.shields.io/travis/Jacajack/mkjson/master.svg?style=flat-square)](https://travis-ci.org/Jacajack/mkjson)
<br>A super-simple C JSON encoder.

`mkjson` is a single-function C library intended to make JSON strings building in C easier. Making use of variable length argument lists, it allows you to create complex JSON objects just in one line.

## Example
```cpp
char *json = mkjson( MKJSON_OBJ, 3,
		's', "this",    "is really simple!",
		'i', "myint",   42,
		'j', "object",  mkjson( MKJSON_ARR, 3, 
				'e', 4.75e-2,
				'n',
				'b', 1
				)
		);
```

This example produces following JSON string
```json
{"this": "is really simple!", "myint": 42, "object": [4.750000e-02, null, true]}

```

## How to use?
`mkjson`'s working principle is very simple. It only takes `otype` value indicating whether the data is going to be an object or an array, the `count` of data values to follow and the actual data to encode. The function returns an allocated JSON string - when no longer needed, you should pass it to `free()`. Valid values for `otype` are `MKJSON_ARR` and `MKJSON_OBJ` macros.

```cpp
char *mkjson( int otype, int length, ... )
```

Each data entry consists of two or three elements, depending on whether it's an array or an object:
**type**, (**key**), **value**

**`type`**  - a `char` (actually, it's treated as `int`) indicating type of JSON data to follow.<br>
**`key`** - a `const char*` used as key in JSON object - this should skipped when an array is generated.<br>
**`value`** - the value written into the array or object. Its type must match with the one indicated by `type`. Please see the table below for full list of supported data types.

The only exception is the JSON `null` value (indicated by type `n`) - in such case, the value argument should be skipped as well. 

### Data type specifiers
|Data type|Expected&nbsp;value&nbsp;type|Description|
|:---:|---:|:---|
|`s`|`const char*`|Normal string data|
|`f`|`char*`|Normal string data, just like `s`, but **the pointer is freed**.|
|`r`|`const char*`|Embeds JSON data. Works like `s`, but without quotes. The data is left untouched.|
|`j`|`char*`|Embeds JSON data. Works like `r`, but **the pointer is freed**. Allows nested `mkjson` call! :tada:|
|`i`|`int`|An integer|
|`I`|`long long int`|Same as `i`|
|`d`|`double`|A floating-point number|
|`D`|`long double`|Same as `d`|
|`e`|`double`|A floating-point number, like `d`, but exponential notation is used|
|`E`|`long double`|Same as `e`|
|`b`|`int`|A boolean value - translated to either `true` or `false`|
|`n`|*nothing*|A JSON `null` value|
