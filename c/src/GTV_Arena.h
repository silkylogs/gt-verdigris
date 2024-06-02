#ifndef GTV_Arena_DEFINED
#define GTV_Arena_DEFINED

typedef struct GTV_Arena {
    unsigned char *backing_memory;
    int allocated;
    int capacity;
} GTV_Arena;

bool GTV_Arena_init(GTV_Arena *a, unsigned char *mem, int size);
void *GTV_Arena_alloc(GTV_Arena *a, int size);
void GTV_Arena_free_all(GTV_Arena *a);
int GTV_Arena_get_current_allocated_bytes(GTV_Arena a);
bool GTV_Arena_shrink(GTV_Arena *a, int shrinked_size);

#endif