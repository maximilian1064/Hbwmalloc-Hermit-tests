HERMIT-CC = x86_64-hermit-gcc
LIBHBWMALLOC = /home/maxi1064/workspace/HermitHBWwork/hbwmalloc-hermit/libhbwmalloc.a

THREAD_FLAG = -pthread

TESTS = test-interface-mt-native test-mt-native test-mt-linux

all: $(TESTS) 

#ignore all targets suffixed with -hbw if you don't have the library

#test-interface-mt-native ==> test a mix of interface functions provided by newlib's malloc, in multi-thread environments
#test-mt-native           ==> test the newlib's malloc performance in multi-thread environments, test pattern is simple for-loop of malloc-free sequence
#test-mt-linux 			  ==> same as test-mt-native, but using linux's glibc

test-interface-st-hbw: interface-single-thread-test.c
	$(HERMIT-CC) $< $(LIBHBWMALLOC) -o $@ 

test-interface-mt-hbw: interface-multi-thread-test.c
	$(HERMIT-CC) $(THREAD_FLAG) -DNUM_THREADS=3 $< $(LIBHBWMALLOC) -o $@ 

test-interface-mt-native: interface-multi-thread-test.c
	$(HERMIT-CC) $(THREAD_FLAG) -DNUM_THREADS=3 -DNATIVE $< -o $@ 

test-st-hbw: test-single-thread.c 
	$(HERMIT-CC) -DSIZE=16384 -DNUM_ITER=10000 $< $(LIBHBWMALLOC) -o $@ 

test-mt-hbw: test-multi-thread.c
	$(HERMIT-CC) $(THREAD_FLAG) -DSIZE=16384 -DNUM_ITER=10000 -DNUM_THREADS=1 $< $(LIBHBWMALLOC) -o $@ 

test-mt-native: test-multi-thread.c
	$(HERMIT-CC) $(THREAD_FLAG) -DSIZE=16384 -DNUM_ITER=10000 -DNUM_THREADS=1 -DNATIVE $< -o $@ 

test-mt-linux: test-multi-thread.c
	$(CC) $(THREAD_FLAG) -DSIZE=16384 -DNUM_ITER=10000 -DNUM_THREADS=1 -DNATIVE -DLINUX $< -o $@ 

clean:
	rm $(TESTS)


