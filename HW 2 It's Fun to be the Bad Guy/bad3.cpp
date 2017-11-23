/*
  CPSC 2120
  Xi Zhao
  10/3/2014
*/

#include <iostream>
#include <cstdlib>
#include <assert.h>

using namespace std;

struct Node {
  int key;
  int size;
  Node *left;
  Node *right;
  Node (int k) { key = k; size = 1; left = right = NULL; }
};

// fix size for the node
void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

// insert value v at rank r
Node *insert(Node *T, int v, int r)
{
  if (T == NULL) return new Node(v);
  assert(r >= 0 && r<= T->size);
  
  int rank_of_root = T->left ? T->left->size : 0;
  if (r <= rank_of_root) T->left = insert(T->left, v, r);
  else T->right = insert(T->right, v, r - rank_of_root - 1);
  fix_size(T);
  return T;
}

// prints out the inorder traversal of T (i.e., the contents of T in sorted order)
void print_inorder(Node *T)
{
  if (T == NULL) return;
  print_inorder(T->left);
  cout << T->key << "\n";
  print_inorder(T->right);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cout << "Usage: bad3 <input size>\n";
    return 0;
  }

  int N = atoi(argv[1]);  // get first command-line argument
  if (N<1 || N>100000) {
    cout << "Invalid input size!\n";
    return 0;
  }
  
  // Generate and print bad input of size N for prog1
  // (currently just generates an input of N random numbers)
  cout << N << "\n";
  
  Node *T = NULL;
  T= insert(T, N, 0);
  
  for (int i = 1; i < N; i++) {
    int value = N - i, rank = 123456789 % (i + 1);
    T = insert(T, value, rank);
  }
  
  print_inorder(T);
  
  return 0;
}
