// this file implements a version of malloc based on a global freelist allocator protected by a lock 

#include "freelist.h"

#include <pthread.h> 

#ifdef USE_LOCAL_ALLOCATOR
	__thread freelist_allocator alloc;
#else 
	freelist_allocator alloc;
#endif // LOCAL


	static inline void initAllocator() {
		freelist_init(&alloc);
	}

	static inline void destroyAllocator() {
		freelist_destroy(&alloc);
	}

	static inline void* myMalloc(size_t size) {
		return freelist_malloc(&alloc, size);
	}

	static inline void myFree(void *mem) {
		freelist_free(&alloc, mem);
	}

