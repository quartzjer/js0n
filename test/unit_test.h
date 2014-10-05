#ifndef _unit_test_h_
#define _unit_test_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* define to anything that needs to be done before exit */
#define TEST_CLEAN_UP

#define fail_unless(test) \
    do { if(!(test)) { \
        fprintf(stderr, \
                "----------------------------------------------\n" \
                "%s:%d: test FAILED:\nFailed test: %s\n" \
                "----------------------------------------------\n", \
                __FILE__, __LINE__, #test); \
        TEST_CLEAN_UP; \
        exit(1); \
    } } while(0)

#endif

