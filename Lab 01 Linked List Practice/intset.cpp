/*
  CPSC 2120
  Xi Zhao
  8/18/2014
*/

#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset()
{
  //size = 0;
  //allocated = 10;  /* start with a 10-element array */
  //A = new int[10];
  
  begin = NULL;
  end = NULL;
}

Intset::~Intset()
{
  if (begin != NULL) {
    Node *it = begin;

    while (it->next != NULL) {
      it = it->next;
      delete it->previous;
    }
    delete it;

    begin = NULL;
    end = NULL;
  }
}

/* Return true if key is in the set */
bool Intset::find(int key)
{
  //int low = 0, high = size-1, mid;

  /* Binary search for key... */
  /*
  while (low <= high) {
    mid = (low + high) / 2;
    if (key == A[mid]) return true;
    if (key > A[mid]) low = mid+1;
    else high = mid-1;
  }
  return false;
  */
  
  if (begin == NULL) 
    return false;

  else {
    Node *it = begin;

    do {
      if (it->num == key)
        return true;
      it = it->next;
    } while (it != NULL);

    return false; 
  }  
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Intset::insert(int key)
{
  //int i;

  //assert (!find(key));

  /* Enlarge (by 2x) the underlying array if it isn't large enough... */
  /*
  if (size == allocated) {
    allocated *= 2;
    int *newA = new int[allocated];
    for (i=0; i<size; i++) newA[i] = A[i];
    delete[] A;
    A = newA;
  }
  */

  /* Shift up elements in array to make room for new element... */
  /*
  size++;
  i = size-1;
  while (i>0 && A[i-1]>key) {
    A[i] = A[i-1];
    i--;
  }
  A[i] = key;
  */

  if (begin == NULL) {
    Node *n = new Node(key, NULL, NULL);
    begin = n;
    end =n;
    return;
  }

  else if (begin == end) {
    if (begin->num < key) {

      Node *n = new Node(key, begin, NULL);
      begin->next = n;
      end = n;
      return;
    }

    else if (begin->num > key) {
      Node *n = new Node(key, NULL, begin);
      begin->previous = n;
      begin = n;
      return;
    }
  }

  else {
    if (end->num < key) {
      Node *n = new Node(key, end, NULL);
      end->next = n;
      end = n;
      return;
    }

    else if (begin->num > key) {
      Node *n = new Node(key, NULL, begin);
      begin->previous = n;
      begin = n;
      return;
    }

    else {
      Node *it = begin;

      for (it = begin; it != end; it = it->next) {
        if (it->next->num > key) {
          Node *n = new Node(key, it, it->next);
          it->next->previous = n;
          it->next = n;
          return;
        }
      }
    }
  }
}

/* Removes a key.  It is an error if key isn't in the set */
void Intset::remove(int key)
{
  //int i;

  //assert (find(key));

  /* Shift down elements in array to plug gap left by deleted element... */
  /*
  for (i=size-1; key!=A[i]; i--);
  while (i<size-1) {
    A[i] = A[i+1];
    i++;
  }
  size--;
  */

  if (begin == NULL)
    return;
  else {
    if (begin == end) {
      if (begin->num == key) {
        delete begin;
        begin = NULL;
        end = NULL;
        return;
      }
    }

    else {
      if (begin->num == key) {
        Node * temp = begin;
        begin = begin->next;
        begin->previous = NULL;
        delete temp;
        return;
      }
      if (end->num == key) {
        Node * temp = end;
        end = end->previous;
        end->next = NULL;
        delete temp;
        return;
      }
    }

    for (Node *it = begin; it != end; it = it->next) {
      if (it->num == key) {
        it->previous->next = it->next;
        it->next->previous = it->previous;

        if (it == begin)
          begin = it->next;

        delete it;
        return;
      }
    }
  }
}

void Intset::print(void)
{
  if (begin == NULL)
    return;
  for (Node *it = begin; it != end; it = it->next)
    cout << it->num << "\n";
  cout << end->num << "\n";
}
