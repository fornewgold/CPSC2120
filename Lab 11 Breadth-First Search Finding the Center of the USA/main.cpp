/*
  CPSC 2120
  Xi Zhao
  11/12/2014
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#define INF 65535

using namespace std;

struct State
{
	string name;
	vector<string> neighbors;
	State *pred;
	int dist, eccentricity;	
	State (void)
	{
		eccentricity = 0;
	}
};

map<string, State*> Nodes;
typedef pair<int, string> result;
vector<result> Results;

void input(void)
{
	fstream fin;
	fin.open("usa_48_state_graph.txt");
	if (!fin.is_open())
	{
		cerr << "Cannot open input file" << endl;
		return;
	}

	string x, y;
	while (fin >> x >> y)
	{
		if (Nodes.find(x) == Nodes.end())
		{
			Nodes[x] = new State;
			Nodes[x]->name = x;
		}
		Nodes[x]->neighbors.push_back(y);

		if (Nodes.find(y) == Nodes.end())
		{
			Nodes[y] = new State;
			Nodes[y]->name = y;
		}
		Nodes[y]->neighbors.push_back(x);
	}
	fin.close();
	
	// Print input
	cout << "Neighbor list:" << endl;
	for (map<string, State*>::iterator it_1 = Nodes.begin(); it_1 != Nodes.end(); it_1++)
	{
		cout << it_1->first << ":";
		for (vector<string>::iterator it_2 = it_1->second->neighbors.begin(); it_2 != it_1->second->neighbors.end(); it_2++)
			cout << " " << *it_2;
		cout << endl;
	}
}

void breadth_first_search()
{
	cout << "Computing eccentricity..." << endl;
	for (map<string, State*>::iterator it_1 = Nodes.begin(); it_1 != Nodes.end(); it_1++)
	{
		for (map<string, State*>::iterator it_2 = Nodes.begin(); it_2 != Nodes.end(); it_2++)
		{
			it_2->second->pred = NULL;
			it_2->second->dist = INF;
		}
		
		queue<State*> q;
		q.push(it_1->second);
		it_1->second->dist = 0;

		while (!q.empty())
		{
			State *it_3 = q.front();
			q.pop();
			for (vector<string>::iterator it_4 = it_3->neighbors.begin(); it_4 != it_3->neighbors.end(); it_4++)
			{
				if (Nodes[*it_4]->dist == INF)
				{
					Nodes[*it_4]->pred = it_3;
					Nodes[*it_4]->dist = it_3->dist + 1;
					if (Nodes[*it_4]->dist > it_1->second->eccentricity)
						it_1->second->eccentricity = Nodes[*it_4]->dist;
					q.push(Nodes[*it_4]);
				}
			}
		}
	}
}

void sort_results(void)
{
	for (map<string, State*>::iterator it = Nodes.begin(); it != Nodes.end(); it++)
	{
		Results.push_back(result(it->second->eccentricity, it->first));
	}
	sort(Results.begin(), Results.end());
}

void output()
{
	for (vector<result>::iterator it = Results.begin(); it != Results.end(); it++)
		cout << it->second << ": " << it->first << endl;
}

int main(void)
{
	input();
	breadth_first_search();
	sort_results();
	output();	
	return 0;
}
