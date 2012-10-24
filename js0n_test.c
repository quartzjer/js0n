#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "js0n.h"

/* this callback will be called for every JSON key and value in the input JSON string */
int js0n_callback ( uint8_t * cursor, uint16_t length )
{
    static int iter = 0;

    printf ( "%d: %.*s\n", iter, length, cursor );
    iter++;
    
    return 0;
}

int main ( int argc, char ** argv )
{
    int ret;
    uint8_t buff [ 1024 ], *json = NULL;
    int len, lastlen = 0;
    FILE * f;
    js0n_parser_t parser;

    if ( ( f = fopen ( argv [ 1 ], "r" ) ) == NULL )
    {
        printf ( "uhoh opening %s\n", argv [ 1 ] );
        exit ( 1 );
    }

    while ( ( len = fread ( buff, 1, 1024, f ) ) > 0 )
    {
        json = realloc ( json, ( lastlen + len ) );
        memcpy ( ( json + lastlen ), buff, len );
        lastlen += len;
    }

    fclose ( f );

    printf ( "got[%.*s]\n\n", lastlen, json );

    parser.js = json;
    parser.length = strlen ( ( const char * ) json );
    parser.user_cb = js0n_callback;

    printf ( "parsing input...\n" );

    ret = js0n_parse ( &parser );

    printf ( "parser returned %d\n", ret );

    return 0;
}
