#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mkjson.h"

int main( )
{
	char *json = mkjson( MKJSON_OBJ, 13,
		MKJSON_STRING,      "mystr", "hello world!",
		MKJSON_INT,         "myinteger", 42,
		MKJSON_LLINT,       "longlong", 784ll,
		MKJSON_DOUBLE,      "double", 1.4481,
		MKJSON_LDOUBLE,     "longdbl", 1.22l,
		MKJSON_SCI_DOUBLE,  "exponential", 3e62,
		MKJSON_SCI_LDOUBLE, "exponentialL", 3e104l,
		MKJSON_BOOL,        "boolean", 1,
		MKJSON_NULL,        "nullvalue",
		MKJSON_STRING,      "alsonull", NULL,

		MKJSON_JSON_FREE,   "object", mkjson( MKJSON_OBJ, 1,
			MKJSON_INT,     "something", 45
			),

		MKJSON_JSON_FREE,   "array", mkjson( MKJSON_ARR, 3,
			MKJSON_INT,         123,
			MKJSON_STRING_FREE, strdup( "another string!" ),
			MKJSON_BOOL,        0
			),

		MKJSON_JSON_FREE,   "empty", mkjson( MKJSON_OBJ, 0 )
			);

	//Just print it and then free
	assert( json != NULL );
	printf( "%s\n", json );
	free( json );
	return 0;
}
