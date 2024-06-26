#ifndef __DAST_ITERATOR_H__
#define __DAST_ITERATOR_H__

#include "ntype.h"

typedef struct _dast_iterator_t dast_iterator_t;

typedef struct _dast_iterator_t
{
    void* elem; // use it to get current element
    void (*next)(dast_iterator_t* self);
    void (*prev)(dast_iterator_t* self);
    void (*reset)(dast_iterator_t* self);
} dast_iterator_t;

#endif /* __DAST_ITERATOR_H__ */