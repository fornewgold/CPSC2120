/*
  CPSC 2120
  Xi Zhao
  8/18/2014
*/

#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

struct Node {
  int num;
  Node *previous;
  Node *next;
  Node (int a, Node *p, Node *n) { num = a; previous = p; next = n;}
};

class Intset {

 private: 
  //int size, allocated;
  //int *A;
  
  Node *begin, *end;

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif
