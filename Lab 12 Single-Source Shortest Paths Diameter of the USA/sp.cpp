/*
  CPSC 2120
  Xi Zhao
  11/19/2014
*/

#include <cstdio>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <iostream>
#define INF -1

using namespace std;

struct Edge {
  int src, dst;
  float travel_time;
};

int N, M;
int *first_edge;
double *cost;
Edge *E;

typedef pair<double, int> pfi;
priority_queue<pfi> q;

// compare two edges based on their source nodes 
// (break ties using destination nodes)
bool comp(Edge e1, Edge e2)
{
  if (e1.src == e2.src) return e1.dst < e2.dst;
  return e1.src < e2.src;
}

void build_index(void)
{
  first_edge = new int[N];
  for (int i = 0; i < N; i++)
    first_edge[i] = 0;
   
  int current_src = -1;
  for (int i = 0; i < M; i++)
  {
    if (current_src != E[i].src)
    {
      first_edge[E[i].src] = i;
      current_src = E[i].src;
    }
  }
}

double dijkstra(int s)
{
  cost = new double[N];
  for (int i = 0; i < N; i++)
    cost[i] = INF;
  cost[s] = 0;

  q.push(pfi(0, s));

  while (!q.empty())
  {
    pfi cpt = q.top();
    q.pop();
    for (int i = first_edge[cpt.second]; cpt.second == E[i].src; i++)
    {
      double alt = cost[E[i].src] + E[i].travel_time;
      if (alt < cost[E[i].dst] || cost[E[i].dst] == INF)
      {
        cost[E[i].dst] = alt;
        q.push(pfi(- alt, E[i].dst));
      }
    }
  }
  
  int max = 0;
  for (int i = 0; i < N; i++)
    max = max > cost[i] ? max : cost[i];  

  delete[] cost;
  
  return max;
}

void swap_edges()
{
  for (int i = 0; i < M; i++)
    swap(E[i].src, E[i].dst);
}

int main(void)
{
  FILE *fp = popen ("zcat /tmp/usa_network.txt.gz", "r");
  fscanf (fp, "%d %d", &N, &M);
  E = new Edge[M];
  printf ("Reading input");
  for (int i=0; i<M; i++) {
    fscanf (fp, "%d %d %f", &E[i].src, &E[i].dst, &E[i].travel_time);
    if (i % 1000000 == 0) { printf ("."); fflush(stdout); }
  }
  fclose(fp);
  printf (" Done.\n" ); fflush(stdout); 

  sort(E, E+M, comp);
  build_index();
  // The node right in front of McAdams Hall is number 17731931.
  int out  = dijkstra(17731931) /3600;
  
  swap_edges();
  sort(E, E+M, comp);
  build_index();
  int in = dijkstra(17731931) /3600;  ;
  
  cout << "Diameter between " << (out + in) / 2 << " and " << out + in << endl;
  
  delete[] first_edge;
  delete[] E;
  
  return 0;
}
