#include "tree/tree.h"
#include "tree/tree_private.h"

void dast_tree_destroy_from(dast_tree_t* tree)
{
    dast_allocator_t* allocator = tree->allocator;
    dast_del_t        del = tree->del;
    dast_knot_t**     glob = &(tree->root);
    dast_knot_t*      curr;

    while (*glob)
    {
        curr = *glob;
        if ((*glob = curr->left))
        {
        }
        else if ((*glob = curr->right))
        {
        }
        else if (curr->parent)
        {
            *glob = curr->parent;
            if (curr->parent->left == curr)
            {
                (*glob)->left = 0;
            }
            else
            {
                (*glob)->right = 0;
            }
            del((char*)(curr) + sizeof(dast_knot_t));
            allocator->deallocate(allocator, curr);
        }
        else
        {
            del((char*)(curr) + sizeof(dast_knot_t));
            allocator->deallocate(allocator, curr);
        }
    }
}

void dast_tree_destroy(dast_tree_t* tree)
{
    dast_allocator_t* allocator = tree->allocator;
    dast_tree_destroy_from(tree);
    allocator->deallocate(allocator, tree);
}