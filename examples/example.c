#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mkjson.h"

int main( )
{
	char *json = mkjson( MKJSON_OBJ, 12,
		's', "mystr", "hello world!",
		'i', "myinteger", 42,
		'I', "longlong", 784ll,
		'd', "double", 1.4481,
		'D', "longdbl", 1.22l,
		'e', "exponential", 3e62,
		'E', "exponentialL", 3e104l,	
		'b', "boolean", 1,
		'n', "nullvalue",
		'j', "object", mkjson( MKJSON_OBJ, 1, 
			'i', "something", 45 
			),
		'j', "array", mkjson( MKJSON_ARR, 3, 
			'i', 123,
			's', "another string!",
			'b', 0 ),
		'j', "empty", mkjson( MKJSON_OBJ, 0 )
		);
	
	//Just print it and then free
	assert( json != NULL );
	printf( "%s\n", json );
	free( json );
	return 0;
}
