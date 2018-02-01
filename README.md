# mkjson
A super-simple C JSON encoder.

`mkjson` is a single-function C library intended to make JSON strings building in C easier. Making use of variable length argument lists, it allows you to create complex JSON objects just in one line.

#### Example
```cpp
char *json = mkjson( 3,
		's', "this",    "is really simple!",
		'i', "myint",   42,
		
		'j', "object",  mkjson( -3, 
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

#### How to use?
`mkjson`'s working principle is very simple. It only takes an integer argument - the length of JSON array/object followed by the data to encode. Positive `count` indicates JSON string is going to be an object and negative indicates an array. The function returns an allocated JSON string. When no longer needed, you should pass it to `free()`.

This is the function prototype:
```cpp
char *mkjson( int count, ... )
```

Each data entry consists of two or three elements, depending on whether it's an array or an object:
```
(char) type , [(const char*) key], value
```
**`type`**  - a character indicating type of JSON data to follow.
**`key`** - a string used as key in JSON object - this should skipped when an array is generated
**`value`** - the value written into the array or object. Its type must match with the one indicated by `type`.

The only exception is the JSON `null` value (indicated by type `n`) - in such case, the value argument should be skipped as well. Please see the table below for full list of supported data types.

##### Data type specifiers
|Data type|Expected&nbsp;value|Description|
|:---:|---:|:---|
|`s`|`const char*`|Normal string data|
|`f`|`char*`|Normal string data, just like `s`, but **the pointer is freed**.|
|`r`|`const char*`|Embeds JSON data. Works like `s`, but without quotes. The data is left untouched.|
|`j`|`char*`|Embeds JSON data. Works like `r`, but **the pointer is freed**. Allows nested `mkjson` call! :tada:|
|`i`|`int`|An integer|
|`d`|`double`|A floating-point number|
|`e`|`double`|A floating-point number, like `d`, but exponential notation is used|
|`b`|`int`|A boolean value - translated to either `true` or `false`|
|`n`|*nothing*|A JSON `null` value|