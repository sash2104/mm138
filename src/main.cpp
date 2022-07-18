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

vector<vector<int>> dice_ = {
{0,1,2,3,4,5},
{2,3,1,0,4,5},
{1,0,3,2,4,5},
{3,2,0,1,4,5},
{5,4,0,1,3,2},
{0,1,4,5,3,2},
{4,5,1,0,3,2},
{1,0,5,4,3,2},
{2,3,5,4,1,0},
{5,4,3,2,1,0},
{3,2,4,5,1,0},
{4,5,2,3,1,0},
{1,0,2,3,5,4},
{2,3,0,1,5,4},
{0,1,3,2,5,4},
{3,2,1,0,5,4},
{5,4,1,0,2,3},
{1,0,4,5,2,3},
{4,5,0,1,2,3},
{0,1,5,4,2,3},
{3,2,5,4,0,1},
{5,4,2,3,0,1},
{2,3,4,5,0,1},
{4,5,3,2,0,1},
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


// up, down, left, right
static constexpr short UP = 0;
static constexpr short DOWN = 1;
static constexpr short LEFT = 2;
static constexpr short RIGHT = 3;
int DX[4] = {0, 0, -1, 1};
int DY[4] = {-1, 1, 0, 0};
vector<vector<int>> orders_ = {
  {0,1,2,3},
  {0,1,3,2},
  {0,2,1,3},
  {0,2,3,1},
  {0,3,1,2},
  {0,3,2,1},
  {1,2,3,0},
  {1,3,2,0},
  {2,1,3,0},
  {2,3,1,0},
  {3,1,2,0},
  {3,2,1,0},
  {2,3,0,1},
  {3,2,0,1},
  {1,3,0,2},
  {3,1,0,2},
  {1,2,0,3},
  {2,1,0,3},
  {3,0,1,2},
  {2,0,1,3},
  {3,0,2,1},
  {1,0,2,3},
  {2,0,3,1},
  {1,0,3,2},
};


// TOP, BOTTOM
static constexpr short TOP = 0;
static constexpr short BOTTOM = 1;

int to_[30*30*24][4];
int dist_[30*30][30*30];
int dir_[30*30][30*30];
int y_(int id3) { return id3/(24*N); }
int x_(int id3) { return (id3/24)%N; }
int d_(int id3) { return id3%24; }

struct Pos {
  short x = -1, y = -1, d = -1;
  Pos() {}
  Pos(int i): x(x_(i)), y(y_(i)), d(d_(i)) {}
  Pos(short x, short y): x(x), y(y) {}
  Pos(short x, short y, short d): x(x), y(y), d(d) {}
  bool eq(const Pos &other) const {
    return (other.x == x) && (other.y == y);
  }
  bool operator == (const Pos &other) const { 
    return (other.x == x) && (other.y == y) && (other.d == d);
  }
  bool operator != (const Pos &other) const { 
    return !((*this) == other);
  }
  short distance(const Pos &other) const {
    return abs(x-other.x)+abs(y-other.y);
  }
  Pos to(short dir) const {
    // assert(d != -1);
    return Pos(x+DX[dir],y+DY[dir], trans_[d][dir]);
  }

  int id3() const { return y*N*24+x*24+d; }
  int id2() const { return y*N+x; }

  friend std::ostream& operator<<(std::ostream& os, const Pos &p) {
    os << "(" << p.x << "," << p.y << "," << p.d << ")";
    return os;
  }
};

int getDir(const Pos &cur, const Pos &nex) {
  // cur -> nexの方向を返す
  if (cur.x > nex.x) return LEFT;
  if (cur.x < nex.x) return RIGHT;
  if (cur.y > nex.y) return UP;
  if (cur.y < nex.y) return DOWN;
  return -1;
}

bool outside(const Pos &p) {
  if (p.x < 0 || p.x >= N) return true;
  if (p.y < 0 || p.y >= N) return true;
  return false;
}
// update(), calcScore(), revert(), write()を実装する
// using grid_t = vector<vector<Pos>>;
using grid_t = vector<int>;

static constexpr int MAX_DEPTH = 15;
vector<char> dir2c = {'A', 'v', '<', '>'};
void show(const grid_t &grid) {
  REP(y,N) {
    REP(x,N) {
      int cur = Pos(x,y).id2();
      int nex = grid[cur];
      if (nex == -1) {
        cerr << '.';
        continue;
      }
      int dir = dir_[cur][nex/24];
      cerr << dir2c[dir];
    }
    cerr << '\n';
  }
  cerr << '\n';
}

struct State {
  vector<int> dice;
  // Pos start, goal;
  int start, goal;
  grid_t grid;
  int score = -INF;
  // backup
  vector<int> bpos;
  int bscore;
  int blen;
  int bsrc, btarget;
  int bid;
  int btype;
  int bdid, bv;
  State(): dice(6), grid(N*N,-1), bpos(MAX_DEPTH+1) {}
  int update2() {
    // diceの数字変更
    btype = 2;
    int did = rng.nextInt(6);
    int vmin = max(1, V-3);
    int v = rng.nextInt(vmin, V);
    while (v == dice[did]) {
      v = rng.nextInt(vmin, V);
    }
    bscore = score;
    bdid = did;
    bv = dice[did];
    dice[did] = v;
    score = calcScore();
    // return (score-bscore)/(N*N*0.8);
    return score-bscore;
  }

  int clear(int src, int *len, int *target) {
    // clearすることにより減るスコアを返す
    int ret = 0;
    int cur = src;
    REP(i,*len) {
      int nex = grid[cur/24];
      assert(nex != -1);
      // cerr << Pos(cur) << Pos(nex) << Pos(start) << Pos(goal) << endl;
      bpos[i] = nex;
      *target = nex;
      int d = dice[dice_[d_(cur)][BOTTOM]];
      int v = grid_[y_(cur)][x_(cur)];
      if (abs(v) == d) ret -= v;
      // cerr << "[cl]" << Pos(cur) << Pos(nex) << endl;
      grid[cur/24] = -1;
      // cerr << i << cur << nex << endl;
      if (nex/24 == goal/24) {
        *len = i+1;
        break;
      }
      cur = nex;
    }
    return ret;
  }

  double update1() { 
    btype = 1;
    int len = rng.nextInt(3,MAX_DEPTH-2);
    int id = rng.nextInt(N*N);
    while(grid[id] == -1 || id == goal/24 || grid[id]/24 == goal/24) {
      id = rng.nextInt(N*N);
    }
    bid = id;
    int cur = grid[id];
    bsrc = cur;
    int target;
    int diff1 = clear(cur, &len, &target);
    // cerr << Pos(x,y) << "->" << Pos(x_(cur), y_(cur)) << "->" << Pos(x_(target), y_(target)) << endl;
    blen = len;
    bscore = score;
    btarget = target;
    // show(grid);
    // cerr << bsrc << target << endl;
    int len2 = dfs(bsrc, bsrc, target, 0);
    if (len2 == -1) {
      return -INF;
    }
    int diff2 = calcDiffScore(bsrc, btarget);
    score += diff1+diff2;

    // show(grid);
    // score = calcScore();
    // cerr << score-bscore << " " << diff1+diff2 << " " << diff1 << " " << diff2 << endl;
    // assert(score-bscore == diff1+diff2);

    assert(len > 0 && len2 > 0);
    // double sc = 1.0*diff1/len+1.0*diff2/len2;
    double sc = diff1+diff2+(len-len2);
    // if (sc > 0) {
    //   D5(diff1, len, diff2, len2, sc);
    // }
    return sc;
  }
  double update() { 
    if (V >= 7) return update1();
    int p = rng.nextInt(N*N);
    if (p == 0) return update2();
    return update1();
  }

  bool empty(int p) const {
    return grid[p/24] == -1;
  }

  int dfs(int cur, int src, int target, int depth) {
    if (!empty(cur)) {
      if (cur == target) {
        // Pos fr = src;
        // while(true) {
        //   Pos nex = grid[fr.y][fr.x];
        //   assert(nex.x != -1);
        //   cerr << nex;
        //   if (nex == target) break;
        //   fr = nex;
        // }
        // cerr << endl;
        return depth;
      }
      return -1;
    }
    // targetにたどり着けない場合は枝刈り
    if (dist_[cur/24][target/24] > MAX_DEPTH-depth) return -1;

    int oid = rng.nextInt(24);
    for (int dir: orders_[oid]) {
      int nex = to_[cur][dir];
      if (nex == -1) continue; // outside
      assert(grid[cur/24] == -1);
      grid[cur/24] = nex;
      int ret = dfs(nex, src, target, depth+1);
      if (ret != -1) return ret;
      grid[cur/24] = -1;
    }
    return -1;
  }

  int size() {
    // 輪の長さ
    int len = 0;
    int cur = start;
    while (true) {
      ++len;
      int nex = grid[cur/24];
      assert(nex != -1);
      if (nex/24 == start/24) break;
      cur = nex;
    }
    return len; 
  }

  int calcDiffScore(int src, int target) {
    int ret = 0;
    int cur = src;
    while (true) {
      int d = dice[dice_[d_(cur)][BOTTOM]];
      int v = grid_[y_(cur)][x_(cur)];
      // REP(i,6) { cerr << dice[dice_[cur.d][i]]; }
      if (abs(v) == d) ret += v;
      int nex = grid[cur/24];
      assert(nex != -1);
      // cerr << "[ad]" <<  cur << nex << d << v << endl;
      if (nex/24 == target/24) break;
      cur = nex;
    }
    return ret;
  }

  int calcScore() { 
    // DEBUG("ok");
    int ret = 0;
    int cur = start;
    // show(grid);
    while (true) {
      int d = dice[dice_[d_(cur)][BOTTOM]];
      int v = grid_[y_(cur)][x_(cur)];
      // cerr << cur;
      // REP(i,6) { cerr << dice[dice_[cur.d][i]]; }
      // cerr << " " << d << v << endl;
      if (abs(v) == d) ret += v;
      int nex = grid[cur/24];
      assert(nex != -1);
      if (nex/24 == start/24) break;
      cur = nex;
    }
    // DEBUG("ok");
    return ret;
  }

  void revert() {
    if (btype == 1) revert1();
    else if (btype == 2) revert2();
  }

  void revert2() {
    score = bscore;
    dice[bdid] = bv;
  }

  void revert1() {
    // show(grid);
    int cur = bsrc;
    while (true) {
      Pos p(cur);
      int nex = grid[cur/24];
      grid[cur/24] = -1;
      if (nex == -1) break;
      if (nex == btarget) break;
      cur = nex;
    }
    cur = bsrc;
    REP(i,blen) {
      // cerr << i << cur << bpos[i] << endl;
      grid[cur/24] = bpos[i];
      cur = bpos[i];
    }
    score = bscore;
    // show(grid);
  } // update()適用前の状態に戻す.

  void write() {
    REP(i,6) cout << dice[i] << '\n';

    int n = size();
    cout << size() << '\n';
    int cur = start;
    REP(i,n) {
      cout << y_(cur) << " " << x_(cur) << '\n';
      int nex = grid[cur/24];
      // if (i == n-1) assert(nex.eq(start));
      // else assert(!nex.eq(start));
      cur = nex;
    }
  } // 現在の状態を出力する.
};

void initState(State &s) {
  int v = V;
  s.dice[0] = V;
  s.dice[1] = V-1;
  s.dice[2] = V;
  s.dice[3] = V-2;
  s.dice[4] = V-1;
  if (V > 4) s.dice[5] = V-3;
  else s.dice[5] = V-2;
  // REP(i,6) {
  //   // s.dice[i] = v;
  //   // --v;
  //   // if (v <= 0) v = V;
  //   s.dice[i] = V-i%3;
  // }
  s.start = Pos(0,0,0).id3();
  int cur = s.start;
  for (int dir: {RIGHT, DOWN, LEFT, UP}) {
    REP(i,N) {
      int nex = to_[cur][dir];
      Pos p(x_(cur),y_(cur));
      Pos np(x_(nex),y_(nex));
      if (nex == -1) break; // outside
      s.grid[cur/24] = nex;
      s.goal = cur;
      if (s.grid[nex/24] != -1) break; // not empty
      cur = nex;
    }
  }
  s.score = s.calcScore();
}

// void initState2(State &s) {
//   int v = V;
//   REP(i,6) {
//     // s.dice[i] = v;
//     // --v;
//     // if (v <= 0) v = V;
//     s.dice[i] = V-i%3;
//   }
//   int cx = N/2;
//   int cy = N/2;
//   s.start = Pos(cx-2,cy-2,0);
//   Pos cur = s.start;
//   for (int dir: {RIGHT, DOWN, LEFT, UP}) {
//     REP(i,3) {
//       Pos nex = cur.to(dir);
//       if (outside(nex)) break;
//       s.grid[cur.y][cur.x] = nex;
//       s.goal = cur;
//       if (!s.empty(nex)) break;
//       cur = nex;
//     }
//   }
//   s.score = s.calcScore();
// }

// void initState3(State &s) {
//   int v = V;
//   s.dice[0] = V;
//   s.dice[1] = V-1;
//   s.dice[2] = V;
//   s.dice[3] = V-1;
//   s.dice[4] = V-2;
//   if (V > 4) s.dice[5] = V-3;
//   else s.dice[5] = V-2;
//   // REP(i,6) {
//   //   // s.dice[i] = v;
//   //   // --v;
//   //   // if (v <= 0) v = V;
//   //   s.dice[i] = V-i%3;
//   // }
//   s.start = Pos(N/2,0,0);
//   Pos cur = s.start;
//   for (int dir: {RIGHT, DOWN, LEFT, UP, RIGHT}) {
//     REP(i,N) {
//       Pos nex = cur.to(dir);
//       if (outside(nex)) break;
//       s.grid[cur.y][cur.x] = nex;
//       s.goal = cur;
//       if (!s.empty(nex)) break;
//       cur = nex;
//     }
//   }
//   s.score = s.calcScore();
// }

struct SASolver {
  double startTemp = 3;
  double endTemp = 0.001;
  Timer timer = Timer(2.85);
  // Timer timer = Timer(9.55);
  // Timer timer = Timer(29.55);
  State best;
  SASolver() { init(); }
  SASolver(double st, double et): startTemp(st), endTemp(et) { init(); }
  SASolver(double st, double et, double limit): startTemp(st), endTemp(et), timer(limit) { init(); }
  void init() {} // 初期化処理をここに書く

  void solve(State &state) {
    double t;
    best = state;
    int counter = 0;
    while ((t = timer.get()) < timer.LIMIT) // 焼きなまし終了時刻までループ
    {
      double T = startTemp + (endTemp - startTemp) * t / timer.LIMIT;
      for (int i = 0; i < 100; ++i) { // 時間計算を間引く
        double diff = state.update();
        if (diff <= -INF+0.1) {
          state.revert();
          continue;
        }

        // 最初t=0のときは、スコアが良くなろうが悪くなろうが、常に次状態を使用
        // 最後t=timer.LIMITのときは、スコアが改善したときのみ、次状態を使用
        // スコアが良くなった or 悪くなっても強制遷移
        double tr = T*rng.nextLog();
        // cerr << t << " " << T << " " << tr << " " << diff << endl;
        if (diff >= tr)
        {
          if (best.score < state.score) {
            best = state;
            cerr << "time = " << t << ", counter = " << counter << ", score = " << best.score << endl;
            // best.write();
          }
        }
        else { state.revert(); }
        ++counter;
      }
    }
    cerr << "counter = " << counter << ", score = " << best.score << " " << best.calcScore() << endl;
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
        // initState3(state);
        // show(state.grid);
        // return;

        // REP(i,100) {
        // int diff = state.update();
        // }
        // if (diff != -INF) state.revert();

        // Pos start = state.grid[0][1];
        // cerr << start << state.grid[0][4] << endl;
        // Pos bkup = state.grid[start.y][start.x];
        // state.grid[start.y][start.x] = Pos();
        // state.dfs(start, start, state.grid[0][4], 0);
        // state.grid[start.y][start.x] = bkup;

        // state.write();
        SASolver s;
        s.solve(state);
        s.best.write();
        // show(s.best.grid);
        int score = s.best.calcScore();
        cerr << "score=" << score << " " << score*B << endl;
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

void initPos() {
  REP(i,30*30*24) REP(dir,4) to_[i][dir] = -1;
  REP(i,30*30) REP(j,30*30) dist_[i][j] = INF;
  REP(i,30*30) REP(j,30*30) dir_[i][j] = -1;
  REP(y,N) REP(x,N) {
    Pos p(x,y,0);
    int id2 = p.id2();
    REP(d,24) {
      p.d = d;
      int id3 = p.id3();
      assert(y_(id3) == y);
      assert(x_(id3) == x);
      assert(d_(id3) == d);
      REP(dir,4) {
        Pos np = p.to(dir);
        if (outside(np)) continue;
        to_[id3][dir] = np.id3();
      }
    }
    REP(ny,N) REP(nx,N) {
      Pos np(nx,ny,0);
      int nid2 = np.id2();
      dist_[id2][nid2] = p.distance(np);
      dir_[id2][nid2] = getDir(p,np);
    }
  }
}

int main() 
{
  Solver solver;
  solver.readInput();
  initPos();
  solver.solve();
  solver.writeOutput();

}