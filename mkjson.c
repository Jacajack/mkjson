#include <mkjson.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

//Works like asprintf, but it's always there
//I don't want the name to collide with anything
static int allsprintf( char **strp, const char *fmt, ... )
{
	int len;
	char *buf;
	
	va_list ap;
	va_start( ap, fmt );
	
	#ifdef _GNU_SOURCE
		//Just handle everything to vasprintf, if it's available
		len = vasprintf( strp, fmt, ap );
	#else
		//Or do it the manual way
		len = vsnprintf( NULL, 0, fmt, ap );
		if ( len >= 0 )
		{
			buf = malloc( ++len );
			if ( buf != NULL )
			{
				//Hopefully, that's the right way to do it
				va_end( ap );
				va_start( ap, fmt );
				
				//Write and return the data
				len = vsnprintf( buf, len, fmt, ap );
				if ( len >= 0 )
				{
					*strp = buf;
				}
				else
				{
					free( buf );
				}
			}
		}
	#endif
	
	va_end( ap );
	return len;
}

//Return JSON string built from va_arg arguments
//If no longer needed, should be passed to free() by user
char *mkjson( int otype, int count, ... )
{
	int i, len, failure = 0;
	char *json, *prefix, **chunks;
	
	//Value - type and data
	int vtype;
	const char *key;
	long long int intval;
	long double dblval;
	char *strval;
	
	//Since v0.9 count cannot be a negative value and datatype is indicated by a separate argument
	//Since I'm not sure whether it's right to put assertions in libraries, the next line is commented out
	//assert( count >= 0 && "After v0.9 negative count is prohibited; please use otype argument instead" );
	if ( count < 0 || ( otype != MKJSON_OBJ && otype != MKJSON_ARR ) ) return NULL;
	
	//Allocate chunk pointer array - on standard platforms each one should be NULL
	chunks = calloc( count, sizeof( char* ) );
	if ( chunks == NULL ) return NULL;
	
	//This should be rather at the point of no return
	va_list ap;
	va_start( ap, count );

	//Create chunks
	for ( i = 0; i < count && !failure; i++ )
	{
		//Get value type
		vtype = va_arg( ap, int );
	
		//Get key
		if ( otype == MKJSON_OBJ )
		{
			key = va_arg( ap, char* );
			if ( key == NULL )
			{
				failure = 1;
				break;
			}
		}
		else key = "";
		
		//Generate prefix
		if ( allsprintf( &prefix, "%s%s%s%s",
			i != 0 ? ", " : "",                         //No comma before first entry
			otype == MKJSON_OBJ ? "\"" : "",            //Quote before key
			key,                                        //Key
			otype == MKJSON_OBJ ? "\": " : "" ) == -1 ) //Quote and colon after key
		{
			failure = 1;
			break;
		}
	
		//Depending on value type
		switch ( vtype )
		{
			case 'j': //Some JSON string (passed to free)
			case 's': //A constant string
			case 'r': //A string without quotes (raw data)
			case 'f': //A string (passed to free)
				strval = va_arg( ap, char* ); //This should be const, but it isn't because sometimes it needs to be freed
				if ( allsprintf( chunks + i, "%s%s%s%s", prefix, ( vtype == 's' || vtype == 'f' ) ? "\"" : "", strval == NULL ? "null" : strval, ( vtype == 's' || vtype == 'f' ) ? "\"" : "" ) == -1 ) chunks[i] = NULL;
				if ( vtype == 'j' || vtype == 'f' ) free( strval ); //Free string memory if requested
				break;
				
			//Integer
			case 'I':
			case 'i':
				if ( vtype == 'i' ) intval = va_arg( ap, int );
				else intval = va_arg( ap, long long int );
				if ( allsprintf( chunks + i, "%s%Ld", prefix, intval ) == -1 ) chunks[i] = NULL;
				break;
					
			//Double
			case 'D':
			case 'd':
				if ( vtype == 'd' ) dblval = va_arg( ap, double );
				else dblval = va_arg( ap, long double );
				if ( allsprintf( chunks + i, "%s%Lf", prefix, dblval ) == -1 ) chunks[i] = NULL;
				break;
				
			//Double (exponential notation)
			case 'E':
			case 'e':
				if ( vtype == 'e' ) dblval = va_arg( ap, double );
				else dblval = va_arg( ap, long double );
				if ( allsprintf( chunks + i, "%s%Le", prefix, dblval ) == -1 ) chunks[i] = NULL;
				break;
				
			//Boolean
			case 'b':
				intval = va_arg( ap, int );
				if ( allsprintf( chunks + i, "%s%s", prefix, intval ? "true" : "false" ) == -1 ) chunks[i] = NULL;
				break;
				
			//JSON null
			case 'n':
				if ( allsprintf( chunks + i, "%snull", prefix ) == -1 ) chunks[i] = NULL;
				break;
			
			//Bad type specifier
			default:
				chunks[i] = NULL;
				break;
		}
		
		//Free prefix memory
		free( prefix );
		
		//NULL chunk indicates failure
		if ( chunks[i] == NULL ) failure = 1;
	}
	
	//We won't use ap anymore
	va_end( ap );
	
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
			json[0] = otype == MKJSON_OBJ ? '{' : '[';
			json[len + 1] = otype == MKJSON_OBJ ? '}' : ']';
		}
	} else json = NULL;
	
	//Free chunks
	for ( i = 0; i < count; i++ )
		free( chunks[i] );
	free( chunks );
	
	return json;
}

