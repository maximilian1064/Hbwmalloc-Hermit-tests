/*
 * test if the interface runs properly in multi-thread environment 
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

#ifndef NATIVE 
#include "../../hbwmalloc-hermit/hbwmalloc.h"
#endif

#ifndef NUM_THREADS
#define NUM_THREADS     3
#endif

#ifndef NUM_ITER
#define NUM_ITER    10000
#endif

#define stats
#define opt
#define malloc_usable_size
#define info
/*#define trim*/

void* perform_work( void* argument )
{
    int passed_in_value;

    passed_in_value = *( ( int* )argument );
    printf( "Hello World! It's me, thread with argument %d!\n", passed_in_value );

    

    /* optionally: insert more useful stuff here */

    int ele = 4000;
    char* str[10];

// set options of the malloc algorithm..... Ignore now for the Interface test
#ifdef opt
    hbw_mallopt(M_TRIM_THRESHOLD, 1024);

    // ddr variant
    mallopt(M_TRIM_THRESHOLD, 1024);
#endif

// functions that allocate memorys
    str[0] = hbw_calloc(ele,1);
    str[1] = hbw_malloc(ele);
    str[2] = hbw_memalign(64, ele);
    str[3] = hbw_valloc(ele);
    str[4] = hbw_pvalloc(ele);

    // ddr variant
    str[5] = calloc(ele,1);
    str[6] = malloc(ele);
    str[7] = memalign(64,ele);
    str[8] = valloc(ele);
    str[9] = pvalloc(ele);


// functions that reflect the overhead of alogrithm (mem taken by malloc interface internal data structures)
#ifdef malloc_usable_size
    size_t usable_size_str1 = hbw_malloc_usable_size(str[1]);
    size_t usable_size_str2 = hbw_malloc_usable_size(str[2]);

    printf("malloc usable size: %d\nmemalign usable size: %d\n",
            usable_size_str1, usable_size_str2);

    // ddr variant
    usable_size_str1 = malloc_usable_size(str[6]);
    usable_size_str2 = malloc_usable_size(str[7]);

    printf("malloc usable size: %d\nmemalign usable size: %d\n",
            usable_size_str1, usable_size_str2);

#endif

// functions that show allocation status

// check alloc stats after allocations
#ifdef stats 
    hbw_malloc_stats();

    // mstats ===> the same as malloc_stats()
    hbw_mstats("whatever");

    // ddr variant
    
    malloc_stats();

    // mstats ===> the same as malloc_stats()
    mstats("whatever");

#endif

#ifdef info 
    struct mallinfo mall_info; 
    mall_info = hbw_mallinfo(); 
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

    // ddr variant
    struct mallinfo mall_info_ddr; 
    mall_info_ddr = mallinfo(); 
    printf( 
        "total space allocated from system %d\n  number of non-inuse chunks %d\n unused -- always zero %d\n number of mmapped regions %d\n total space in mmapped regions %d\n unused -- always zero %d\n unused -- always zero %d\n total allocated space %d\n total non-inuse space %d\n top-most, releasable (via malloc_trim) space %d\n ",

        mall_info_ddr.arena,    /* total space allocated from system */
        mall_info_ddr.ordblks,  /* number of non-inuse chunks */
        mall_info_ddr.smblks,   /* unused -- always zero */
        mall_info_ddr.hblks,    /* number of mmapped regions */
        mall_info_ddr.hblkhd,   /* total space in mmapped regions */
        mall_info_ddr.usmblks,  /* unused -- always zero */
        mall_info_ddr.fsmblks,  /* unused -- always zero */
        mall_info_ddr.uordblks, /* total allocated space */
        mall_info_ddr.fordblks, /* total non-inuse space */
        mall_info_ddr.keepcost /* top-most, releasable (via malloc_trim) space */
    );	

#endif


// assign values to allocated chunks
    int i;
    for(i=0; i<ele/sizeof(char); i++)
    {
        str[0][i] = '0';
        str[1][i] = '1';
        str[2][i] = '2';
        str[3][i] = '3';
        str[4][i] = '4';

        // ddr variant
        str[5][i] = '5';
        str[6][i] = '6';
        str[7][i] = '7';
        str[8][i] = '8';
        str[9][i] = '9';

    }

// realloc the malloced space
    str[1] = hbw_realloc(str[1], 4096*4);

    // ddr variant
    str[6] = realloc(str[6], 4096*4);

// check alloc stats after reallocation
#ifdef stats 
    hbw_malloc_stats();

    // ddr variant
    malloc_stats();
#endif 

// check if allocation succeed
    for(i=0; i<5; i++)
    {
        printf("%c\n", str[i][1]);
        hbw_free(str[i]);
    }

    // ddr variant
    for(i=5; i<10; i++)
    {
        printf("%c\n", str[i][1]);
        free(str[i]);
    }

// this function returns mem back to the system, useless in hermitCore, sbrk doesn't support negative argument
#ifdef trim

#ifdef stats 
    hbw_malloc_stats();

    // ddr variant
    malloc_stats();
#endif

    if(!hbw_malloc_trim(128))
        return 1;

    // ddr variant
    if(!malloc_trim(128))
        return 1;

#endif

// check alloc stats after free
#ifdef stats 
    hbw_malloc_stats();

    // ddr variant
    malloc_stats();
#endif


// show thread id
    pthread_t id = pthread_self();
#ifndef LINUX 
    printf("thread %lu finished allocation\n", id.x);
#else
    printf("thread %u finished allocation\n", id);
#endif

    return NULL;
}

int main( int argc, char** argv )
{
    pthread_t threads[ NUM_THREADS ];
    int thread_args[ NUM_THREADS ];
    int result_code;
    unsigned index;

    // create all threads one by one
    for( index = 0; index < NUM_THREADS; ++index )
    {
        thread_args[ index ] = index;
        printf("In main: creating thread %d\n", index);
        result_code = pthread_create( &threads[index], NULL, perform_work, &thread_args[index] );
        assert( !result_code );
    }

    // wait for each thread to complete
    for( index = 0; index < NUM_THREADS; ++index )
    {
        // block until thread 'index' completes
        result_code = pthread_join( threads[ index ], NULL );
        assert( !result_code );
        printf( "In main: thread %d has completed\n", index );
    }

    printf( "In main: All threads completed successfully\n" );
    exit( EXIT_SUCCESS );
}
