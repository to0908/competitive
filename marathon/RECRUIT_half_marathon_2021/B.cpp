#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0};
long double eps = 1e-9;
long double pi = acos(-1);

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

// int n,m,k;

const int n = 40;
int p[n][n];
array<int,3> a[n*n]; // {p,i,j}
int bestscore = 0;
vector<array<int,2>> best(n*n); // {idx(a[idx]),power}
int ans[n][n];

int f(int x,int y){ return x*n+y; }

int score(vector<array<int,2>> &cur){
    int ret = 0;
    for(int i=0;i<n*n;i++){
        auto [idx,power] = cur[i];
        auto [e,x,y] = a[idx];
        ret += power * e;
    }
    return ret;
}

int dist(int x1,int y1,int x2,int y2){
    return abs(x1-x2) + abs(y1-y2);
}


void init(){
    // 各頂点から、距離Kで行ける頂点を全列挙
    // パワーとKからK+1に変更するとき、K+1の頂点の利益の総和がその頂点の利益より大きいとダメ?
    for(int musi=0;musi<=15;musi++){
        int sum[n][n][n+2]={};
        vector<array<int,2>> cur(n*n); // {idx(a[idx]),power}
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                for(int k=1;k<=n;k++){
                    for(int dx=-k;dx<=k;dx++){
                        int x = i + dx;
                        if(x < 0 || x >= n)continue;
                        for(int dy=-1;dy<=1;dy++){
                            if(dy == 0)continue;
                            if(k-abs(dx) == 0 && dy == 1)continue;
                            int y = j + dy * (k - abs(dx));
                            if(y < 0 || y>=n)continue;
                            if(p[x][y] <= musi)continue;
                            sum[i][j][k] += p[x][y];
                        }
                    }
                }
            }
        }
        for(int i=0;i<n*n;i++){
            bool ok = true;
            auto [po,x,y] = a[i];
            int ma = n;
            for(int j=0;j<i;j++){
                auto [idx,di] = cur[j];
                auto [p2,x2,y2] = a[idx];
                if(di > 0)chmin(ma,dist(x,y,x2,y2)-1);
                if(dist(x,y,x2,y2) <= di){
                    ok = false;
                    break;
                }
            }
            int ret = 0;
            if(ok){
                for(int j=1;j<=ma;j++){
                    if(po >= sum[x][y][j] - (sum[x][y][j+1]/100)){
                        ret = j;
                    }
                    else break;
                }
            }
            cur[i] = {i,ret};
            if(ret == 0){
                if(p[x][y] >= musi)
                for(int k=1;k<=n;k++){
                    for(int dx=-k;dx<=k;dx++){
                        int x2 = x + dx;
                        if(x2 < 0 || x2 >= n)continue;
                        for(int dy=-1;dy<=1;dy++){
                            if(dy == 0)continue;
                            if(k-abs(dx) == 0 && dy == 1)continue;
                            int y2 = y + dy * (k - abs(dx));
                            if(y2 < 0 || y2>=n)continue;
                            sum[x2][y2][k] -= p[x][y];
                        }
                    }
                }
            }
        }
        if(chmax(bestscore,score(cur))){
            swap(best,cur);
        }
    }
}

void yaki(){
    Timer time;
    int nn = n*n;
    vector<array<int,2>> cur(n*n);
    int end = 1500;
    int R = 1000;
    int curscore = bestscore;
    while(time.elapsed() < end){
        for(int _=0;_<10;_++){
            int r = randint() % (nn);
            while(cur[r][1] == r)r = randint() % (nn);
            vector<array<int,2>> tmp = {{r,cur[r][1]}};
            cur[r][1] = 0;

            for(int i=r+1;i<n*n;i++){
                bool ok = true;
                auto [po,x,y] = a[i];
                int ma = n;
                for(int j=0;j<i;j++){
                    auto [idx,di] = cur[j];
                    auto [p2,x2,y2] = a[idx];
                    if(di > 0)chmin(ma,dist(x,y,x2,y2)-1);
                    if(dist(x,y,x2,y2) <= di){
                        ok = false;
                        break;
                    }
                }
                int ret = 0;
                if(ok){
                    ret = ma;
                    // for(int j=1;j<=ma;j++){
                    //     if(po >= sum[x][y][j] - (sum[x][y][j+1]/100)){
                    //         ret = j;
                    //     }
                    //     else break;
                    // }
                }
                if(cur[i][1] != ret)tmp.push_back({i,cur[i][1]});
                cur[i] = {i,ret};
            }
            int nowscore = score(cur);
            const bool force_next = R * (end - time.elapsed()) > end*(randint()%R);
            if(chmax(bestscore,nowscore)){
                best = cur;
            }
            else if(chmax(curscore, nowscore) || force_next){
                curscore = nowscore;
            }
            else {
                for(auto x:tmp){
                    cur[x[0]][1] = x[1];
                }
            }
        }
    }
}

void print(){
    for(int i=0;i<n*n;i++){
        auto [idx,ret] = best[i];
        auto [po,x,y] = a[idx];
        ans[x][y] = ret;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout << ans[i][j] << " ";
        }
        cout << "\n";
    }
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    cin>>a[0][0];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>p[i][j];
            // if(p[i][j] <= 2)p[i][j]=0;
            a[f(i,j)] = {p[i][j],i,j};
        }
    }
    sort(a,a+n*n,greater<array<int,3>>());
    init();
    // yaki();
    print();
}
