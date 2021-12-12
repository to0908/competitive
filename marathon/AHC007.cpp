#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
long double eps = 1e-9;
template< typename T > istream &operator>>(istream &is, vector< T > &v) {
    for(T &in : v) is >> in;
    return is;
}
template< typename T > ostream &operator<<(ostream &os, const vector< T > &v) {
    for(int i = 0; i < (int) v.size(); i++) {
        os << v[i] << (i + 1 != (int) v.size() ? " " : "");
    }
    return os;
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
unsigned int randint() {
    static unsigned int tx = 123456789, ty=362436069, tz=521288629, tw=88675123;
    unsigned int tt = (tx^(tx<<11));
    tx = ty; ty = tz; tz = tw;
    return ( tw=(tw^(tw>>19))^(tt^(tt>>8)) );
}

class UnionFind{
    public:
    //親の番号を格納する。親だった場合は-(その集合のサイズ)
    vector<int> parent;

    UnionFind(int N){
        parent = vector<int>(N,-1);
    }
    UnionFind(vector<int> &v) {
        parent = v;
    }

    int root(int A){
        if(parent[A] < 0) return A;
        return parent[A]=root(parent[A]);
    }

    int size(int A){
        return -parent[root(A)];
    }

    bool unite(int A, int B) {
        A = root(A), B = root(B);
        if(A == B) return false; 

        if(size(A) < size(B)) swap(A,B);
        parent[A] += parent[B];
        parent[B] = A;
        return true;
    }

    bool same(int A, int B){
        return root(A)==root(B);
    } 
};

const int n = 400, m = 1995;
array<int,2> p[n];
array<int,3> edge[m];
array<int,2> board[n][n];
vector<array<int, 3>> v(m);
vector<double> params = {
    1.69, 
    5000.0, 
    800000
};

int dist(int x, int y) {
    int ret = (p[x][0] - p[y][0]) * (p[x][0] - p[y][0]);
    ret += (p[x][1] - p[y][1]) * (p[x][1] - p[y][1]);
    return sqrt(ret) + 0.5;
}

int expected_dist(int x, int y, double a=1.81) {
    int ret = a * dist(x, y);
    return ret;
}

vector<bool> mst(m);

void get_expected(int query, vector<int> &univec) {
    int rem = m - query;
    v = vector<array<int, 3>>(rem);
    double t = (double)query / params[1];
    double a = params[0];
    a += t;
    for(int i=query;i<m;i++) {
        auto [d, x, y] = edge[i];
        int ed = expected_dist(x, y, a);
        v[i - query] = {ed, x, y};
    }
    sort(all(v));
    UnionFind uni(univec);
    mst = vector<bool>(m);
    for(int i=0;i<(int)v.size();i++){
        auto [d,x,y] = v[i];
        if(uni.unite(x, y)) {
            int idx = board[x][y][1];
            mst[idx] = 1;
        }
    }
}

void init() {
    for(int i=0;i<n;i++) {
        int x, y;
        cin>>x>>y;
        p[i] = {x, y};
    }
    for(int i=0;i<m;i++) {
        int x,y;
        cin>>x>>y;
        int d = expected_dist(x, y);
        edge[i] = {d, x, y};
        board[x][y] = {d, i};
        v[i] = {d, x, y};
    }
}

bool douse(int query, int cost, vector<int> &univec) {
    int usecost = cost;
    int notusecost = 0;
    UnionFind use(univec);
    auto [d, x, y] = edge[query];
    assert(use.unite(x, y));
    UnionFind notuse(univec);
    vector<int> usevec, notusevec;
    for(int i=0;i<(int)v.size();i++){
        auto [d, x, y] = v[i];
        int idx = board[x][y][1];
        if(idx <= query) continue;
        if(use.unite(x, y)) {
            usevec.push_back(idx);
            usecost += d;
        }
        if(notuse.unite(x, y)) {
            notusevec.push_back(idx);
            notusecost += d;
        }
    }
    if(notuse.size(0) < n or usecost < notusecost) {
        mst = vector<bool>(m);
        for(auto i:usevec) mst[i] = 1;
        return true;
    }
    else {
        mst = vector<bool>(m);
        for(auto i:notusevec) mst[i] = 1;
        return false;
    }
}

bool douse2(int query, int cost, UnionFind &uni){
    auto [d, x, y] = edge[query];
    double ret = 1;
    // 今後、costより大きいものしか出てこない可能性
    for(int i=0;i<(int)v.size();i++){
        auto [exd, nx, ny] = v[i];
        int idx = board[nx][ny][1];
        if(idx <= query) continue;
        bool pos = uni.same(nx, x) && uni.same(ny, y);
        pos |= uni.same(ny, x) && uni.same(nx, y);
        if(pos) {
            int d = dist(nx, ny);
            int ma = 3 * d;
            double p = double(ma - cost) / double(ma - d + 1);
            chmax(p, 0);
            chmin(p, 1);
            ret *= p;
        }
    }
    return ret > 0.5;
}

bool isConnected(int query, vector<int> &univec){
    UnionFind uni(univec);
    for(int i=0;i<(int)v.size();i++){
        auto [exd, nx, ny] = v[i];
        int idx = board[nx][ny][1];
        if(idx <= query) continue;
        uni.unite(nx, ny);
    }
    return uni.size(0) == n;
}

void solve(Timer &time) {
    UnionFind uni(n);
    for(int i=0;i<m;i++) {
        if(i % 100 == 0) get_expected(i, uni.parent);
        int d;
        cin>>d;
        auto [ed, x, y] = edge[i];
        if(uni.same(x, y)) {
            cout << 0 << endl;
            continue;
        }
        if(mst[i]) {
            if(d > ed) {
                bool use = douse(i, d, uni.parent);
                if(use) use = douse2(i, d, uni);
                if(!use && isConnected(i, uni.parent)) {
                    cout << 0 << endl;
                    continue;
                }
            }
            uni.unite(x, y);
            cout << 1 << endl;
        }
        else {
            if(d < ed) {
                bool use = douse(i, d, uni.parent);
                if(use) use = douse2(i, d, uni);
                if(use) {
                    uni.unite(x, y);
                    cout << 1 << endl;
                    continue;
                }
            }
            if(isConnected(i, uni.parent)) {
                cout << 0 << endl;
            }
            else {
                cout << 1 << endl;
                uni.unite(x, y);
            }
        }
    }
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);


    Timer time;
    init();
    solve(time);
    cerr << "time: " << time.elapsed() << endl;

}
