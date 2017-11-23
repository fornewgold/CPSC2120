/*
  CPSC 2120
  Xi Zhao
  12/3/2014
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

struct Point
{
	double label, pdcn;
	double *position;
};

struct Node 
{
	Point *key;
	Node *left;
	Node *right;
	Node(Point *k) { key = k; left = right = NULL; }
};

int K, N, D;
double alpha = 1.0;
Point *pts;
vector<Point*> V;
Node *KDT;
typedef pair<double, Point*> dist_point;
priority_queue<dist_point> PQ;

void read_input(char *filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cerr << "Cannot open input file" << endl;
		system("pause");
		return;
	}
	fin >> N >> D;
	pts = new Point[N];
	for (int i = 0; i < N; i++)
	{
		fin >> pts[i].label;
		pts[i].position = new double[D];
		for (int j = 0; j < D; j++)
			fin >> pts[i].position[j];
	}
	fin.close();
	cout << N << " points in " << D << " dimensions read!" << endl;
}

void normalize_input()
{
	for (int i = 0; i < D; i++)
	{
		// Computer mean and STD
		double mean = 0, variance = 0, STD;
		for (int j = 0; j < N; j++)
			mean += pts[j].position[i];
		mean /= N;

		for (int j = 0; j < N; j++)
			variance += (pts[j].position[i] - mean)*(pts[j].position[i] - mean);
		variance /= N;
		STD = sqrt(variance);

		// Normalization
		for (int j = 0; j < N; j++)
		{
			pts[j].position[i] -= mean;
			pts[j].position[i] /= STD;
		}
	}
	cout << "Input data normailized!" << endl;
}

double get_distance(Point *pt1, Point *pt2)
{
	double SSE = 0;
	for (int i = 0; i < D; i++)
		SSE += (pt1->position[i] - pt2->position[i])*(pt1->position[i] - pt2->position[i]);
	return sqrt(SSE);
}

double get_prediction()
{
	double numerator = 0, denominator = 0;
	for (int i = 0; i < K; i++)
	{
		double wt = exp(-alpha*PQ.top().first);
		numerator += wt*PQ.top().second->label;
		denominator += wt;
		PQ.pop();
	}
	return numerator / denominator;
}

/* Classify points based on vector */
void classify_points(void)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == j)
				continue;
			double dist = get_distance(&pts[i], &pts[j]);
			if (PQ.size()<K)
				PQ.push(dist_point(dist, &pts[j]));
			else if (PQ.top().first>dist)
			{
				PQ.pop();
				PQ.push(dist_point(dist, &pts[j]));
			}
		}

		pts[i].pdcn = get_prediction();
		//cout << i << ":" << pts[i].label << "," << pts[i].pdcn << endl;
	}
	cout << "Classification done!" << endl;
}

Node *insert(Node *T, Point *p, int d)
{
	if (T == NULL)
		return new Node(p);
	if (p->position[d] < T->key->position[d])
		T->left = insert(T->left, p, (d + 1) % D);
	else
		T->right = insert(T->right, p, (d + 1) % D);
	return T;
}

void create_kd_tree(void)
{
	for (int i = 0; i < N; i++)
		V.push_back(&pts[i]);
	random_shuffle(V.begin(), V.end());
	for (int i = 0; i < N; i++)
		KDT = insert(KDT, V[i], 0);
	cout << "kd-Tree created!" << endl;
}

bool is_near_to_boundary(Node *T, Point *p, int d)
{
	if (PQ.size() < K)
		return true;
	if (abs(p->position[d] - T->key->position[d]) < PQ.top().first)
		return true;
	else
		return false;
}

/* Search K nearest points based on kd-tree, prune if the K nearest points are near to the dividing line/boundary */
void search_neighbors(Node *T, Point *p, int d)
{
	if (T == NULL)
		return;
	if (T->key != p)
	{
		double dist = get_distance(p, T->key);
		if (PQ.size()<K)
			PQ.push(dist_point(dist, T->key));
		else if (PQ.top().first>dist)
		{
			PQ.pop();
			PQ.push(dist_point(dist, T->key));
		}
	}
	if (p->position[d] == T->key->position[d])
	{
		search_neighbors(T->left, p, (d + 1) % D);
		search_neighbors(T->right, p, (d + 1) % D);
	}
	else if (p->position[d] < T->key->position[d])
	{
		search_neighbors(T->left, p, (d + 1) % D);
		if (is_near_to_boundary(T, p, d))
			search_neighbors(T->right, p, (d + 1) % D);
	}
	else
	{
		search_neighbors(T->right, p, (d + 1) % D);
		if (is_near_to_boundary(T, p, d))
			search_neighbors(T->left, p, (d + 1) % D);
	}
	return;
}

/* Classify points based on kd-tree */
void classify_points_kd()
{
	for (int i = 0; i < N; i++)
	{
		search_neighbors(KDT, &pts[i], 0);
		pts[i].pdcn = get_prediction();
		//cout << i << ":" << pts[i].label << "," << pts[i].pdcn << endl;
	}
	cout << "Classification done!" << endl;
}

void output_ASE()
{
	double sum = 0;
	for (int i = 0; i < N; i++)
		sum += (pts[i].label - pts[i].pdcn)*(pts[i].label - pts[i].pdcn);
	cout << "The average squared error is " << sum / N << endl;
}

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
	if (argc != 3)
	{
		cerr << "Usage: main [filename] [k value]" << endl;
		return 0;
	}
	K = atoi(argv[2]);
	cout << "Alpha = " << alpha << ", k = " << K << endl;
	read_input(argv[1]);
	normalize_input();
	//classify_points();
	create_kd_tree();
	classify_points_kd();
	output_ASE();
	return 0;
}
