#include <mkjson.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

//Return JSON string built from va_arg arguments
//If no longer needed, should be passed to free() by user
char *mkjson( int count, ... )
{
	int i, intval, len, object, failure = 0;
	char *json, *prefix, *strval, **chunks;
	const char *key;
	char type;
	double dblval;
	
	va_list ap;
	va_start( ap, count );
	
	//Determine whether it's an JSON object or normal array
	//Positive count indicates an object
	//Negative count indicates an array
	if ( count == 0 ) return NULL;
	else if ( count > 0 ) object = 1;
	else
	{
		count = -count;
		object = 0;
	}
	
	//Allocate chunk pointer array - on standard platforms each one should be NULL
	chunks = calloc( count, sizeof( char* ) );
	if ( chunks == NULL ) return NULL;
	
	//Create chunks
	for ( i = 0; i < count && !failure; i++ )
	{
		//Get type
		type = tolower( va_arg( ap, int ) );
	
		//Get key
		if ( object )
		{
			key = va_arg( ap, char* );
			if ( key == NULL )
			{
				key = "(nil)";
				type =  'n';
			}
		}
		
		//Predict prefix length
		//asprintf is not a standard unfortunately :(
		len = snprintf( NULL, 0, "%s%s%s%s", 
			i != 0 ? ", " : "",    //No comma before first entry
			object ? "\"" : "",     //Quote before key
			object ? key : "",      //Key
			object ? "\": " : "" ); //Quote and colon after key
			
		//Allocate prefix memory or indicate failure
		prefix = malloc( len + 1 );
		if ( prefix == NULL )
		{
			failure = 1;
			break;
		}
			
		//Generate prefix - this has to match the sprintf above
		snprintf( prefix, len + 1, "%s%s%s%s", 
			i != 0 ? ", " : "",    //No comma before first entry
			object ? "\"" : "",     //Quote before key
			object ? key : "",      //Key
			object ? "\": " : "" ); //Quote and colon after key
			
			
		//Depending on entry type
		switch ( type )
		{
			case 'j': //Some JSON string (passed to free)
			case 's': //A constant string
			case 'r': //A string without quotes (raw data)
			case 'f': //A string (passed to free)
				strval = va_arg( ap, char* ); //This should be const, but it isn't because sometimes it needs to be freed
				len = snprintf( NULL, 0, "%s%s%s%s", prefix, ( type == 's' || type == 'f' ) ? "\"" : "", strval == NULL ? "(nil)" : strval, ( type == 's' || type == 'f' ) ? "\"" : "" );
				if ( ( chunks[i] = malloc( len + 1 ) ) != NULL )
					snprintf( chunks[i], len + 1, "%s%s%s%s", prefix, ( type == 's' || type == 'f' ) ? "\"" : "", strval == NULL ? "(nil)" : strval, ( type == 's' || type == 'f' ) ? "\"" : "" );
					
				//Free string memory if necessary
				if ( type == 'j' || type == 'f' ) free( strval );
				break;
				
			//Integer
			case 'i':
				intval = va_arg( ap, int );
				len = snprintf( NULL, 0, "%s%d", prefix, intval );
				if ( ( chunks[i] = malloc( len + 1 ) ) == NULL ) break;
				snprintf( chunks[i], len + 1, "%s%d", prefix, intval  );
				break;
					
			//Double
			case 'd':
				dblval = va_arg( ap, double );
				len = snprintf( NULL, 0, "%s%f", prefix, dblval );
				if ( ( chunks[i] = malloc( len + 1 ) ) == NULL ) break;
				snprintf( chunks[i], len + 1, "%s%f", prefix, dblval );
				break;
				
			//Double (exponential notation)
			case 'e':
				dblval = va_arg( ap, double );
				len = snprintf( NULL, 0, "%s%e", prefix, dblval );
				if ( ( chunks[i] = malloc( len + 1 ) ) == NULL ) break;
				snprintf( chunks[i], len + 1, "%s%e", prefix, dblval );
				break;
				
			//Boolean
			case 'b':
				intval = va_arg( ap, int );
				len = snprintf( NULL, 0, "%s%s", prefix, intval ? "true" : "false" );
				if ( ( chunks[i] = malloc( len + 1 ) ) == NULL ) break;
				snprintf( chunks[i], len + 1, "%s%s", prefix, intval ? "true" : "false" );
				break;
				
			//JSON null
			case 'n':
				len = snprintf( NULL, 0, "%snull", prefix );
				if ( ( chunks[i] = malloc( len + 1 ) ) == NULL ) break;
				snprintf( chunks[i], len + 1, "%snull", prefix);
				break;
			
			//Bad type specifier
			default:
				chunks[i] = NULL;
				break;
		}
		
		//NULL chunk indicates failure
		if ( chunks[i] == NULL ) failure = 1;
	}
	
	//If everything is fine, merge chunks and create full JSON table
	if ( !failure )
	{
		//Get total length (this is without NUL byte)
		len = 0;
		for ( i = 0; i < count; i++ )
		{
			if ( chunks[i] != NULL ) //That should be ALWAYS true
				len += strlen( chunks[i] );
		}
		
		//Allocate memory for the whole thing
		json = calloc( len + 1 + 2, sizeof( char ) );
		if ( json != NULL )
		{
			//Merge chunks
			for ( i = 0; i < count; i++ )
			{
				if ( chunks[i] != NULL ) //That should be ALWAYS true
					strcat( json + 1, chunks[i] );
			}
	
			//Add proper brackets
			json[0] = object ? '{' : '[';
			json[len + 1] = object ? '}' : ']';
		}
	}
	else json = NULL;
	
	//Free chunks
	for ( i = 0; i < count; i++ )
		free( chunks[i] );
	
	va_end( ap );
	return json;
}

