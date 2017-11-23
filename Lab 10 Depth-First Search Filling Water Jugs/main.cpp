/*
  CPSC 2120
  Xi Zhao
  11/5/2014
*/

#include <iostream>

using namespace std;

#define N 1001
int A, B, X;
int a, b;

bool visited[N][N];
int did[N][N];

bool visit(int a, int b)
{
	if (a < 0 || a > N - 1 || b < 0 || b > N - 1)	return false;
	if (visited[a][b] == true) 	return false;
	
	visited[a][b] = true;
	if ((a + b) == X) return true;
	if (visit(A, b))	
	{
		did[a][b] = 1; return true; // Fill 1
	}
	if (visit(a, B))	
	{
		did[a][b] = 2; return true; // Fill 2
	}
	if(visit(0, b))	
	{
		did[a][b] = 3; return true; // Empty 1	
	}
	if (visit(a, 0))	
	{
		did[a][b] = 4; return true; // Empty 2
	}
	if (visit((a + b > B) ? a + b - B : 0, (a + b > B) ? B : a + b))	
	{
		did[a][b] = 5; return true; // Pour 1 in 2
	}
	if (visit((a + b > A) ? A : a + b, (a + b > B) ? a + b - A : 0))	
	{
		did[a][b] = 6; return true; // Pour 2 in 1
	}
	return false;
}

void print_steps(int a, int b)	
{
	int done = did[a][b];
	int a5, b5, a6, b6;
	switch (done)		
	{
		case 1:
			cout << "Fill jug 1   [a = " << A << ", b = " << b << "]" << endl;
			print_steps(A, b);
			break;
		case 2:
			cout << "Fill jug 2   [a = " << a << ", b = " << B << "]" << endl;
			print_steps(a, B);
			break;
		case 3:
			cout << "Empty jug 1  [a = " << 0 << ", b = " << b << "]" << endl;
			print_steps(0, b);
			break;
		case 4:
			cout << "Empty jug 2  [a = " << a << ", b = " << 0 << "]" << endl;
			print_steps(a, 0);
			break;
		case 5:
			a5 = (a + b > B) ? a + b - B : 0;
			b5 = (a + b > B) ? B : a + b;
			cout << "Pour 1 -> 2  [a = " << a5 << ", b = " << b5 << "]" << endl;
			print_steps((a + b > B) ? a + b - B : 0, (a + b > B) ? B : a + b);
			break;
		case 6:
			a6 = (a + b > A) ? A : a + b;
			b6 = (a + b > B) ? a + b - A : 0;
			cout << "Pour 2 -> 1  [a = " << a6 << ", b = " << b6 << "]" << endl;
			print_steps((a + b > A) ? A : a + b, (a + b > B) ? a + b - A : 0);
			break;
	}
}

int main(void)
{
	do {cout << "Enter A [0 - 1000]: "; cin >> A;} while (A < 0 || A > N -1);
	do {cout << "Enter B [0 - 1000]: "; cin >> B;} while (B < 0 || B > N -1);
	do {cout << "Enter X [0 - 1000]: "; cin >> X;} while (X < 0 || X > N -1);
	
	if (visit(0, 0))
		print_steps(0, 0);	
	else
		cout << "Impossible!" << endl;

}
