#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

struct Node
{
  int parent;
  char letter;
  Node (int _parent, int _letter)
  {
    parent = _parent;
    letter = _letter;
  }
};

vector<Node> nodes;

void print_string(int current)
{
  if (current == 0)
    return;
  print_string(nodes[current].parent);
  if (nodes[current].letter != -1)
    cout << nodes[current].letter;
}

int main(void)
{
  nodes.push_back(Node(0, -1));
  int a, b;
  while (cin >> a >> b)
  {
    nodes.push_back(Node(a, b));
    print_string(nodes.size() - 1);
  }
  return 0;
}
