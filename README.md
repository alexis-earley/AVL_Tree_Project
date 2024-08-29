This was the first of three projects I worked on in my Data Structures course during my undergraduate degree. It was based on creating an AVL Tree, a binary search tree that is balanced
for easier traversing. In this project, it consisted of nodes containing hypothetical students' names and ID numbers. I had to use my knowledge of data structures to insert and delete 
nodes, balancing the tree afterwards. Other features included searching the tree, returning the number of levels, and printing all of the nodes in various orders (preorder, inorder, 
and postorder).

This furthered my understanding of concepts such as:
- Pointers and references
    - Each node pointed to its previous and next node
    - These parameters constantly had to be adjusted during tree manipulation
- Creating test cases, especially edge cases
    - Our grade included 10 test cases that were unknown to us
    - We had to be robust and think of all possibe scenarios to make sure our code ran properly
  - The math behind data structures as a whole
      - In the case of AVL trees, they must maintain a threshold of +/- 1 to be balanced
      - In other words, one bottom branch of the AVL tree cannot dip more than one level below the other
      - In cases where the balance factor becomes 2, the tree must be rearranged
      - Each branch can be rearranged in four different ways: left, right, left-right, and right-left
      - In some cases, the entire tree might be rearranged, with elements shifting multiple times with varying rotations
   
  Score: 100 / 100.
