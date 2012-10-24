#ifndef _JS0N_H_
#define _JS0N_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* user callback type */
typedef int ( * js0n_user_cb_t ) ( uint8_t * cursor, uint16_t length );

typedef struct
{
    // public:
    uint8_t * js;
    uint16_t length;
    js0n_user_cb_t user_cb;

    // private:
    uint8_t * cursor;
    uint8_t * mark;
    uint8_t depth;
    uint8_t utf8_remain;
    uint16_t gostate;
} js0n_parser_t;

int js0n_parse ( js0n_parser_t * parser );

#ifdef __cplusplus
}
#endif

#endif
