#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mkjson.h"

int main( )
{
	char *json = mkjson( 8,
		's', "mystr", "hello world!",
		'i', "myinteger", 42,
		'd', "mydoublevalue", 1.4481,
		'e', "exponential", 3e62,
		'b', "boolean", 1,
		'n', "nullvalue",
		'j', "object", mkjson( 1, 
			'i', "something", 45 
			),
		'j', "array", mkjson( -3, 
			'i', 123,
			's', "another string!",
			'b', 0 )
		);
	
	//Just print it and then free
	assert( json != NULL );
	printf( "%s\n", json );
	free( json );
	return 0;
}
