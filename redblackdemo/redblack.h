//
//  redblack.h
//  redblack
//

#ifndef redblack_h
#define redblack_h
#include <sys/rbtree.h>

typedef struct my_node_s {
    rb_node_t my_node_internal;
    const char* key;
    size_t count;
} my_node_s;
typedef struct my_context_s {
    size_t count_compare_nodes;
    size_t count_compare_key;
    size_t count_dup_key;
    size_t depth;
} my_context_s;

signed int my_compare_nodes(
    void* context,
    const void * a,
    const void* b)
;

signed int my_compare_key(
    void* context,
    const void* a,
    const void* b)
;

my_node_s* make_node(const char the_key[]);
void free_node(my_node_s* node);
void print_node(my_node_s* node, size_t depth);
void print_tree(rb_tree_t* rbt);

#endif /* redblack_h */
