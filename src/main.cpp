#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

int main() 
{
  int N, V;
  double B;
  cin >> N >> V >> B;

  int grid[N][N];
  for (int r=0; r<N; r++)
    for (int c=0; c<N; c++)
      cin >> grid[r][c];

  //print random face values
  for (int i=0; i<6; i++) cout << (i%V)+1 << endl;

  //move in a spiral
  int dr[] = {0,1,0,-1};
  int dc[] = {1,0,-1,0};  
  int dir = 0;
  int r = 0;
  int c = 0;    

  bool seen[N][N];
  for (int i=0; i<N; i++) for (int k=0; k<N; k++) seen[i][k]=false;

  cout << N*N << endl;
  for (int i=0; i<N*N; i++)
  {
    cout << std::to_string(r)+" "+std::to_string(c) << endl;
    seen[r][c]=true;

    int r2=r+dr[dir];
    int c2=c+dc[dir];
    if (r2<0 || r2>=N || c2<0 || c2>=N || seen[r2][c2])
    {
      dir=(dir+1)%4;
      r2=r+dr[dir];
      c2=c+dc[dir];        
    }
    r=r2;
    c=c2;
  } 
  cout.flush();  
}