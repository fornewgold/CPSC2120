/*
  CPSC 2120
  Xi Zhao
  11/6/2014
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define N 936

using namespace std;

vector<int> A[N]; // Adjacency list
int seq[N];
int min_sum = -1;

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
	for (int i = 0; i < N; i++)
		seq[i] = i;
	random_shuffle(&seq[0], &seq[N]);
}

/* Get solution for all wires */
int get_sum()
{
	int sum = 0;
	for (int i = 0; i < N; i++)
	for (vector<int>::iterator it = A[i].begin(); it < A[i].end(); it++)
		sum += (seq[i] - seq[*it])*(seq[i] - seq[*it]);
	return sum / 2;
}

/* Get solution for related wires */
int get_related_sum(int a, int b)
{
	int sum = 0;
	for (vector<int>::iterator it = A[a].begin(); it < A[a].end(); it++)
		sum += (seq[a] - seq[*it])*(seq[a] - seq[*it]);
	for (vector<int>::iterator it = A[b].begin(); it < A[b].end(); it++)
	{
		if (*it == a)
			continue;
		sum += (seq[b] - seq[*it])*(seq[b] - seq[*it]);
	}
	return sum;
}

/* Swap a and b if valid */
bool is_valid_swap(int a, int b)
{
	int before = 0, after = 0;
	before = get_related_sum(a, b);
	swap(seq[a], seq[b]);
	after = get_related_sum(a, b);
	if (before > after)
	{
		min_sum += (after - before);
		return true;
	}
	else
	{
		swap(seq[a], seq[b]);
		return false;
	}
}

/* Refine sequence with valid swap */
bool is_valid_refine(void)
{
	bool is_valid = false;
	for (int a = 0; a < N; a++)
		for (int b = a + 1; b < N; b++)
			if (is_valid_swap(a, b))
				is_valid = true;
	return is_valid;
}

int main(void)
{
	srand((unsigned int)time(NULL));
	read_input();

	while (min_sum < 0 || min_sum >= 1000000)
	{
		initial_sequence();
		min_sum = get_sum();
		while (is_valid_refine())
			cout << min_sum << endl;
		cout << "Found a local local optimum!" << endl;
	}

	cout << "Found a local optimum less than 1 million!" << endl;
	exit(0);
}
