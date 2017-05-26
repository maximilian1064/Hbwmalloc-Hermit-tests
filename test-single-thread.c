#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef NATIVE 
#include "../../hbwmalloc-hermit/hbwmalloc.h"
#endif

#ifndef NUM_ITER
#define NUM_ITER    10000
#endif

#ifndef SIZE
#define SIZE    16*1024
#endif 

int main()
{
    /* Test Scenario 1 */

    int i;
    void* buf;
    void* buf_nm;
    for(i=0; i<NUM_ITER; i++)
    {
        buf = hbw_calloc(2,SIZE*i);
        buf_nm = calloc(2,SIZE*i);
        hbw_free(buf);
        free(buf_nm);
    }

    hbw_malloc_stats();
    malloc_stats();



    /* Test Scenario 2 */
    int* buf_array[NUM_ITER];
    int* buf_nm_array[NUM_ITER];
    
    for(i=0; i<NUM_ITER; i++)
    {
        buf_array[i] = hbw_malloc(SIZE*i);
        buf_nm_array[i] = malloc(SIZE*i);
        int k;
        for(k=0; k<SIZE*i/sizeof(int); k++)
        {
            buf_array[i][k] = k;
            buf_nm_array[i][k] = k;
        }
        if(k>0)
        {
            printf("%d\n", buf_array[i][k-1]);
            printf("%d\n", buf_nm_array[i][k-1]);
        }
    }

    for(i=0; i<NUM_ITER; i++)
    {
        hbw_free(buf_array[i]);
        free(buf_nm_array[i]);
    }

    hbw_malloc_stats();
    malloc_stats();


    return 0;
}


