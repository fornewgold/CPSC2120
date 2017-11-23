/*
  CPSC 2120
  Xi Zhao
  12/3/2014
*/

#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

// A node in our encoding trie
struct Node {
  char letter; // the character labeling the edge to this node's parent
  int first_child; // -1 if none
  int next_sibling; // -1 if none
  Node (char _letter, int _next_sibling) {
    letter = _letter; next_sibling = _next_sibling; first_child = -1;
  }
};

int main(void)
{
  vector<Node> nodes;
  nodes.push_back(Node(0,-1));
  int curnode = 0, c;

  do {
    // read one character from standard input, whitespace included
    c = cin.get();  

    // walk down the tree, adding a child if we get stuck
    int n = nodes[curnode].first_child;
    while (n != -1 && nodes[n].letter != c) n = nodes[n].next_sibling;
    if (n != -1) curnode = n; // if we found a child matching c, step to it
    else {
      // add c as a new child and print out its information.  reset to root
      nodes.push_back(Node(c, nodes[curnode].first_child));
      nodes[curnode].first_child = nodes.size()-1;
      cout << curnode << " " << c << "\n";
      curnode = 0;
    } 
  } while (c != EOF);
  return 0;
}
