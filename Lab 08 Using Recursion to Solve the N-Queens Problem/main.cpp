/*
  CPSC 2120
  Xi Zhao
  10/22/2014
*/

#include <iostream>
#include <cstdlib>

using namespace std;

int *CB; // Chess board, the values in array represent the column of the queen
int n, half, count; // Size of chess board, half size, number of solutions
bool odd; // If the size is even

bool check_cell(int row, int col)
{
  if (row == 0)
  {
    CB[0] = col;
    return true;
  }    
  else
  {
    for (int i = 0; i < row; i++)
    {
      if(col == CB[i])
        return false;
        
      int dist_row = row - i;
      int dist_col = col - CB[i];
      if (dist_row == dist_col || dist_row == - dist_col)
        return false;
    }
    CB[row] = col;
    return true;
  }
}

void check_row(int row)
{
  if (row == n)
  {
    count++;
    return;
  }
  if (row == 0)
  {    
    for (int i = 0; i < half; i++)
    {
      CB[0] = i;
      check_row(1);
    }
        
    count *= 2;
    
    if (odd)
    {
      CB[row] = half;
      check_row(1);
    }
  }  
  else
  {
    for (int i = 0; i < n; i++)
    {
      if (check_cell(row, i))
        check_row(row + 1);
      else
        continue;
    }
  }
}

int main(int argc, char* argv[])
{

  if (argc != 2)
  {
    cerr << "usage: ./main number" << endl;
    exit(1);
  } 
  else
    n = atoi(argv[1]);
    
  CB = new int [n];
  
  half = n / 2;
  odd = n % 2;
  
  check_row(0);
  
  cout << "Number of solutions is " << count << endl;

  delete[] CB;
}
