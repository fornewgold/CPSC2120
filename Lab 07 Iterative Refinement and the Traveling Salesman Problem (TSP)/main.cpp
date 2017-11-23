/*
  CPSC 2120
  Xi Zhao
  10/15/2014
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

struct Point 
{
  int key;
  double x, y;
  Point(int _key, double _x, double _y) 
  {
    key = _key;
    x = _x;
    y = _y;
  }
};

const int size = 50;
double min_length;
Point **A, **B;

void allocate_table(void)
{
  A = new Point *[size];
  for (int i = 0; i < size; i++)
    A[i] = NULL;
    
  B = new Point *[size];
  for (int i = 0; i < size; i++)
    B[i] = NULL;
}

void free_table(void)
{
  for (int i = 0; i < size; i++)
    delete A[i];
  delete[] A;
  
  for (int i = 0; i < size; i++)
    delete B[i];
  delete[] B;
}

void read_input(Point** P)
{
  ifstream fin;
  fin.open ("tsp_points.txt");
  double x, y;
  int i = 0;
  while (fin >> x >> y) 
  {
    P[i] = new Point(i, x, y);
    i++;
  }
  fin.close();
}

void random_tour(Point** P)
{  
  for (int i = 1; i < size; i++)
  {
    int j = rand() % i;
    Point *temp = P[i];
    P[i] = P[j];
    P[j] = temp;
  }
}

double dist(const Point p, const Point q)
{
  double dx = p.x-q.x, dy = p.y-q.y;
  return sqrt(dx*dx + dy*dy);
}

double get_length(Point** P)
{
  double length = 0;  
  for (int i = 0; i < size - 1; i++)
    length += dist(*P[i], *P[i + 1]);
  return length += dist(*P[size - 1], *P[0]);
}

void reverse(int s, int e)
{
  while (s < e)
  {
    Point * temp = A[s];
    A[s] = A[e];
    A[e] = temp;
    s++;
    e--;
  }
}

bool refine_tour(Point** P, Point** Q)
{
  for (int i = 0; i < size; i++)
    for (int j = i + 1; j < size; j++)
    {
      reverse(i, j);
      double new_length = get_length(P);
      if (new_length < min_length)
      {
        min_length = new_length;
        for (int k = 0; k < size; k++)
          Q[k] = P[k];
        return true;
      }
      reverse(i, j);
    }    
  return false;
}

void output_tour(Point** P)
{
  cout << "The best distance is : " << get_length(P) << " with tour:"<< endl;
  for (int i = 0; i < size; i++)
    cout << P[i]->key << " ";
  cout << endl;
}

int main(void)
{
  srand(time(NULL));
  allocate_table();
  read_input(A);
  min_length = get_length(A);

  for (int i = 0; i < 1000; i++) 
  {
    random_tour(A);
    while (refine_tour(A, B));
  }
  output_tour(B);
  free_table();
}
