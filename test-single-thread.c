#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef NATIVE 
#include "../../hbwmalloc-hermit/malloc.h"
#endif

#ifndef NUM_ITER
#define NUM_ITER    10000
#endif

#ifndef SIZE
#define SIZE    16*1024
#endif 

int main()
{
    /* optionally: insert more useful stuff here */
    int i;
    void* buf;
    for(i=0; i<NUM_ITER; i++)
    {
        buf = malloc(SIZE*i);
        free(buf);
    }
    malloc_stats();
    return 0;
}


