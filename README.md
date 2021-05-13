# redblackdemo
In order to refresh outdated programming skills in C, I started this small project to demonstrate the use of the BSD Library functions covering __Red Black Trees__.

## Why rbtree?
Using programming languages like Java, Python or Swift, thanks to the support of collections there is little reason to bother with Red Black Trees. So I was curious to see what C had to offer me.

I did not found any samples showing the use of sys/rbtree.h. My only source of documentation is the man file. 
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
## sample run

```
~ % redblack
Enter some text lines…
Start splitting by  ,.:;*{}()	<>-
a b c d e f g h i j k l
a b b c c c d d d d e e
Tree: Count of Nodes: 12
Tree's opaque pointers:
	0 :     0x7fa36a405d60	1 :     0x7ffee89f7620
	2 :     0x7fa36a405950	3 :     0x7fa36a405f90
	4 :                0xc	5 : 0xc000000000000000
	6 : 0x742d7465672e0008	7 : 0x6f6c6c612d6b7361
Hexdump of Tree
	+0000 | 605D406A A37F0000 20769FE8 FE7F0000
	+0010 | 5059406A A37F0000 905F406A A37F0000
	+0020 | 0C000000 00000000 00000000 000000C0
	+0030 | 08002E67 65742D74 61736B2D 616C6C6F
Context:
	Calls compare_nodes: 37
	Calls compare_key: 65
	Calls dup_key: 12
0x7fa36a405950 : d=3 2 	a                                :               0x0 :               0x0 :    0x7fa36a405d00 :
0x7fa36a405d00 : d=2 3 	b                                :    0x7fa36a405950 :    0x7fa36a405d30 :    0x7fa36a405d60 :
0x7fa36a405d30 : d=3 4 	c                                :               0x0 :               0x0 :    0x7fa36a405d02 :
0x7fa36a405d60 : d=1 5 	d                                :    0x7fa36a405d00 :    0x7fa36a405e90 :    0x7fa36a405cc0 :
0x7fa36a405df0 : d=4 3 	e                                :               0x0 :               0x0 :    0x7fa36a405e20 :
0x7fa36a405e20 : d=3 1 	f                                :    0x7fa36a405df0 :    0x7fa36a405e50 :    0x7fa36a405e91 :
0x7fa36a405e50 : d=4 1 	g                                :               0x0 :               0x0 :    0x7fa36a405e22 :
0x7fa36a405e90 : d=2 1 	h                                :    0x7fa36a405e20 :    0x7fa36a405f10 :    0x7fa36a405d62 :
0x7fa36a405ed0 : d=4 1 	i                                :               0x0 :               0x0 :    0x7fa36a405f10 :
0x7fa36a405f10 : d=3 1 	j                                :    0x7fa36a405ed0 :    0x7fa36a405f50 :    0x7fa36a405e93 :
0x7fa36a405f50 : d=4 1 	k                                :               0x0 :    0x7fa36a405f90 :    0x7fa36a405f12 :
0x7fa36a405f90 : d=5 1 	l                                :               0x0 :               0x0 :    0x7fa36a405f53 :
Depth   1 :     1 Nodes     1 accumulated (8.3%)
Depth   2 :     2 Nodes     3 accumulated (25.0%)
Depth   3 :     4 Nodes     7 accumulated (58.3%)
Depth   4 :     4 Nodes    11 accumulated (91.7%)
Depth   5 :     1 Nodes    12 accumulated (100.0%)

```

You can see from the print of the nodes, how the nodes are connected in the resulting Tree:

![rb-sample](https://user-images.githubusercontent.com/55148527/118146754-357fa900-b40f-11eb-97ea-bb9f3f8d2ff9.png)
