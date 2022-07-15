#include <bits/stdc++.h>
#define DEBUG(x) do { std::cerr << __LINE__ << " " << __func__ <<  " " << x << std::endl; } while (0)
#define D1(x) std::cerr <<  "! " << x << std::endl;
#define D2(x,y) std::cerr <<  "! " << x << " " << y << std::endl;
#define D3(x,y,z) std::cerr <<  "! " << x << " " << y << " " << z << std::endl;
#define D4(x,y,z,a) std::cerr <<  "! " << x << " " << y << " " << z << " " << a << std::endl;
#define D5(x,y,z,a,b) std::cerr <<  "! " << x << " " << y << " " << z << " " << a << " " << b << std::endl;
#define D11(x,y,z,a,b,c,d,e,f,g,h) std::cerr <<  "! " << x << " " << y << " " << z << " " << a << " " << b << " " << c << " " << d << " " << e << " " << f << " " << g << " " << h << std::endl;

#define REP(i, n) for (int i = 0; (i) < (int)(n); ++ (i))
#define REP3(i, m, n) for (int i = (m); (i) < (int)(n); ++ (i))
#define REPR(i, n) for (int i = (int)(n) - 1; (i) >= 0; -- (i))
#define REP3R(i, m, n) for (int i = (int)(n) - 1; (i) >= (int)(m); -- (i))
#define ALL(x) std::begin(x), std::end(x)
using namespace std;

//*********************************  CONSTANTS  **********************************************
static constexpr int INF = 1<<30;

// #define NDEBUG

static std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (getline(ss, item, delim)) {
    if (!item.empty()) {
      elems.push_back(item);
    }
  }
  return elems;
}

struct XorShift {
  unsigned int x, y, z, w;
  double nL[65536];
  XorShift() { init(); }
  void init()
  {
    x = 314159265; y = 358979323; z = 846264338; w = 327950288;
    double n = 1 / (double)(2 * 65536);
    for (int i = 0; i < 65536; i++) {
      nL[i] = log(((double)i / 65536) + n);
    }
  }
  inline unsigned int next() { unsigned int t=x^x<<11; x=y; y=z; z=w; return w=w^w>>19^t^t>>8; }
  inline double nextLog() { return nL[next()&0xFFFF]; }
  inline int nextInt(int m) { return (int)(next()%m); } // [0, m)
  int nextInt(int min, int max) { return min+nextInt(max-min+1); } // [min, max]
  inline double nextDouble() { return (double)next()/((long long)1<<32); } // [0, 1]
};
XorShift rng;

template <typename T>
inline void rough_shuffle(vector<T>& lv) {
    int n = lv.size();
    for (int i = n; i > 0; --i) {
        int id = rng.nextInt(i);
        swap(lv[id], lv[i-1]);
    }
}

std::size_t calc_hash(std::vector<int> const& vec) {
  std::size_t seed = vec.size();
  for(auto& i : vec) {
    seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

struct Timer {
  const double LIMIT; // FIXME: 時間制限(s)
  Timer() : LIMIT(0.95) { reset(); }
  Timer(double limit) : LIMIT(limit) { reset(); }
	chrono::system_clock::time_point start;
	void reset() {
		start = chrono::system_clock::now();
	}
 
	double get() {
		auto end = chrono::system_clock::now();
		return chrono::duration_cast<chrono::milliseconds>(end - start).count()/1000.0;
	}
};

// ------------- Constants ------------
int N, V;
double B;
int grid_[30][30];

// up, down, left, right
static constexpr short UP = 0;
static constexpr short DOWN = 1;
static constexpr short LEFT = 2;
static constexpr short RIGHT = 3;
int DX[4] = {0, 0, -1, 1};
int DY[4] = {-1, 1, 0, 0};

struct Pos {
  int x = -1, y = -1;
  Pos() {}
  Pos(int x, int y): x(x), y(y) {}
  bool operator == (const Pos &other) const { 
    return (other.x == x) && (other.y == y);
  }
  bool operator != (const Pos &other) const { 
    return !((*this) == other);
  }
  int distance(const Pos &other) const {
    return abs(x-other.x)+abs(y-other.y);
  }
  Pos to(int dir) const {
    return Pos(x+DX[dir],y+DY[dir]);
  }

  friend std::ostream& operator<<(std::ostream& os, const Pos &p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
  }
};

bool outside(const Pos &p) {
  if (p.x < 0 || p.x >= N) return true;
  if (p.y < 0 || p.y >= N) return true;
  return false;
}

vector<vector<int>> dice_ = {
{0,1,2,3,4,5},
{3,0,2,5,4,1},
{5,3,2,1,4,0},
{1,5,2,0,4,3},
{4,5,1,0,3,2},
{0,4,1,2,3,5},
{2,0,1,5,3,4},
{5,2,1,4,3,0},
{3,2,5,4,0,1},
{4,3,5,1,0,2},
{1,4,5,2,0,3},
{2,1,5,3,0,4},
{5,1,4,3,2,0},
{3,5,4,0,2,1},
{0,3,4,1,2,5},
{1,0,4,5,2,3},
{4,0,3,5,1,2},
{5,4,3,2,1,0},
{2,5,3,0,1,4},
{0,2,3,4,1,5},
{1,2,0,4,5,3},
{4,1,0,3,5,2},
{3,4,0,2,5,1},
{2,3,0,1,5,4},
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

// update(), calcScore(), revert(), write()を実装する
using grid_t = vector<vector<Pos>>;

struct State {
  vector<int> dice;
  Pos start;
  grid_t grid;
  int score = -INF;
  int bscore;
  State(): dice(6), grid(N,vector<Pos>(N)) {}
  int update() { 
    return 0;
  }

  int size() {
    // 輪の長さ
    int len = 0;
    Pos cur = start;
    while (true) {
      ++len;
      Pos nex = grid[cur.y][cur.x];
      if (nex == start) break;
      cur = nex;
    }
    return len; 
  }

  int calcScore() { 
    return 0;
  }
  void revert() {
  } // update()適用前の状態に戻す.

  void write() {
    REP(i,6) cout << dice[i] << endl;

    int n = size();
    cout << size() << endl;
    Pos cur = start;
    REP(i,n) {
      cout << cur.y << " " << cur.x << endl;
      Pos nex = grid[cur.y][cur.x];
      if (i == n-1) assert(nex == start);
      else assert(nex != start);
      cur = nex;
    }
  } // 現在の状態を出力する.
};

void initState(State &s) {
  int v = V;
  REP(i,6) {
    s.dice[i] = v;
    --v;
    if (v <= 0) v = V;
  }
  s.start = Pos(0,0);
  Pos cur = s.start;
  for (int dir: {RIGHT, DOWN, LEFT, UP}) {
    REP(i,N) {
      Pos nex = cur.to(dir);
      if (outside(nex)) break;
      s.grid[cur.y][cur.x] = nex;
      if (s.grid[nex.y][nex.x].x != -1) break;
      cur = nex;
    }
  }
}

struct SASolver {
  double startTemp = 3;
  double endTemp = 0.001;
  Timer timer = Timer(2.85);
  // Timer timer = Timer(29.85);
  State best;
  SASolver() { init(); }
  SASolver(double st, double et): startTemp(st), endTemp(et) { init(); }
  SASolver(double st, double et, double limit): startTemp(st), endTemp(et), timer(limit) { init(); }
  void init() {} // 初期化処理をここに書く

  void solve(State &state) {
    double t;
    int score = state.calcScore();
    best = state;
    int bestScore = score;
    int counter = 0;
    while ((t = timer.get()) < timer.LIMIT) // 焼きなまし終了時刻までループ
    {
      double T = startTemp + (endTemp - startTemp) * t / timer.LIMIT;
      for (int i = 0; i < 100; ++i) { // 時間計算を間引く
        int diff = state.update();

        // 最初t=0のときは、スコアが良くなろうが悪くなろうが、常に次状態を使用
        // 最後t=timer.LIMITのときは、スコアが改善したときのみ、次状態を使用
        // スコアが良くなった or 悪くなっても強制遷移
        double tr = T*rng.nextLog();
        // cerr << t << " " << T << " " << tr << endl;
        if (diff >= tr)
        {
          score += diff;
          if (bestScore < score) {
            bestScore = score;
            best = state;
            // cerr << "time = " << t << ", counter = " << counter << ", score = " << bestScore << endl;
            // cerr << "time = " << t << ", counter = " << counter << ", score = " << bestScore << " " << best.calcScore(true) << endl;
            // best.write();
          }
        }
        else { state.revert(); }
        ++counter;
      }
    }
    cerr << "counter = " << counter << ", score = " << bestScore << " " << best.calcScore() << endl;
  }
};

struct Solver {
    int turn_ = 0;
    Solver() {
        reset();
    }
    void reset() {
    }

    void solve() {
        State state; // 開始状態
        initState(state);
        state.write();
        // SASolver s;
        // s.solve(state);
        // s.best.write();
    }
    void show() { 
        cerr << "[turn]" << turn_ << endl;
    }

    void readInput() { // loopから抜け出す時にtrue
      cin >> N >> V >> B;

      REP(r,N) REP(c,N) {
        cin >> grid_[r][c];
      }
    }

    void writeOutput() {
      return;
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
};

int main() 
{
  Solver solver;
  solver.readInput();
  solver.solve();
  solver.writeOutput();

}