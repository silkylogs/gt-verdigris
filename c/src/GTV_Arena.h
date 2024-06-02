#ifndef GTV_Arena_DEFINED
#define GTV_Arena_DEFINED

#include <stdbool.h>

typedef struct GTV_Arena {
    unsigned char *backing_memory;
    int allocated;
    int capacity;
} GTV_Arena;

bool GTV_Arena_init(GTV_Arena *a, unsigned char *mem, int size) {
    if (!mem) {
        printf("Provided memory is null\n");
        return false;
    }

    a->backing_memory = mem;
    a->allocated = 0;
    a->capacity = size;

    return true;
}

void *GTV_Arena_alloc(GTV_Arena *a, int size) {
    int remaining_capacity = a->capacity - a->allocated;
    if (size > remaining_capacity || size <= 0) {
        printf("Cannot provide %d bytes of memory\n", size);
        return NULL;
    }
    
    unsigned char *mem = &a->backing_memory[a->allocated];
    a->allocated += size;
    return mem;
}

void GTV_Arena_free_all(GTV_Arena *a) {
    a->allocated = 0;
}

// Use this as a "checkpoint" of sorts
int GTV_Arena_get_current_allocated_bytes(GTV_Arena a) {
    return a.allocated;
}

// Set the arena's internal state to the "checkpoint"
bool GTV_Arena_shrink(GTV_Arena *a, int shrinked_size) {
    if (shrinked_size < 0 || shrinked_size > a->allocated) return false;
    a->allocated = shrinked_size;
    return true;
}
#endif