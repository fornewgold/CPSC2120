/*
  CPSC 2120
  Xi Zhao
  9/4/2014
*/

#include <iostream>
#include <string.h>
#include <assert.h>
#include "stringset.h"

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int hash(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

/* Allocate a table of pointers to nodes, all initialized to NULL */
Node **allocate_table(int size)
{
  Node **table = new Node *[size];
  for (int i=0; i<size; i++)
    table[i] = NULL;
  return table;
}

Stringset::Stringset()
{
  size = 4; // initial size of table    
  table = allocate_table(size);
  num_elems = 0; 
}

Stringset::~Stringset()
{
  for (int i=0; i<size; i++) {
    while (table[i] != NULL) {
      Node *temp = table[i];
      table[i] = table[i]->next;
      delete temp;
    }
  }
  delete[] table;
}

/* Return true if key is in the set */
bool Stringset::find(string key)
{
  int h = hash(key, size);
  Node *n = table[h];
  while (n != NULL) {
    if (n->key == key) return true;
    n = n->next;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Stringset::insert(string key)
{
  assert (!find(key));
  num_elems++;

  if (num_elems == size) {
    // TBD: Expand table -- allocate new table of twice the size,
    // re-insert all keys into new table, and de-allocate old table.
    // (you may want to wait and add this code last, once everything
    // else is working, since the class will still function properly,
    // albeit slowly, without this part)
    int new_size = 2 * size;
    Node **new_table = allocate_table(new_size); // Create new table
    
    // Move keys into new table
    for (int i = 0; i < size; i++) {
      while (table[i] != NULL) {
        Node *n = table[i];
        int new_h = hash(table[i]->key, new_size);
        new_table[new_h] = new Node(table[i]->key, new_table[new_h]);
        table[i] = table[i]->next;
        delete n;
      }
    }
    
    int new_h = hash(key, new_size); 
    new_table[new_h]  = new Node(key, new_table[new_h]); 
    
    // De-allocate old table
    delete[] table;
    table = new_table;
    size = new_size;
    return;    
  }

  // TBD: Insert new element
  int h = hash(key, size); 
  table[h]  = new Node(key, table[h]); 
}

/* Removes a key.  It is an error if key isn't in the set */
void Stringset::remove(string key)
{
  assert (find(key));
  num_elems--;

  // TBD: Delete element
  int h = hash(key, size);
  Node *n = table[h];
  
  // Remove the key if it is the first node in the linked list
  if (n->key == key) {
    table[h] = n->next;
    delete n;
    return;
  }
  
  // Remove the key if it is not the first node in the linked list
  else {
    Node *m = n; // The previous node
    n = n->next;
  
	while (n != NULL) {
      if (n->key == key) {
        m->next = n->next;
        delete n;
        return;
      }
      n = n->next;
      m = m->next;
    } ;
  }
}

void Stringset::print(void)
{
  // TBD: Print contents of table
  cout << "Total number of strings are: " << num_elems << endl;
  
  for (int i = 0; i < size; i++) {
    cout << "The linked list [" << i << "]: ";

    Node *n = table[i];
    
    while (n != NULL) {
      cout << " " << n->key;
      n = n->next;
    }
    cout << endl;
  }
}
