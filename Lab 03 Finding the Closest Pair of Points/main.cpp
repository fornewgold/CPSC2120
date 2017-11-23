/*
  CPSC 2120
  Xi Zhao
  9/11/2014
*/

#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

struct Node
{
	double x, y;
	Node *next;
	Node()
	{
		x = 0;
		y = 0;
		next = NULL;
	}
	Node(double a, double b, Node* n)
	{
		x = a;
		y = b;
		next = n;
	}
};

double getDistance(Node* a, Node* b)
{
	return sqrt(pow(a->x - b->x, 2.0) + pow(a->y - b->y, 2.0));
}

int main(void)
{
	// Allocate and initialize 2D array of pointers to linked lists.
	cout << "Allocating space...";

	int width = 1000, height = 1000;
	Node ***grid = new Node **[height];
	for (int i = 0; i < height; i++)
	{
		grid[i] = new Node *[width];
		for (int j = 0; j < width; j++)
			grid[i][j] = NULL;
	}

	cout << "Done!" << endl;

	// Read input le, inserting each point into the appropriate linked list based on the cell to which it maps.
	fstream fin;
	fin.open("points.txt");
	if (!fin.is_open())
		cerr << "Cannot open input file\n" << endl;

	cout << "Reading points...";

	double x, y;
	int num = 0;
	while (fin >> x >> y) 
	{
		//hashing the points into the data structure
		int a = x * width;
		int b = y * height;
		grid[a][b] = new Node(x, y, grid[a][b]);
		num++;
	}

	cout << "Done!" << endl << num << " points read" << endl;
	fin.close();

	// For each point, compare it to all the points within its cell and the 8 adjacent cells; rememberthe smallest distance obtained during this process.
	cout << "Calculating minimum distance...";

	double min_dist = 1;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (grid[i][j] != NULL) // If the cell is not empty
			{
				for (Node *it_1 = grid[i][j]; it_1 != NULL; it_1 = it_1->next) // For each point in the cell
				{
					for (Node *it_2 = it_1->next; it_2 != NULL; it_2 = it_2->next) // Compare to all points in the same cell
					{
						double dist = getDistance(it_1, it_2);
						min_dist = (min_dist < dist) ? min_dist : dist;
					}

					for (int m = i - 1; m < i + 2; m++) // Compare to 8 points in adjacent cells
					{
						for (int n = j - 1; n < j + 2; n++)
						{
							if ((m == i) && (n == j)) // Skip if 2 iteratorscells are in same cell 
								continue;

							if ((m < 0) || (m > height - 1) || (n < 0) || (n > width - 1)) // Skip if the cell is out of the grid
								continue;

							if (grid[m][n] != NULL) // If the cell is not empty
							{
								for (Node *it_2 = grid[m][n]; it_2 != NULL; it_2 = it_2->next) // Compare to all points in the same cell
								{
									double dist = getDistance(it_1, it_2);
									min_dist = (min_dist < dist) ? min_dist : dist;
								}
							}
							else
								continue;
						}
					}
				}
			}
			else
				continue;
		}
	}

	cout << "Done!" << endl;

	// Print out minimum distance.
	cout << "The minimum distance is " << min_dist << endl;

	// De-allocate 2D array and linked lists.
	cout << "De-allocating space...";

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			while (grid[i][j] != NULL)
			{
				Node *temp = grid[i][j];
				grid[i][j] = grid[i][j]->next;
				delete temp;
			}
			delete[] grid[i][j];
		}
		delete[] grid[i];
	}
	delete[] grid;

	cout << "Done!" << endl;

	return 0;
}
