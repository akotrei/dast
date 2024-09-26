#ifndef __DAST_MAP_H__
#define __DAST_MAP_H__

#include "array.h"
#include "memory.h"

#define DAST_SET_GROW_FACTOR 2.0f
#define DAST_SET_INITIAL_CAPACITY 8
#define DAST_SET_MAX_LOAD 0.5f

// memory laoyot of @dast_slote_t type:
// dast_slote_t* slote -> |--hash--|--key--|
typedef struct _dast_slote_t { unsigned long hash; } dast_slote_t;

// used to handle each bucket of slots
typedef struct _dast_bucket_info_t {unsigned long elems; unsigned long capacity; } dast_bucket_info_t;

typedef struct _dast_set_t
{
    dast_slote_t**      buckets;
    dast_bucket_info_t* buckets_info;
    unsigned long       elems;
    unsigned long       capacity;
    unsigned long       (*hash)(void* k);
    int                 (*cmp)(void* l, void* r);
    int                 key_size;
} dast_set_t;

// Initialize @map
// @key_size - size (in bytes) of keys that @map should hold
// @hash - hash function to get key's hash
// @cmp - comparing function to compare keys
void dast_set_init(dast_set_t* set, int key_size, unsigned long (*hash)(void* k), int (*cmp)(void* l, void* r));

// Destroy @map and free memory that @map uses
void dast_set_destroy(dast_set_t* set);

// Copy @map to @dst. Note: it is a shallow copying
// Note: @dst supposed to be not initialized
void dast_set_copy(dast_set_t* set, dast_set_t* dst);

// Copy @map to @dst. Also it copies all objcets too (invokes @cpy for each key)
// Note: @dst supposed to be not initialized
void dast_set_deepcopy(dast_set_t* set, dast_set_t* dst, void (*cpy)(void* src, void* dst));

// remove all objects from @map
void dast_set_clear(dast_set_t* set);

// remove all objects from @map with invoking @del for each key
void dast_set_deepclear(dast_set_t* set, void (*del)(void* key));

// Get key from a slote that is return by @dast_set_get. Note: @slote should not be 0
#define DAST_SLOTE_KEY(slote) ((char*)(slote) + sizeof(dast_slote_t))

// Get a slote by @key. If @key does not exist return 0
dast_slote_t* dast_set_get(dast_set_t* set, void* key);

// Set new slote with @key and @value, if @key is already exists -
// overrides this slote with @key and @value
void dast_set_add(dast_set_t* set, void* key);

// Delete @slote from @map, @slote should be a result of @dast_set_get, and not be 0
// Also between @dast_set_get and @dast_set_del should not be any @map's modifications -
// it may make @slote to be invalid
void dast_set_del(dast_set_t* set, dast_slote_t* slote);

// Get array of slots from @map
// The caller is responsible for deleting return slots
dast_array_t* dast_set_slots(dast_set_t* set);

#endif // __DAST_MAP_H__
