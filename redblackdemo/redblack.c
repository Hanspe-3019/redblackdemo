//
//  redblack.c
//  redblack
//

#include <stdlib.h>
#include <sys/rbtree.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "redblack.h"

void hexdump(size_t len, unsigned char byte[]){
    char hexa[36];
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if ( ! (i % 4) ) {
            hexa[j++] = ' ';
        }
        sprintf(hexa+j, "%.02hhX", byte[i]);
        j += 2;
        assert ( j <= 36 );
        if ( j >= 36 ) {
            hexa[j] = '\0';
            size_t offset = (i / 16) * 16;
            printf("\t+%04zx |%s\n", offset, hexa);
            j = 0;
        }
    }
    if ( j > 0 ) {
        hexa[j] = '\0';
        size_t offset = (len / 16) * 16;
        printf("\t+%04zx |%s\n", offset, hexa);
    }
    
}

void print_tree(rb_tree_t* rbt) {
    printf("Tree: Count of Nodes: %zu\n", rb_tree_count(rbt));
    printf("Tree's opaque pointers:");
    for (size_t i = 0; i < sizeof(rb_tree_t) / sizeof(void*); i++) {
        if ( ! (i%2) ) {
            printf("\n");
        }
        printf("\t%zu : %18p", i, rbt->opaque[i]);
    }
    printf("\nHexdump of Tree\n");
    hexdump(sizeof(rb_tree_t), ( void* ) rbt);
}
void print_node(my_node_s* node, size_t depth) {

    printf("%p : d=%zu %zu \t%-32s :", node, depth, node->count, node->key);
    for (size_t i = 0; i < sizeof(rb_node_t) / sizeof(void*); i++) {
        printf("%18p :", node->my_node_internal.opaque[i]);
    }
    printf("\n");

}
signed int my_compare_nodes(void* context, const void * a, const void* b) {
    const my_node_s* node1 = a;
    const my_node_s* node2 = b;
    my_context_s* con = context;
    con -> count_compare_nodes += 1;
    return strcmp(node1->key, node2->key);
}
signed int my_compare_key(void* context, const void* a, const void* b) {
    const my_node_s* node = a;
    my_context_s* con = context;
    con -> count_compare_key += 1;
    con -> depth += 1;
    return strcmp(node->key, b);
}
my_node_s* make_node(const char the_key[]) {

    my_node_s* node = malloc(sizeof(my_node_s));
    memset(node, '\0', sizeof(my_node_s));
    node->key = strdup(the_key);
    node->count = 0;
    return node;
}
void free_node(my_node_s* node) {
    free((void*) (node->key));
    free(node);
}
