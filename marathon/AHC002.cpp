#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).endtime()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0};
long double eps = 1e-9;
long double pi = acos(-1);

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

int sx,sy;
const int n = 50;
int t[n][n], p[n][n];
vector<vector<array<int,2>>> v(n*n);

string ans = "LURD";
void print(vector<int> v){
    for(auto i:v){
        cout << ans[i];
    }
    cout << "\n";
}


int MAX=0;
vector<int> BEST;
vector<int> root;
bool used[n][n];
int score = 0;
int itr = 0;
int endtime = 1700;

bool dfs(int x,int y, Timer &time){
    ++itr;
    if((itr&(1<<10)) && time.elapsed()>endtime)return 0;
    used[x][y]=1;
    score += p[x][y];
    int i=0;
    if(root.size()){
        i = root.back()+3;
        if(i>=4)i-=4;
    }
    bool ret = 1;
    for(int _=0;_<4;++_){
        i++;
        if(i>=4)i-=4;
        int nx=dx[i]+x,ny=dy[i]+y;
        if(nx>=0 && nx<n && ny>=0 && ny<n && !used[nx][ny]){
            int tile = t[nx][ny];
            if(used[v[tile][0][0]][v[tile][0][1]])continue;
            if(v[tile].size() == 1){
                root.emplace_back(i);
                if(chmax(MAX,score+p[nx][ny])){
                    BEST = root;
                }
                ret &= dfs(nx,ny,time);
                if(ret == false)break;
                root.pop_back();
            }
            else if(used[v[tile][1][0]][v[tile][1][1]] == false){
                root.emplace_back(i);
                if(chmax(MAX,score+p[nx][ny])){
                    BEST = root;
                }
                ret &= dfs(nx,ny,time);
                if(ret == false)break;
                root.pop_back();
            }
        }
    }
    used[x][y]=0;
    score -= p[x][y];
    return ret;
}

int ref_time=1980;
bool visited[n][n];

bool tile_check(int x,int y){
    int tile = t[x][y];
    if(visited[v[tile][0][0]][v[tile][0][1]])return false;
    if(v[tile].size() == 1) return true;
    else if(visited[v[tile][1][0]][v[tile][1][1]] == false){
        return true;
    }
    return false;
}
int dif = 0;
void refine(Timer &time){
    int x=sx,y=sy;
    visited[x][y]=1;
    for(auto i:BEST){
        x+=dx[i],y+=dy[i];
        visited[x][y]=1;
    }
    /*
    ....    .--.
    ---- -> -||- みたいにする. 遅いけど、vector::insertで？
    ....    ....
    */ 
    x=sx,y=sy;
    int r = 0;
    while(ref_time>time.elapsed() && r<(int)BEST.size()){
        for(;r<(int)BEST.size();){
            if(BEST[r]==0 || BEST[r]==2){
                // Right / Left
                // string ans = "RDLU"; 
                int ny = y + dy[BEST[r]];
                if(x!=0 && !visited[x-1][y] && !visited[x-1][ny] && t[x-1][y]!=t[x-1][ny] && tile_check(x-1,y) && tile_check(x-1,ny)){
                    BEST.insert(BEST.begin()+r,3);
                    BEST.insert(BEST.begin()+r+2,1);
                    dif += p[x-1][ny] + p[x-1][y];
                    visited[x-1][ny]=1;
                    visited[x-1][y]=1;
                    break;
                }
                if(x!=n-1 && !visited[x+1][y] && !visited[x+1][ny] && t[x+1][y]!=t[x+1][ny] && tile_check(x+1,y) && tile_check(x+1,ny)){
                    BEST.insert(BEST.begin()+r,1);
                    BEST.insert(BEST.begin()+r+2,3);
                    dif += p[x+1][y] + p[x+1][ny];
                    visited[x+1][y]=1;
                    visited[x+1][ny]=1;
                    break;
                }
            }
            if(BEST[r]==1 || BEST[r]==3){
                // U / D
                // string ans = "RDLU";
                int nx = x + dx[BEST[r]];
                if(y!=0 && !visited[x][y-1] && !visited[nx][y-1] && t[x][y-1]!=t[nx][y-1] && tile_check(x,y-1) && tile_check(nx,y-1)){
                    BEST.insert(BEST.begin()+r,2);
                    BEST.insert(BEST.begin()+r+2,0);
                    dif += p[x][y-1] + p[nx][y-1];
                    visited[x][y-1]=1;
                    visited[nx][y-1]=1;
                    break;
                }
                if(y!=n-1 && !visited[x][y+1] && !visited[nx][y+1] && t[x][y+1]!=t[nx][y+1] && tile_check(x,y+1) && tile_check(nx,y+1)){
                    BEST.insert(BEST.begin()+r,0);
                    BEST.insert(BEST.begin()+r+2,2);
                    dif += p[x][y+1] + p[nx][y+1];
                    visited[x][y+1]=1;
                    visited[nx][y+1]=1;
                    break;
                }
            }
            x+=dx[BEST[r]],y+=dy[BEST[r]];
            r++;
        }
    }
}


// idea::道のどこかからDFSをして、既に訪れた事のあるマスに到達→その2つの道を入れ替えてスコアが高くなるならswap？ -> muri (nokori 30minute)

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    ifstream cin("in.txt");
    cin>>sx>>sy;
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            cin>>t[i][j];
            v[t[i][j]].push_back({i,j});
        }
    }
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            cin>>p[i][j];
        }
    }
    endtime = 1950;
    ref_time = 1980;
    // endtime = 14;
    // ref_time = 29;
    Timer time;
    dfs(sx,sy,time);
    refine(time);
    print(BEST);
    // cerr << itr << " " << BEST.size() << " " << MAX << " " << dif << " " << time.elapsed() << endl;
}
