/*
 * test if the interface is sucessfully extracted from newLib
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../hbwmalloc-hermit/malloc.h"

#define stats
/*#define opt*/
/*#define malloc_usable_size*/
/*#define info*/
/*#define trim*/

int main()
{
    int ele = 3000;
    char* str[9];

// set options of the malloc algorithm..... Ignore now for the Interface test
#ifdef opt
    mallopt(M_TRIM_THRESHOLD, 1024);
#endif

// functions that allocate memorys
    str[0] = calloc(ele,1);
    str[1] = malloc(ele);
    str[2] = memalign(64, ele);
    str[3] = valloc(ele);
    str[4] = pvalloc(ele);

// functions that reflect the overhead of alogrithm (mem taken by malloc interface internal data structures)
#ifdef malloc_usable_size
    size_t usable_size_str1 = malloc_usable_size(str[1]);
    size_t usable_size_str2 = malloc_usable_size(str[2]);

    printf("malloc usable size: %d\nmemalign usable size: %d\n",
            usable_size_str1, usable_size_str2);
#endif

// functions that show allocation status

// check alloc stats after allocations
#ifdef stats 
    malloc_stats();

    // mstats ===> the same as malloc_stats()
    /*mstats("whatever");*/
#endif

#ifdef info 
    struct mallinfo mall_info; 
    mall_info = mallinfo(); 
    printf( 
        "total space allocated from system %d\n  number of non-inuse chunks %d\n unused -- always zero %d\n number of mmapped regions %d\n total space in mmapped regions %d\n unused -- always zero %d\n unused -- always zero %d\n total allocated space %d\n total non-inuse space %d\n top-most, releasable (via malloc_trim) space %d\n ",

        mall_info.arena,    /* total space allocated from system */
        mall_info.ordblks,  /* number of non-inuse chunks */
        mall_info.smblks,   /* unused -- always zero */
        mall_info.hblks,    /* number of mmapped regions */
        mall_info.hblkhd,   /* total space in mmapped regions */
        mall_info.usmblks,  /* unused -- always zero */
        mall_info.fsmblks,  /* unused -- always zero */
        mall_info.uordblks, /* total allocated space */
        mall_info.fordblks, /* total non-inuse space */
        mall_info.keepcost /* top-most, releasable (via malloc_trim) space */
    );	
#endif


// assign values to allocated chunks
    int i;
    for(i=0; i<ele; i++)
    {
        str[0][i] = '0';
        str[1][i] = '1';
        str[2][i] = '2';
        str[3][i] = '3';
        str[4][i] = '4';
    }

// realloc the malloced space
    str[1] = realloc(str[1], 4096*4);

// check alloc stats after reallocation
#ifdef stats 
    malloc_stats();
#endif 

// check if allocation succeed
    for(i=0; i<5; i++)
    {
        printf("%c\n", str[i][1]);
        free(str[i]);
    }

// this function returns mem back to the system, useless in hermitCore, sbrk doesn't support negative argument
#ifdef trim

#ifdef stats 
    malloc_stats();
#endif

    if(!malloc_trim(128))
        return 1;
#endif

// check alloc stats after free
#ifdef stats 
    malloc_stats();
#endif

    return 0;
}
