//
//  main.c
//  redblack
//
/*
 Some routines around libc's implementation of red Black trees.
 
 main: 1 optional arg: path to textfile to be parsed, default stdin
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/rbtree.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "redblack.h"

#define BUFFSIZE 256 // size of input-buffer
#define MAXDEPTH 20

int main(int argc, const char * argv[]) {

    FILE* input = stdin;
    if ( argc == 2 ) {
        input = fopen(argv[1], "r");
    } else {
        printf("Enter some text lines…\n");
    }
    assert ( input );
    
    /*
     * Red Black Tree is represented by a struct consisting of 8 opaque void pointers.
     * This structure contains
     * - the addresses of 2 functions supplied by the user:
     *   - compare_nodes
     *   - compare_key
     * - the address of an optional context area supplied by the user to be used for communication between main and the exit functions.
     * - The offset of the internal part rb_node_t inside the structure of the nodes.
     *
     * The structure of the tree's nodes is defined by the user and must contain an opace structure rb_node_t, which is maintained by the implementation. It hosts the left/right pointers to the node's children.
     */
    
    rb_tree_t* my_tree = malloc(sizeof(rb_tree_t));
    my_context_s context = {
        .count_compare_nodes = 0,
        .count_compare_key = 0,
    };
    rb_tree_ops_t my_ops = {
        .rbto_compare_key = my_compare_key,
        .rbto_compare_nodes = my_compare_nodes,
        .rbto_context = &context,
        .rbto_node_offset = 0,
    };
    
    rb_tree_init(my_tree, &my_ops);
    /*
     * Now the tree is ready to receive some nodes.
     * This program reads some text, with is splitted into words.
     * The node's payload beside the key (the word) here has only a count of the occurence of the word.
     */
    char* buffer = malloc(BUFFSIZE);
    const char sep[] = " ,.:;*{}()\t<>-";
    printf("Start splitting by %s\n", sep);

    while (fgets(buffer, BUFFSIZE, input)) {
        buffer[strlen(buffer)-1] = 0;
        char* line = strdup(buffer);
        char* tok;
        while ((tok = strsep(&line, sep)) != NULL) {
    
            if ( tok[0] == '\0' )  continue;
            if ( ! isalpha(tok[0]) ) continue;
            
            context.depth = 0;
            my_node_s* node = rb_tree_find_node(my_tree, tok);
            
            if ( node ) {
                context.count_dup_key += 1;
            } else {
                node = rb_tree_insert_node(my_tree, make_node(tok));
            }
            node->count += 1;
            // print_node(node, context.depth);
        }
        free(line);
    }

    /*
     * Now the tree ist filled with the words form the input file.
     * Inspecting and iterating the tree:
     */
    print_tree(my_tree);

    printf(
        "Context:"
        "\n\t" "Calls compare_nodes: %zu"
        "\n\t" "Calls compare_key: %zu"
        "\n\t" "Calls dup_key: %zu"
        "\n",
           context.count_compare_nodes,
           context.count_compare_key,
           context.count_dup_key
           );
    
    size_t depths[MAXDEPTH];
    for (size_t i = 0; i < MAXDEPTH; i++) depths[i] = 0;
    
    for (my_node_s* node = RB_TREE_MIN(my_tree); node;
         node = rb_tree_iterate(my_tree, node, RB_DIR_RIGHT)) {
        context.depth = 0;
        assert( node = rb_tree_find_node(my_tree, node->key) );
        print_node(node, context.depth);
        if ( context.depth < MAXDEPTH )
            depths[context.depth] += 1;
    }
    
    /*
     * Print distribution of depths in tree:
     */
    size_t accumulated = 0;
    size_t tree_count = rb_tree_count(my_tree);
    for (size_t i = 0; i < MAXDEPTH; i++) {
        if (depths[i]) {
            accumulated += depths[i];
            double percent = (accumulated * 100.) / tree_count;
            printf("Depth %3zu :"
                   " %5zu Nodes %5zu accumulated"
                   " (%.1f%%)\n",
                   i, depths[i], accumulated, percent);
        }
    }

}

