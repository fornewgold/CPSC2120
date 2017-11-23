/*
  CPSC 2120
  Xi Zhao
  9/24/2014
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

using namespace std;

struct Node {
  int key;
  int size;
  Node *left;
  Node *right;
  Node (int k) { key = k; size = 1; left = right = NULL; }
};

void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

/*
// insert key k into tree T, returning a pointer to the resulting tree
Node *insert(Node *T, int k)
{
  if (T == NULL) return new Node(k);
  if (k < T->key) T->left = insert(T->left, k);
  else T->right = insert(T->right, k);
  fix_size(T);
  return T;
}
*/

// insert value v at rank r
Node *insert(Node *T, int v, int r)
{
  // TBD: fill this part in
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

/*
// return a pointer to the node with key k in tree T, or NULL if it doesn't exist
Node *find(Node *T, int k)
{
  if (T == NULL) return NULL;
  if (k == T->key) return T;
  if (k < T->key) return find(T->left, k);
  else return find(T->right, k);
}
*/

// return pointer to node of rank r (with r'th largest key; e.g. r=0 is the minimum)
Node *select(Node *T, int r)
{
  assert(T!=NULL && r>=0 && r<T->size);

  int rank_of_root = T->left ? T->left->size : 0;
  if (r == rank_of_root) return T;
  if (r < rank_of_root) return select(T->left, r);
  else return select(T->right, r - rank_of_root - 1);
}

/*
// Join trees L and R (with L containing keys all <= the keys in R)
// Return a pointer to the joined tree.  
Node *join(Node *L, Node *R)
{
  // choose either the root of L or the root of R to be the root of the joined tree
  // (where we choose with probabilities proportional to the sizes of L and R)
  if (L == NULL) return R;
  if (R == NULL) return L;
  int random = rand() % (L->size + R->size);
  if (random < L->size) {
    // L stays root
    L->right = join(L->right, R);
    fix_size(L);
    return L;
  } else {
    // R stays root
    R->left = join(L, R->left);
    fix_size(R);
    return R;
  }
}
*/

/*
// remove key k from T, returning a pointer to the resulting tree.
// it is required that k be present in T
Node *remove(Node *T, int k)
{
  assert(T != NULL);
  if (k == T->key) {
    Node *to_delete = T;
    T = join(T->left, T->right);
    delete to_delete;
    return T;
  }
  if (k < T->key) T->left = remove(T->left, k);
  else T->right = remove(T->right, k);
  fix_size(T);
  return T;  
}
*/

/*
// Split tree T on key k into tree L (containing keys <= k) and a tree R (containing keys > k)
void split(Node *T, int k, Node **L, Node **R)
{
  if (T == NULL) {
    *L = NULL;
    *R = NULL;
    return;
  }
  if (k < T->key) {
    // recursively split left subtree
    split(T->left, k, L, &T->left);
    *R = T;
  } else {
    split(T->right, k, &T->right, R);
    *L = T;
  }
  fix_size(T);
}
*/

// Split tree T on rank r into tree L (containing ranks < r) and 
// a tree R (containing ranks >= r)
void split(Node *T, int r, Node **L, Node **R)
{
  // TBD: please fill in this function appropriately
  if (T == NULL) {
    *L = NULL;
    *R = NULL;
    return;
  }
  
  assert(r >= 0 && r<= T->size);
  int rank_of_root = T->left ? T->left->size : 0;
  if (r <= rank_of_root) {
    split(T->left, r, L, &T->left);
    *R = T;
  } else {
    split(T->right, r - rank_of_root - 1, &T->right, R);
    *L = T;
  }
  fix_size(T);
}

/*
// insert key k into the tree T, returning a pointer to the resulting tree
Node *insert_random(Node *T, int k)
{
  // If k is the Nth node inserted into T, then:
  // with probability 1/N, insert k at the root of T
  // otherwise, insert_random k recursively left or right of the root of T
  if (T == NULL) return new Node(k);
  if (rand() % (T->size + 1) == 0) {
    // insert at root
    Node *new_root = new Node(k);
    split(T, k, &new_root->left, &new_root->right);
    fix_size(new_root);
    return new_root;
  }
  if (k < T->key) T->left = insert_random(T->left, k);
  else T->right = insert_random(T->right, k);
  fix_size(T);
  return T;
}
*/

// insert value v at rank r
Node *insert_random(Node *T, int v, int r)
{
  // If k is the Nth node inserted into T, then:
  // with probability 1/N, insert k at the root of T
  // otherwise, insert_random k recursively left or right of the root of T
  if (T == NULL) return new Node(v);
  if (rand() % (T->size + 1) == 0) {
    // insert at root
    Node *new_root = new Node(v);
    split(T, r, &new_root->left, &new_root->right);
    fix_size(new_root);
    return new_root;
  }
  // otherwise insert recursively...
  // TBD: you'll need to fill in this part
  assert(r >= 0 && r<= T->size);
  int rank_of_root = T->left ? T->left->size : 0;
  if (r <= rank_of_root) T->left = insert_random(T->left, v, r);
  else T->right = insert_random(T->right, v, r - rank_of_root - 1);
  fix_size(T);
  return T;
}

// Returns true if team x defeated team y
bool did_x_beat_y(int x, int y)
{
  assert (x != y);
  if (x > y) return !did_x_beat_y(y,x);
  unsigned long long lx = x;
  unsigned long long ly = y;
  return ((17 + 8321813 * lx + 1861 * ly) % 1299827) % 2 == 0;
}

// Return a BST containing a valid ordering of n teams
Node *order_n_teams(int n)
{
  Node *T = NULL;

  // start by inserting the first team
  T = insert(T, 0, 0);

  // now insert the other teams...
  for (int i=1; i<n; i++) {
    // insert team i so the sequence encoded by the BST remains valid
    if (did_x_beat_y(i, select(T,0)->key))
      T = insert(T, i, 0); // can we insert at beginning?
    else 
      if (did_x_beat_y(select(T,T->size-1)->key, i))
        T = insert(T, i, T->size); // can we insert at end?
      else {
        // nope -- now we need to binary search for correct position
        // TBD: please fill in this part...
        int start = 0, end = T->size - 1;
        while (end - start != 1) {
          int mid = (start + end) / 2;
          if(did_x_beat_y(i, select(T, mid)->key))
            end = mid;
          else
            start = mid;
        }
        T = insert(T, i, end);
      }
  }
  return T;
}

int main(void)
{
  Node *T = NULL;

  // test insert at beginning
  for (int i=0; i<5; i++)
    T = insert_random(T, i+1, 0);
  cout << "Tree should contain 5 4 3 2 1:\n";
  print_inorder(T);

  // test insert at end
  for (int i=5; i<10; i++)
    T = insert_random(T, i+1, T->size);
  cout << "Tree should contain 5 4 3 2 1 6 7 8 9 10:\n";
  print_inorder(T);
  
  // test insert at middle
  for (int i=10; i<15; i++)
    T = insert_random(T, i+1, T->size/2);
  cout << "Tree should contain 5 4 3 2 1 12 14 15 13 11 6 7 8 9 10:\n";
  print_inorder(T);

  // once insert is working, the next step is to build the
  // insert_random function -- to test this, just change
  // calls to insert above to insert_random.

  int N = 10; // this should run quickly even for very large N!
  Node *S = order_n_teams(N);
  if (S == NULL || S->size != N)
    cout << "Size of tree returned by order_n_teams is wrong\n";
  else {
    cout << "Team ordering:\n";
    print_inorder(S);
    for (int i=0; i<N-1; i++) {
      Node *x = select(S, i);
      Node *y = select(S, i+1);
      if (!did_x_beat_y(x->key, y->key)) {
        cout << "Invalid sequence: team " << x->key << " (position " << i << ") lost to team " << y->key << " (position " << i+1 << ")\n";
      }
    }
  }  
  
  return 0;
}
