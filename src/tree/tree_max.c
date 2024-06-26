#include "tree/tree.h"
#include "tree/tree_private.h"

dast_knot_t* dast_knot_max(dast_knot_t* knot)
{
    if (knot)
    {
        while (knot->right)
        {
            knot = knot->right;
        }
    }
    return knot;
}

void* dast_tree_max(dast_tree_t* tree)
{
    if (!(tree->max))
    {
        return 0;
    }
    return (char*)(tree->max) + sizeof(dast_knot_t);
}