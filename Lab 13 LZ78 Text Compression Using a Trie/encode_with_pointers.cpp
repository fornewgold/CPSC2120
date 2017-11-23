#include <iostream>
#include <cstdio>
using namespace std;

// A node in our encoding trie
struct Node {
  int id;   
  char letter; // the character labeling the edge to this node's parent
  Node *first_child;
  Node *next_sibling;
  Node (int _id, char _letter, Node *_next_sibling) {
    id = _id; letter = _letter; next_sibling = _next_sibling;
    first_child = NULL;
  }
};

int main(void)
{
  Node *root = new Node(0, 0, NULL);
  Node *curnode = root;
  int max_id = 0, c;

  do {
    // read one character from standard input, whitespace included
    c = cin.get();  

    // walk down the tree, adding a child if we get stuck
    Node *n = curnode->first_child;
    while (n != NULL && n->letter != c) n = n->next_sibling;
    if (n != NULL) curnode = n; // if we found a child matching c, step to it
    else {
      // add c as a new child and print out its information.  reset to root
      curnode->first_child = new Node(++max_id, c, curnode->first_child);
      cout << curnode->id << " " << c << "\n";
      curnode = root;
    } 
  } while (c != EOF);
  return 0;
}
