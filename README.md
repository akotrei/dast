# **dast**

## **Fudamental Data Structures And Algorithms For ANSI C**

## Data structures

1. **array** (alike python list or c++ std::vector)
2. **list** (alike c++ std::list)
3. **set** (alike c++ std::unordered_set)
4. **table** (alike c++ std::unordered_map)
5. **tree** (alike c++ std::map)

## Algorithms

1. **bsearch** (binary search)
2. **qsort** (quick sort)
3. **msort** (merge sort)

## Interfaces

1. **iallocator** (memory allocation, reallocation, freeing)
2. **iiterator** (allows to iterate over structure that implements this interface)

## Utils

1. **cmp** (functions for objects comparing: pointers, integer, floats, ascii strings)
2. **cpy** (functions for objects copying: only shallow)
3. **hash** (functions for hash calculating: pointers, integers, floats, ascii strings)
4. **allocator** (base allocator that implements iallocator (wrapper around malloc free and realloc))
