#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <malloc.h>

#ifndef NATIVE 
#include "../../hbwmalloc-hermit/hbwmalloc.h"
#endif

#ifndef NUM_THREADS
#define NUM_THREADS     3
#endif

#ifndef NUM_ITER
#define NUM_ITER    10000
#endif

#ifndef SIZE
#define SIZE    16384
#endif

void* perform_work( void* argument )
{
    int passed_in_value;

    passed_in_value = *( ( int* )argument );
    printf( "Hello World! It's me, thread with argument %d!\n", passed_in_value );

    mallopt(M_TRIM_THRESHOLD, 1024*1024*1024);  
    hbw_mallopt(M_TRIM_THRESHOLD, 1024*1024*1024);


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
