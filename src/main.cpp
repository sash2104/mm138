#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

vector<vector<int>> dice_ = {
{0,1,2,3,4,5},
{3,0,2,5,4,1},
{5,3,2,1,4,0},
{1,5,2,0,4,3},
{1,2,0,4,5,3},
{4,1,0,3,5,2},
{3,4,0,2,5,1},
{2,3,0,1,5,4},
{2,0,1,5,3,4},
{5,2,1,4,3,0},
{4,5,1,0,3,2},
{0,4,1,2,3,5},
{0,3,4,1,2,5},
{1,0,4,5,2,3},
{5,1,4,3,2,0},
{3,5,4,0,2,1},
{3,2,5,4,0,1},
{4,3,5,1,0,2},
{1,4,5,2,0,3},
{2,1,5,3,0,4},
{2,5,3,0,1,4},
{0,2,3,4,1,5},
{4,0,3,5,1,2},
{5,4,3,2,1,0},
};
vector<vector<int>> trans_ = {
{1,3,21,11},
{2,0,16,6},
{3,1,9,23},
{0,2,4,18},
{5,7,13,3},
{6,4,22,10},
{7,5,1,15},
{4,6,8,20},
{9,11,19,7},
{10,8,14,2},
{11,9,5,17},
{8,10,0,12},
{13,15,11,21},
{14,12,18,4},
{15,13,23,9},
{12,14,6,16},
{17,19,15,1},
{18,16,10,22},
{19,17,3,13},
{16,18,20,8},
{21,23,7,19},
{22,20,12,0},
{23,21,17,5},
{20,22,2,14},
};

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