# redblackdemo
In order to refresh outdated programming skills in C, I started this small project to demonstrate the use of the BSD Library functions covering __Red Black Trees__.

## Why rbtree?
Using programming languages like Java, Python or Swift, thanks to the support of collections there is little reason to bother with Red Black Trees. So I was curious to see what C had to offer me.

I did not found any samples showing the use of __sys/rbtree.h__. My only source of documentation is the man file. 

I was not able to find the source code of the implementation in the repositories of Darwin or BSD.

## My findings
A Red Black Tree is represented by a struct consisting of 8 opaque void pointers.

This structure contains among others:

  - the addresses of two exit functions, which have to be supplied by the user:
    - compare_nodes()
    - compare_key()
  - the address of an optional context area supplied by the user to be used as tunnel between the user of rbtree and the exit functions.
  - The offset of the internal part rb_node_t inside the structure of the nodes.
 
 The structure of the tree's nodes is defined by the user and must contain an opaque structure *rb_node_t*, which is maintained by the implementation. *rb_node_t* hosts the left/right pointers to the node's children.
 
## What does the demo do?

This program reads some text which is splitted into words.
These words are inserted as nodes into a Red Black Tree:
```c
    my_node_s* node = rb_tree_find_node(my_tree, word);
            
    if ( node ) { // word already in tree
         context.count_dup_key += 1;
    } else {      // insert into tree
         node = rb_tree_insert_node(my_tree, make_node(word));
    }
    node->count += 1;

```

The node's payload beside the key (the word) here has only a count of the occurence of the word.

After insertion of all words, the tree is examined and parsed:

```c
    for (my_node_s* node = RB_TREE_MIN(my_tree); node;
         node = rb_tree_iterate(my_tree, node, RB_DIR_RIGHT) ) {

        assert( node = rb_tree_find_node(my_tree, node->key) );
        // …
    }

```

The context area is used for counting the calls to the two exit functions and to determine the depth of the node: Before calling *rb_tree_find_node()* with repeatedly calls compare_key() the depth in the context area is resetted to zero and compare_key() increments depth at each call.

## sample run
The Kinks just entered the building, so lets entering some lines from a song:
```
~ % redblackdemo song.txt 
Start splitting by  ,.:;*{}()    <>-
Tree: Count of Nodes: 19
Tree's opaque pointers:
    0 :     0x7fc563d04d60    1 :     0x7ffee7a4e5c0
    2 :     0x7fc563d04fe0    3 :     0x7fc563d05060
    4 :               0x13    5 :                0x0
    6 :                0x0    7 :                0x0
Hexdump of Tree
    +0000 | 604DD063 C57F0000 C0E5A4E7 FE7F0000
    +0010 | E04FD063 C57F0000 6050D063 C57F0000
    +0020 | 13000000 00000000 00000000 00000000
    +0030 | 00000000 00000000 00000000 00000000
Context:
    Calls compare_nodes: 58
    Calls compare_key: 60
    Calls dup_key: 1
0x7fc563d04fe0 : d=4 1     The        :            0x0 :            0x0 : 0x7fc563d049b1 :
0x7fc563d049b0 : d=3 1     This       : 0x7fc563d04fe0 :            0x0 : 0x7fc563d04ef0 :
0x7fc563d04ef0 : d=2 1     a          : 0x7fc563d049b0 : 0x7fc563d05190 : 0x7fc563d04d60 :
0x7fc563d04dd0 : d=5 1     age        :            0x0 :            0x0 : 0x7fc563d051d1 :
0x7fc563d051d0 : d=4 1     and        : 0x7fc563d04dd0 : 0x7fc563d05210 : 0x7fc563d05190 :
0x7fc563d05210 : d=5 1     biological :            0x0 :            0x0 : 0x7fc563d051d3 :
0x7fc563d05190 : d=3 1     bombs      : 0x7fc563d051d0 : 0x7fc563d05150 : 0x7fc563d04ef3 :
0x7fc563d05150 : d=4 1     hydrogen   :            0x0 :            0x0 : 0x7fc563d05192 :
0x7fc563d04d60 : d=1 1     is         : 0x7fc563d04ef0 : 0x7fc563d04e50 : 0x7fc563d04d20 :
0x7fc563d04e90 : d=4 1     machinery  :            0x0 :            0x0 : 0x7fc563d04f30 :
0x7fc563d04f30 : d=3 1     mechanical : 0x7fc563d04e90 : 0x7fc563d04f70 : 0x7fc563d04e51 :
0x7fc563d05110 : d=5 1     napalm     :            0x0 :            0x0 : 0x7fc563d04f71 :
0x7fc563d04f70 : d=4 1     nightmare  : 0x7fc563d05110 :            0x0 : 0x7fc563d04f32 :
0x7fc563d04e50 : d=2 2     of         : 0x7fc563d04f30 : 0x7fc563d05020 : 0x7fc563d04d62 :
0x7fc563d050a0 : d=5 1     technology :            0x0 :            0x0 : 0x7fc563d04d91 :
0x7fc563d04d90 : d=4 1     the        : 0x7fc563d050a0 : 0x7fc563d05250 : 0x7fc563d05020 :
0x7fc563d05250 : d=5 1     warfare    :            0x0 :            0x0 : 0x7fc563d04d93 :
0x7fc563d05020 : d=3 1     wonderful  : 0x7fc563d04d90 : 0x7fc563d05060 : 0x7fc563d04e53 :
0x7fc563d05060 : d=4 1     world      :            0x0 :            0x0 : 0x7fc563d05022 :
Depth   1 :     1 Nodes     1 accumulated (5.3%)
Depth   2 :     2 Nodes     3 accumulated (15.8%)
Depth   3 :     4 Nodes     7 accumulated (36.8%)
Depth   4 :     7 Nodes    14 accumulated (73.7%)
Depth   5 :     5 Nodes    19 accumulated (100.0%)

```
The node's print line consist of

  - Its address
  - Its depth in the tree
  - the node's occurrence
  - the node's key
  - first opaque pointer: address of left child or zero
  - second opaque poitner: address of right child or zero
  - third opaque pointer: address of its parent, last two bit seems to indicate node's color or status (?).
  
Now we can draw the structure of the Red Black Tree. The last two bits of the the opaque pointer are mapped to the color of the nodes:

<img width="734" alt="resulting tree, root at bottom, leafs at top" src="https://user-images.githubusercontent.com/55148527/118361308-202f8980-b58b-11eb-921e-f9e70fedf837.png">

From there we decipher the structure rb_tree_t:

  - First pointer points to the root of the tree.
  - Second pointer holds the address of the option area.
  - Third pointer points to the node with the smallest key.
  - 4th pointer points to the node with the largest key.
  - 5th pointer holds the count of nodes in tree.
  
  
