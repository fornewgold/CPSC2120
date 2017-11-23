/*
  CPSC 2120
  Xi Zhao
  11/6/2014
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#define N 936

using namespace std;

vector<int> A[N]; // Adjacency list
typedef pair<double, int> wire;
set<wire> sorted;
double seq[N];
int inted[N];
int current_min = -1;
int a, b;

void read_input(void)
{
	fstream fin;
	fin.open("wires.txt");
	if (!fin.is_open())
	{
		cerr << "Cannot open input file" << endl;
		exit(1);
	}

	int x, y;
	while (fin >> x >> y)
	{
		A[x].push_back(y);
		A[y].push_back(x);
	}
	fin.close();
}

void initial_sequence(void)
{
	b = (rand() % (N - 1)) + 1;
	a = rand() % b;
	for (int i = 0; i < N; i++)
		seq[i] = i;
	swap(seq[a], seq[0]);
	swap(seq[b], seq[N - 1]);
}

/* Get solution for all wires */
template <class T>
double get_sum(const T *node)
{
	T sum = 0;
	for (int i = 0; i < N; i++)
		for (vector<int>::iterator it = A[i].begin(); it < A[i].end(); it++)
			sum += (node[i] - node[*it])*(node[i] - node[*it]);
	return sum / 2;
}

/* Get new position of a node based on connected nodes */
double get_new_position(int i)
{
	double sum = 0;
	for (vector<int>::iterator it = A[i].begin(); it != A[i].end(); it++)
		sum += seq[*it];
	return sum / A[i].size();
}

/* Update positions for all nodes */
void update_sequence()
{
	double temp[N];
	for (int i = 0; i < N; i++)
	{
		if (i == a || i == b)
			temp[i] = seq[i];
		else
			temp[i] = get_new_position(i);
	}	
	for (int i = 0; i < N; i++)
		seq[i] = temp[i];
}

/* Sort and ouput positions as integer */
void sort_to_int()
{	
	if(!sorted.empty())
		sorted.clear();
	for (int i = 0; i < N; i++)
		sorted.insert(wire(seq[i], i));	
	for (int i = 0; i < N; i++)
		inted[i] = distance(sorted.begin(), sorted.find(wire(seq[i], i)));
}

/* Refine sequence */
void refine_sequence(void)
{
	double old_sum = 0, new_sum=0;
	while (old_sum == 0 || old_sum - new_sum > 1)
	{
		old_sum = old_sum ? new_sum : get_sum(seq);
		update_sequence();
		new_sum = get_sum(seq);
	}
	sort_to_int();
	current_min = get_sum(inted);
}

int main(void)
{
	srand((unsigned int)time(NULL));
	read_input();

	while (current_min < 0 || current_min >= 1000000)
	{
		initial_sequence();
		refine_sequence();
		cout <<	"[a = " << a << ",b = " << b << "] " << get_sum(inted) << endl;
	}
		
	cout << "Found a local optimum less than 1 million!" << endl;
	exit(0);
}
