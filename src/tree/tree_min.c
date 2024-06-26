#include "tree/tree.h"
#include "tree/tree_private.h"

dast_knot_t* dast_knot_min(dast_knot_t* knot)
{
    if (knot)
    {
        while (knot->left)
        {
            knot = knot->left;
        }
    }
    return knot;
}
void* dast_tree_min(dast_tree_t* tree)
{
    if (!(tree->min))
    {
        return 0;
    }
    return (char*)(tree->min) + sizeof(dast_knot_t);
}