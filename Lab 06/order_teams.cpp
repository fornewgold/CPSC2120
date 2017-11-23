/*
  CPSC 2120
  Xi Zhao
  10/1/2014
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <assert.h>
#include <set>

using namespace std;

struct Node {
  int val;
  Node *next;
  Node (int v, Node *n) { val = v; next = n; }
};

// Returns true if team x defeated team y
bool did_x_beat_y(int x, int y)
{
  if (x > y) return !did_x_beat_y(y,x);
  unsigned long long lx = x;
  unsigned long long ly = y;
  return ((17 + 8321813 * lx + 1861 * ly) % 982451653) % 2 == 0;
}

// TBD: Please change this to work in a non-recursive fashion.
// The current recursive implementation works but it runs out of 
// stack space for large problem sizes... (a pity, since it's so 
// simple to write this function recursively...)
Node *merge(Node *left, Node *right)
{
  //if (left == NULL) return right;
  //if (right == NULL) return left;
  //if (did_x_beat_y(left->val, right->val)) {
    //left->next = merge(left->next, right);
    //return left;
  //} else {
    //right->next = merge(right->next, left);
    //return right;
  //}
  
  Node *head = new Node(0, NULL);
  Node *it_left = left, *it_right = right, *it = head;

  while (it_left != NULL || it_right != NULL) {  
    if (it_left == NULL) {
      it->next = it_right;
      break;
    } else if (it_right == NULL) {
      it->next = it_left;
      break;
    } else {
      if (did_x_beat_y(it_left->val, it_right->val)) {
        it->next = it_left;
        it = it->next;
        it_left = it_left->next;
    } else{
        it->next = it_right;
        it = it->next;
        it_right = it_right->next;
      }
    }
  }
  
  Node *to_delete = head;
  head = head->next;
 	delete to_delete;
  return head;
}

// Return a linked list containing a valid ordering of teams start..end
Node *list_ordering(int start, int end)
{
  if (start == end) {
    return new Node(start, NULL);
  }
  int mid = (start+end)/2;
  Node *left = list_ordering(start,mid);
  Node *right = list_ordering(mid+1,end);
  return merge(left, right);
}

// Return an array containing a valid ordering of teams start..end
int *array_ordering(int start, int end)
{
  // TBD... the following line is here just to prevent a seg fault when
  // running initially; it returns an array of the correct size but filled
  // with garbage values, so it won't pass the checks in main()...
  //return new int[end-start+1];
  
  if (start == end) {
    int *a = new int [1];
    a[0] = start;
    return a;
  }
  
  int mid = (start + end) / 2;
  int size_left = mid - start + 1, size_right = end - mid, size_all = end - start + 1;
  
  int *left = array_ordering(start, mid);
  int *right = array_ordering(mid + 1, end);
  
  int it_left = 0, it_right = 0;
  int *all = new int [size_all];
  for (int i = 0; i < size_all; i++) {
    if (it_left == size_left) {
      all[i] = right[it_right];
      it_right++;
    } else if (it_right == size_right) {
      all[i] = left[it_left];
      it_left++;
    } else {  
      if (did_x_beat_y(left[it_left], right[it_right])) {
        all[i] = left[it_left];
        it_left++;
      } else {
        all[i] = right[it_right];
        it_right++;
      }      
    }
  }
  
  delete [] left, right;
  return all;
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cout << "You need to specify the number of teams on the command line\n";
    return 0;
  }

  int Nteams = atoi(argv[1]);
  cout << "Testing with " << Nteams << " teams.\n";

  cout << "Checking list-based implementation...\n";
  Node *L = list_ordering(0,Nteams-1);
  bool correct = true;
  set<int> teams_used;
  for (int i=0; i<Nteams-1; i++) {
    if (L == NULL || L->next == NULL) {
      cout << "List appears to be too short!\n";
      correct = false; break;
    } else if (L->val==L->next->val || teams_used.count(L->val)>0) {
      cout << "Team " << L->val << " appears twice in your ordering!\n";
      correct = false; break;
    } else if (L->val<0 || L->val>=Nteams) {
      cout << "Invalid team " << L->val << " appears in your ordering!\n";
      correct = false; break;
    } else if (!did_x_beat_y(L->val, L->next->val)) {
      cout << "Invalid order: team " << L->val << " (position " << i <<
	") lost to team " << L->next->val << " (position " << i+1 << ")\n";
      correct = false; break;
    }
    teams_used.insert(L->val);
    Node *to_delete = L;
    L = L->next;
    delete to_delete; // free elements of list as we go...
  }
  if (L != NULL && L->next != NULL) {
    cout << "List appears too long!\n";
    correct = false;
  }
  if (correct) cout << "Correct answer!\n";
  if (L) delete L; // delete last element

  cout << "Checking array-based implementation...\n";  
  int *A = array_ordering(0,Nteams-1);
  correct = true;
  teams_used.clear();
  for (int i=0; i<Nteams-1; i++) {
    if (A[i]==A[i+1] || teams_used.count(A[i])>0) {
      cout << "Team " << A[i] << " appears twice in your ordering!\n";
      correct = false; break;
    } else if (A[i]<0 || A[i]>=Nteams) {
      cout << "Invalid team " << A[i] << " appears in your ordering!\n";
      correct = false; break;
    } else if (!did_x_beat_y(A[i], A[i+1])) {
      cout << "Invalid order: team " << A[i] << " (position " << i <<
	") lost to team " << A[i+1] << " (position " << i+1 << ")\n";
      correct = false; break;
    }
    teams_used.insert(A[i]);
  }
  if (correct) cout << "Correct answer!\n";
  delete [] A; // free memory used by array
  
  return 0;
}
