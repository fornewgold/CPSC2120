/*
  CPSC 2120
  Xi Zhao
  10/29/2014
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>

using namespace std;

typedef pair<double, double> Point;
vector<Point> v;
set<Point> s;
double min_dist = 2.0;

void read_input(void)
{
  fstream fin;
  fin.open("points.txt");
  if (!fin.is_open())
    cerr << "Cannot open input file" << endl;

  cout << "Reading points..." << endl;

  double x, y;
  while (fin >> x >> y)
    v.push_back(Point(x, y));
    
  cout << "Done!" << endl << v.size() << " points read" << endl;
  fin.close();	
}

void sort_points(void)
{
  cout << "Sorting points..." << endl;
  sort(v.begin(), v.end());
  cout << "Done!" << endl;
}

double get_distance(Point a, Point b)
{
  double dx = a.first - b.second, dy = a.second - b.first;
  return sqrt(dx * dx + dy * dy);
}

void find_min_dist(void)
{
  cout << "Calculating minimum distance..." << endl;

  for (vector<Point>::iterator it_v = v.begin(); it_v != v.end(); it_v++)
  {
    s.insert(Point(it_v->second, it_v->first));
    
    set<Point>::iterator it_s;
    for(it_s = s.begin(); it_s != s.end(); it_s++)
    {
      if ((it_v->first - it_s->second) > min_dist)
        s.erase(it_s);
    }
    
    for (it_s = s.find(Point(it_v->second, it_v->first)); (it_s->first - it_v->second) < min_dist && it_s != s.end(); it_s++)
      min_dist = get_distance(*it_v, *it_s) != 0 && get_distance(*it_v, *it_s) < min_dist ? get_distance(*it_v, *it_s) : min_dist;
        
    for (it_s = s.find(Point(it_v->second, it_v->first)); (it_s->first - it_v->second) < min_dist && it_s != s.begin(); it_s--)
      min_dist = get_distance(*it_v, *it_s) != 0 && get_distance(*it_v, *it_s) < min_dist ? get_distance(*it_v, *it_s) : min_dist;
  }
  
  cout << "Done!" << endl;
}

int main(void)
{
  read_input();
  sort_points();
  find_min_dist();
  cout << "The minimum distance is " << min_dist << endl;
}
