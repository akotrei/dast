#include "array/array_private.h"
#include "utils/mem.h"

dast_u64_t dast_array_sizeof() { return sizeof(dast_array_t); }

dast_array_t* dast_array_init_on(void*             memory,
                                 dast_allocator_t* allocator,
                                 dast_u64_t        obj_size,
                                 dast_cpy_t        cpy,
                                 dast_del_t        del)
{
    dast_array_t* array = (dast_array_t*)memory;
    array->data = allocator->allocate(allocator, obj_size * DAST_ARRAY_INIT_SIZE);
    array->elems = 0;
    array->elem_size = obj_size;
    array->capacity = DAST_ARRAY_INIT_SIZE;
    array->factor = DAST_ARRAY_GROW_FACTOR;
    array->allocator = allocator;
    array->cpy = cpy;
    array->del = del;
    return array;
}

dast_array_t* dast_array_init(dast_allocator_t* allocator, dast_u64_t obj_size, dast_cpy_t cpy, dast_del_t del)
{
    dast_array_t* array = allocator->allocate(allocator, sizeof(dast_array_t));
    array = dast_array_init_on(array, allocator, obj_size, cpy, del);
    return array;
}

void dast_array_destroy_from(void* array)
{
    dast_array_t* a = (dast_array_t*)array;
    char*      data = a->data;
    dast_u64_t obj_size = a->elem_size;
    dast_del_t del = a->del;
    dast_u64_t i = 0;
    dast_u64_t end = a->elems;
    while (i < end)
    {
        del(data + i * obj_size);
        i++;
    }
    a->allocator->deallocate(a->allocator, data);
}

void dast_array_destroy(dast_array_t* array)
{
    dast_allocator_t* allocator = array->allocator;
    dast_array_destroy_from(array);
    allocator->deallocate(allocator, array);
}

void dast_array_shrink(dast_array_t* array)
{
    dast_allocator_t* allocator = array->allocator;
    dast_u64_t        capacity = array->capacity;
    dast_u64_t        elems = array->elems;
    dast_u64_t        elem_size = array->elem_size;
    if (capacity == elems)
    {
        return;
    }
    array->capacity = elems < DAST_ARRAY_INIT_SIZE ? DAST_ARRAY_INIT_SIZE : elems;
    array->data = allocator->reallocate(allocator, array->data, elem_size * array->capacity);
}

void dast_array_reserve(dast_array_t* array, dast_u64_t size)
{
    dast_allocator_t* allocator = array->allocator;
    dast_u64_t        elems = array->elems;
    dast_u64_t        elem_size = array->elem_size;
    dast_u64_t        free_slots = array->capacity - elems;

    if (free_slots < size)
    {
        array->capacity = elems + size;
        array->data = allocator->reallocate(allocator, array->data, elem_size * array->capacity);
    }
}

void dast_array_copy_to(void* array, void* memory, dast_u64_t size)
{
    dast_array_t* a = (dast_array_t*)array;
    dast_array_t* dst_array = (dast_array_t*)memory;

    dst_array->data = a->allocator->allocate(a->allocator, a->capacity * a->elem_size);
    dast_cpy_generic(a->data, dst_array->data, a->elems * a->elem_size);

    dst_array->elems = a->elems;
    dst_array->elem_size = a->elem_size;
    dst_array->capacity = a->capacity;
    dst_array->factor = a->factor;
    dst_array->allocator = a->allocator;
    dst_array->cpy = a->cpy;
    dst_array->del = a->del;
}

void dast_array_deepcopy_to(void* array, void* memory, dast_u64_t size)
{
    dast_array_t* a = (dast_array_t*)array;
    dast_array_t* dst_array = (dast_array_t*)memory;
    dast_u64_t    elem_size = a->elem_size;
    dast_u64_t    i = 0;
    dast_u64_t    end = a->elems;
    dast_cpy_t    cpy = a->cpy;

    char* src_data = a->data;
    char* dst_data = a->allocator->allocate(a->allocator, a->capacity * a->elem_size);
    while (i < end)
    {
        cpy(src_data + i * elem_size, dst_data + i * elem_size, elem_size);
        i++;
    }
    dst_array->data = dst_data;

    dst_array->elems = a->elems;
    dst_array->elem_size = a->elem_size;
    dst_array->capacity = a->capacity;
    dst_array->factor = a->factor;
    dst_array->allocator = a->allocator;
    dst_array->cpy = a->cpy;
    dst_array->del = a->del;
}

dast_array_t* dast_array_copy(dast_array_t* array, dast_allocator_t* allocator)
{
    allocator = allocator ? allocator : array->allocator;
    dast_array_t* new_array = array->allocator->allocate(array->allocator, sizeof(dast_array_t));
    dast_array_copy_to(array, new_array, sizeof(dast_array_t));
    return new_array;
}

dast_array_t* dast_array_deepcopy(dast_array_t* array, dast_allocator_t* allocator)
{
    allocator = allocator ? allocator : array->allocator;
    dast_array_t* new_array = array->allocator->allocate(array->allocator, sizeof(dast_array_t));
    dast_array_deepcopy_to(array, new_array, sizeof(dast_array_t));
    return new_array;
}

dast_u64_t dast_array_size(dast_array_t* array) { return array->elems; }

dast_u64_t dast_array_obj_size(dast_array_t* array) { return array->elem_size; }

void dast_array_clear(dast_array_t* array)
{
    dast_u64_t end = array->elems;
    dast_u64_t elem_size = array->elem_size;
    dast_del_t del = array->del;
    char*      data = array->data;
    dast_u64_t i = 0;
    while (i < end)
    {
        del(data + i * elem_size);
        i++;
    }
    array->elems = 0;
}

void* dast_array_begin(dast_array_t* array) { return array->data; }

void* dast_array_end(dast_array_t* array) { return array->data + array->elems * array->elem_size; }

void* dast_array_index(dast_array_t* array, dast_u64_t index) { return array->data + index * array->elem_size; }

void dast_array_append(dast_array_t* array, void* obj)
{
    dast_allocator_t* allocator = array->allocator;
    dast_u64_t        elems = array->elems;
    dast_u64_t        elem_size = array->elem_size;
    if (elems == array->capacity)
    {
        array->capacity *= array->factor;
        array->data = allocator->reallocate(allocator, array->data, array->capacity * elem_size);
    }
    array->cpy(obj, array->data + elems * elem_size, elem_size);
    array->elems++;
}

void dast_array_insert(dast_array_t* array, void* obj, dast_u64_t index)
{
    if (index >= array->elems)
    {
        dast_array_append(array, obj);
        return;
    }

    char*             data = array->data;
    dast_allocator_t* allocator = array->allocator;
    dast_u64_t        elems = array->elems;
    dast_u64_t        elem_size = array->elem_size;
    if (array->elems == array->capacity)
    {
        array->capacity++;
        char* new_data = allocator->allocate(allocator, array->capacity * elem_size);
        dast_cpy_generic(data, new_data, index * elem_size);
        array->cpy(obj, new_data + index * elem_size, elem_size);
        dast_cpy_generic(data + index * elem_size, new_data + (index + 1) * elem_size, (elems - index) * elem_size);
        allocator->deallocate(allocator, data);
        array->data = new_data;
    }
    else
    {
        dast_move_generic(data + index * elem_size, data + (index + 1) * elem_size, (elems - index) * elem_size);
        array->cpy(obj, data + index * elem_size, elem_size);
    }
    array->elems++;
}

void dast_array_extend(dast_array_t* array, void* objs, dast_u64_t n)
{
    char*             data = array->data;
    dast_allocator_t* allocator = array->allocator;
    dast_u64_t        elems = array->elems;
    dast_u64_t        elem_size = array->elem_size;
    dast_u64_t        i = 0;
    if (elems + n > array->capacity)
    {
        array->capacity = (elems + n) * array->factor;
        array->data = data = allocator->reallocate(allocator, data, array->capacity * elem_size);
    }
    while (i < n)
    {
        array->cpy((char*)objs + i * elem_size, data + (i + elems) * elem_size, elem_size);
        i++;
    }
    array->elems += n;
}

dast_u8_t dast_array_pop(dast_array_t* array, void* dst)
{
    if (array->elems == 0)
    {
        return 0;
    }
    if (dst)
    {
        array->cpy(array->data + (array->elems - 1) * array->elem_size, dst, array->elem_size);
    }
    array->del(array->data + (array->elems - 1) * array->elem_size);
    array->elems--;
    return 1;
}

dast_u8_t dast_array_remove(dast_array_t* array, dast_u64_t index, void* dst)
{
    if (index >= array->elems)
    {
        return 0;
    }
    dast_u64_t elems = array->elems;
    dast_u64_t elem_size = array->elem_size;
    dast_u64_t bytes_to_copy = (elems - 1 - index) * elem_size;
    if (dst)
    {
        array->cpy(array->data + index * elem_size, dst, elem_size);
    }
    array->del(array->data + index * elem_size);
    dast_move_generic(array->data + (index + 1) * elem_size, array->data + index * elem_size, bytes_to_copy);
    array->elems--;
    return 1;
}

dast_u8_t dast_array_replace(dast_array_t* array, void* obj, dast_u64_t index)
{
    if (index >= array->elems)
    {
        return 0;
    }

    array->del(array->data + index * array->elem_size);
    array->cpy(obj, array->data + index * array->elem_size, array->elem_size);
    return 1;
}

void dast_array_reverse(dast_array_t* array)
{
    dast_u64_t start = 0;
    dast_u64_t end = array->elems - 1;
    dast_u64_t elem_size = array->elem_size;
    char*      data = array->data;
    char       tmp[elem_size];
    while (start < end)
    {
        dast_cpy_generic(data + start * elem_size, tmp, elem_size);
        dast_cpy_generic(data + end * elem_size, data + start * elem_size, elem_size);
        dast_cpy_generic(tmp, data + end * elem_size, elem_size);
        start++;
        end--;
    }
}