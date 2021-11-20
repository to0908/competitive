#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0};
long double eps = 1e-9;
long double pi = acos(-1);
template< typename T >istream &operator>>(istream &is, vector< T > &v) {
    for(T &in : v) is >> in;
    return is;
}
template< typename T >ostream &operator<<(ostream &os, const vector< T > &v) {
    for(int i = 0; i < (int) v.size(); i++) {
        os << v[i] << (i + 1 != (int) v.size() ? " " : "");
    }
    return os;
}
unsigned int randint() {
    static unsigned int tx = 123456789, ty=362436069, tz=521288629, tw=88675123;
    unsigned int tt = (tx^(tx<<11));
    tx = ty; ty = tz; tz = tw;
    return ( tw=(tw^(tw>>19))^(tt^(tt>>8)) );
}
#include<chrono>
struct Timer {
    chrono::high_resolution_clock::time_point st;

    Timer() { reset(); }

    void reset() {
        st = chrono::high_resolution_clock::now();
    }

    chrono::milliseconds::rep elapsed() {
        auto ed = chrono::high_resolution_clock::now();
        return chrono::duration_cast< chrono::milliseconds >(ed - st).count();
    }
};

const int n = 1000, m = 50;

const int h = 801, w = 801;
vector<vector<int>> a(n), b(n);
vector<int> atcoder = {400, 400};
vector<array<int,2>> pos(n);
vector<int> rev;
vector<int> ans1;
vector<array<int,3>> ans2;
const int INF = 1e9;

void addAtCoder() {
    ans2.push_back({atcoder[0], atcoder[1], INF});
}

void answer() {
    cout << m << " " << ans1 << endl;
    cout << (int)ans2.size() << " ";
    for(int i=0;i<(int)ans2.size();i++){
        cout << ans2[i][0] << " " << ans2[i][1] << " ";
    }
    cout << endl;
}


// connect a -> b and c -> d
__inline void swap_edges(vector<int>& seq, int a, int b, int c, int d) {
	int x = d, y = b, N = (int)seq.size();
	// if (y - x > a + N - c) {
	// 	x = c, y = a + N;
	// }
	while (x < y) {
		// swap(seq[x < N ? x : x - N], seq[y < N ? y : y - N]);
		// ++x, --y;
        int cntx = seq[x];
        int cnty = seq[y];
        int idx = rev[cntx];
        int idy = rev[cnty];
        int px = pos[abs(idx)-1][idx<0];
        int py = pos[abs(idy)-1][idy<0];
        pos[abs(idx)-1][idx<0] = py;
        pos[abs(idy)-1][idy<0] = px;
        assert(idx != INF);
        swap(seq[x++], seq[y--]);
	}
}
 
bool can_swap(vector<int>& seq, int a, int b, int c, int d) {
	int x = d, y = b, N = (int)seq.size();
    if(y - x + 1 >= m) return false;
	while (x < y) {
        int cnt = seq[x];
        int idx = rev[cnt];
        assert(idx != INF);
        int p1 = pos[abs(idx)-1][0];
        int p2 = pos[abs(idx)-1][1];
        if(d <= p1 && p1 <= b && d <= p2 && p2 <= b) return false;
        x++;
        // swap(seq[x++], seq[y--]);
	}
    return true;
}
 
int dist(vector<vector<int>> &points, vector<int> &edges, int xx, int yy){
    int x = edges[xx];
    int y = edges[yy];
    return abs(points[x][0] - points[y][0]) + abs(points[x][1] - points[y][1]);
}
 
void two_opt1(vector<vector<int>> &points, vector<int> &edges, Timer &time){
    int n = (int)edges.size();
    int iter = 5000;
    // while(time.elapsed() < 1900){
    while(iter--){
        for(int t=0;t<100;t++){
            int x = randint() % n, y = randint() % n;
            while (x == y) y = randint() % n;
            if (x > y) swap(x, y);
            int d = (x + 1) % n, c = (y + 1) % n;
            int dist1 = dist(points, edges, x, y);
            int dist2 = dist(points, edges, c, d);
            int dist3 = dist(points, edges, x, d);
            int dist4 = dist(points, edges, y, c);
            if (dist1 + dist2 < dist3 + dist4 && can_swap(edges, x, y, c, d)) {
                swap_edges(edges, x, y, c, d);
            }
        }
    }
}

void two_opt2(vector<vector<int>> &points, vector<int> &edges, Timer &time){
    int n = (int)edges.size();
    int iter = 5000;
    // while(time.elapsed() < 1900){
    while(iter--){
        for(int t=0;t<100;t++){
            int x = randint() % n, y = randint() % n;
            while (x == y) y = randint() % n;
            if (x > y) swap(x, y);
            int d = (x + 1) % n, c = (y + 1) % n;
            int dist1 = dist(points, edges, x, y);
            int dist2 = dist(points, edges, c, d);
            int dist3 = dist(points, edges, x, d);
            int dist4 = dist(points, edges, y, c);
            if (dist1 + dist2 < dist3 + dist4 && can_swap(edges, x, y, c, d)) {
                swap_edges(edges, x, y, c, d);
            }
        }
    }
}

int score(vector<array<int,3>> &seq){
    int dist = 0;
    int n = (int)seq.size();
    for(int i=0;i<(int)seq.size()-1;i++){
        for(int j=0;j<2;j++){
            dist += abs(seq[i][j] - seq[i+1][j]);
        }
    } 
    return dist;
}

int solve(Timer &time, vector<int> &mad){
    ans1.clear();
    ans2.clear();

    addAtCoder();

    rev.clear();

    vector<int> edges; // edges[i] := 
    int cnt = 1;
    edges.push_back(0);
    vector<vector<int>> points;
    points.push_back(atcoder);
    rev.push_back(INF);

    for(int i=0;i<m;i++){
        int x = mad[i];
        ans1.push_back(x+1);
        pos[x][0] = cnt;
        points.push_back(a[x]);
        edges.push_back(cnt);
        rev.push_back(x + 1);
        cnt++;
    }
    // cnt -> a or b
    two_opt1(points, edges, time);
    int N = edges.size();
    for(int j=0;j<N-1;j++){
        vector<int> pts = points[edges[(j+1) % N]];
        ans2.push_back({pts[0], pts[1], rev[edges[(j+1) % N]]});
    }

    for(int i=0;i<m;i++){
        int x = mad[m-i-1];
        bool ok = false;
        int ma = 0;
        for(int j=0;j<2;j++) {
            ma += abs(atcoder[j] - b[x][j]);
            ma += abs(ans2.back()[j] - b[x][j]);
        }
        int idx = (int)ans2.size();
        for(int j=0;j<(int)ans2.size();j++) {
            if(ans2[j][0] == a[x][0] && ans2[j][1] == a[x][1]){
                ok = 1;
                continue;
            }
            if(ok){
                int dist = 0;
                for(int k=0;k<2;k++) {
                    dist += abs(b[x][k] - ans2[j][k]);
                    dist += abs(b[x][k] - ans2[j-1][k]);
                }
                if(chmin(ma, dist)) idx = j;
            }
            // ans2.insert(ans2.begin() + j)で、ans2[j]の直前にinsertされる
        }
        ans2.insert(ans2.begin() + idx, {b[x][0], b[x][1], -x-1});
    }

    cnt = 0;
    rev.clear();
    edges.clear();
    points.clear();
    rev.push_back(INF);
    for(auto pts:ans2) {
        points.push_back({pts[0], pts[1]});
        edges.push_back(cnt);
        if(cnt){
            int x = pts[2];
            pos[abs(x) - 1][x<0] = cnt;
            rev.push_back(x);
        }
        cnt++;
    }

    two_opt2(points, edges, time);
    N = edges.size();
    for(int j=0;j<N-1;j++){
        auto pts = points[edges[(j+1) % N]];
        ans2[j+1] = {pts[0], pts[1], -1};
    }
    addAtCoder();
    return score(ans2);
}

void yaki(Timer &time){
    vector<array<int,3>> r;
    int mat[n][n];
    for(int i=0;i<n;i++){
        mat[i][i] = 0;
        for(int j=i+1;j<n;j++){
            int x = 0, y = 0, z = 0;
            for(int k=0;k<2;k++){
                z += abs(a[i][k] - atcoder[k]) + abs(a[j][k] - atcoder[k]);
                z += abs(b[i][k] - atcoder[k]) + abs(b[j][k] - atcoder[k]);
                x += abs(a[i][k] - a[j][k]) + abs(b[i][k] - b[j][k]);
                y += abs(a[i][k] - b[i][k]) + abs(a[j][k] - b[j][k]);
            }
            y /= 2;
            z /= 2;
            mat[i][j] = mat[j][i] = x+y+z;
            r.push_back({x+y+z,i,j});
        }
    } 
    sort(all(r));
    vector<array<int,2>> hoge(n);
    for(int i=0;i<n;i++){
        hoge[i] = {0, i};
        for(int j=0;j<n;j++) hoge[i][0] += mat[i][j];
    }
    sort(all(hoge));

    vector<array<int,2>> mad;
    for(int i=0;i<n;i++) {
        int d1 = 0, d2 = 0, d3 = 0;
        for(int j=0;j<2;j++) {
            d1 += abs(atcoder[j] - a[i][j]);
            d2 += abs(atcoder[j] - b[i][j]);
            d3 += abs(a[i][j] - b[i][j]);
        }
        mad.push_back({max(d1, d2), i});
    }
    sort(all(mad));

    vector<int> now;
    for(int i=0;i<(int)mad.size();i++)now.push_back(mad[i][1]);

    int bestscore = solve(time, now);
    vector<int> best1 = ans1;
    vector<array<int,3>> best2 = ans2;
    int R = 1500;
    while(time.elapsed() < 1000) {
        int x = randint() % m;
        int y = randint() % 200;
        y += m;
        swap(now[x], now[y]);
        if(chmin(bestscore, solve(time, now))){
            best1 = ans1;
            best2 = ans2;
        }
        else if(R * (2000 - time.elapsed()) > 2000*(randint()%R)){
            continue;
        }
        else {
            swap(now[x], now[y]);
        }
    }

    now.clear();
    for(int i=0;i<(int)hoge.size();i++)now.push_back(hoge[i][1]);

    while(time.elapsed() < 1930) {
        int x = randint() % m;
        int y = randint() % 200;
        y += m;
        swap(now[x], now[y]);
        if(chmin(bestscore, solve(time, now))){
            best1 = ans1;
            best2 = ans2;
        }
        else if(R * (2000 - time.elapsed()) > 2000*(randint()%R)){
            continue;
        }
        else {
            swap(now[x], now[y]);
        }
    }

    ans1 = best1;
    ans2 = best2;

    answer();
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    for(int i=0;i<n;i++){
        a[i].resize(2);
        b[i].resize(2);
        cin>>a[i]>>b[i];
    }
    Timer time;
    // solve(time);
    yaki(time);
    cerr << "[time]: " << time.elapsed() << endl;
}
