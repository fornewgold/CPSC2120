/*
  CPSC 2120
  Xi Zhao
  10/3/2014
*/

#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cout << "Usage: bad1 <input size>\n";
    return 0;
  }

  int N = atoi(argv[1]);  // get first command-line argument
  if (N<1 || N>100000) {
    cout << "Invalid input size!\n";
    return 0;
  }
  
  // Generate and print bad input of size N for prog1
  // (currently just generates an input of N random numbers)
  cout << N << "\n";
  for (int i=0; i<N; i++) {
    if (i == 0) cout << N << "\n";
    else if (i == N - 1) cout << N << "\n";
    else cout << i << "\n";
  }
    
  return 0;
}
