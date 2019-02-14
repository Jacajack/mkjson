#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "mkjson.h"

int main( )
{
	time_t dummy = time( NULL );

	char *json = mkjson( MKJSON_OBJ, 16,
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

		MKJSON_JSON_FREE,   "empty", mkjson( MKJSON_OBJ, 7,
			MKJSON_IGN_STRING,      "mystr", "hello world!",
			MKJSON_IGN_INT,         "myinteger", 42,
			MKJSON_IGN_LLINT,       "longlong", 784ll,
			MKJSON_IGN_DOUBLE,      "double", 1.4481,
			MKJSON_IGN_LDOUBLE,     "longdbl", 1.22l,
			MKJSON_IGN_BOOL,        "boolean", 1,
			MKJSON_IGN_NULL,        "nullvalue"
		),

		MKJSON_JSON_FREE,   "silly0", mkjson( MKJSON_OBJ, 2,
			dummy % 2 ? MKJSON_INT : MKJSON_IGN_INT,   "something", 777,
			dummy % 2 ? MKJSON_IGN_NULL : MKJSON_NULL, "something"
			),

		MKJSON_JSON_FREE,   "silly1", mkjson( MKJSON_ARR, 4,
			dummy % 2 == 0 ? MKJSON_INT : MKJSON_IGN_INT,  123,
			dummy % 2 == 1 ? MKJSON_INT : MKJSON_IGN_INT,  456,
			dummy % 2 == 0 ? MKJSON_INT : MKJSON_IGN_INT,  456,
			dummy % 2 == 1 ? MKJSON_INT : MKJSON_IGN_INT,  123
			),

		MKJSON_JSON_FREE,   "silly2", mkjson( MKJSON_OBJ, 4,
			dummy % 2 == 0 ? MKJSON_INT : MKJSON_IGN_INT, "A", 123,
			dummy % 2 == 1 ? MKJSON_INT : MKJSON_IGN_INT, "B", 456,
			dummy % 2 == 0 ? MKJSON_INT : MKJSON_IGN_INT, "C", 456,
			dummy % 2 == 1 ? MKJSON_INT : MKJSON_IGN_INT, "D", 123
			)
	);

	//Just print it and then free
	assert( json != NULL );
	printf( "%s\n", json );
	free( json );
	return 0;
}
