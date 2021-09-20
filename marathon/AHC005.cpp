#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0};
string road = "RDLU";
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

int n,sx,sy;
int road_cnt = 0, cross_cnt = 1;
const int INF = 1e9;
const int MAX_ROAD = 140, MAX_TIME = 2800;

vector<vector<int>> s, grid, dist;
vector<vector<string>> idou;
// 4*69 == 276
vector< bitset<MAX_ROAD> > bits;
vector<vector<int>> v;


void init(){
    cin>>n>>sx>>sy;
    s = vector<vector<int>>(n,vector<int>(n,-1));
    grid = vector<vector<int>>(n,vector<int>(n,-1));
    for(int i=0;i<n;i++){
        string t; cin>>t;
        for(int j=0;j<n;j++){
            if(t[j]!='#') s[i][j] = t[j]-'0';
        }
    }

    // 交差点の番号付け
    grid[sx][sy] = 0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==sx && j == sy)continue;
            if(s[i][j]==-1)continue;
            bool h=false,w=false;
            if(i && s[i-1][j]!=-1)h=1;
            if(i+1<n && s[i+1][j]!=-1)h=1;
            if(j && s[i][j-1]!=-1)w=1;
            if(j+1<n && s[i][j+1]!=-1)w=1;
            if(h&w) grid[i][j] = cross_cnt;
            cross_cnt += (h&w);
        }
    }
    assert(cross_cnt < 600);
    dist = vector<vector<int>>(cross_cnt,vector<int>(cross_cnt,INF));
    idou = vector<vector<string>>(cross_cnt,vector<string>(cross_cnt,""));
    bits = vector< bitset<MAX_ROAD> >(cross_cnt);
    v.resize(cross_cnt);
    for(int i=0;i<cross_cnt;i++)dist[i][i] = 0;

    // cerr << cross_cnt << endl;

    // どちから1方向に進んだ時の交差点間の距離
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(grid[i][j]==-1)continue;
            for(int k=0;k<4;k++){
                int sum = 0;
                int nx = i + dx[k], ny = j + dy[k];
                string ret = "";
                bool used = false;
                while(nx>=0 && nx<n && ny>=0 && ny<n && s[nx][ny] != -1){
                    sum += s[nx][ny];
                    ret += road[k];
                    if(!used && grid[nx][ny]!=-1){
                        used = 1;
                        v[grid[i][j]].push_back(grid[nx][ny]);
                    }
                    if(grid[nx][ny] != -1 && chmin(dist[grid[i][j]][grid[nx][ny]], sum)){
                        idou[grid[i][j]][grid[nx][ny]] = ret;
                    }
                    nx += dx[k];
                    ny += dy[k];
                }
            }
        }
    }

    // ワーシャルフロイド
    for(int k=0;k<cross_cnt;k++)for(int i=0;i<cross_cnt;i++)for(int j=0;j<cross_cnt;j++){
        if(chmin(dist[i][j],dist[i][k]+dist[k][j])){
            idou[i][j] = idou[i][k] + idou[k][j];
        }
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(s[i][j]==-1)continue;
            if(i==0 || (i && s[i-1][j] == -1)){
                if(i+1<n && s[i+1][j] != -1){
                    int nx = i;
                    while(nx<n && s[nx][j] != -1){
                        if(grid[nx][j]!=-1) bits[grid[nx][j]][road_cnt] = 1;
                        nx++;
                    }
                    road_cnt++;
                }
            }
            if(j == 0 || (j && s[i][j-1] == -1)){
                if(j+1<n && s[i][j+1] != -1){
                    int ny = j;
                    while(ny<n && s[i][ny] != -1){
                        if(grid[i][ny]!=-1) bits[grid[i][ny]][road_cnt] = 1;
                        ny++;
                    }
                    road_cnt++;
                }
            }
        }
    }
    
    assert(road_cnt < MAX_ROAD);
    // bitset<MAX_ROAD> test;
    // for(auto i:bits)test |= i;
    // assert(road_cnt == test.count());
    
}


vector<int> best_seq;
int best_dist = INF;

void solve_di(Timer &time){
    // ダイクストラベースで
    unordered_map< bitset<MAX_ROAD> ,int> dp[cross_cnt];
    struct state{
        int dist,p;
        vector<int> vs;
        bitset<MAX_ROAD> bit;
        bool operator>(const state s) const{
            // if(dist == s.dist) return bit.count() < s.bit.count();
            // return dist > s.dist;
            if(bit.count() == s.bit.count()) return dist > s.dist;
            return bit.count() < s.bit.count();
        }
    };
    priority_queue<state,vector<state>,greater<state>> pq;
    pq.push({0, grid[sx][sy], vector<int>(), bits[grid[sx][sy]]});
    bitset<MAX_ROAD> nbit;
    bitset<MAX_ROAD> comp;
    for(int i=0;i<road_cnt;i++)comp[i] = 1;
    int iter = 0;
    while(pq.size()){
        iter++;
        if(iter%1000==0 && time.elapsed() > MAX_TIME)break;
        state S = pq.top();

        pq.pop();
        
        if(dp[S.p].count(S.bit) &&  dp[S.p][S.bit] < S.dist)continue;
        dp[S.p][S.bit] = S.dist;
        S.vs.push_back(S.p);

        if((int)S.bit.count() == road_cnt){
            if(S.p == 0){
                if(chmin(best_dist,S.dist)){
                    best_seq = S.vs;
                }
            }
            else{
                if(chmin(best_dist,S.dist + dist[S.p][0])){
                    best_seq = S.vs;
                    best_seq.push_back(0);
                }
            }
            continue;
        }
        
        for(auto i:v[S.p]){
            nbit = S.bit | bits[i];
            // if(nbit == S.bit)continue;
            int ndist = S.dist + dist[S.p][i];
            if(dp[i].count(nbit) && dp[i][nbit] <= ndist)continue;
            dp[i][nbit] = ndist;
            pq.push(state{ndist, i, S.vs, nbit});
        }
    }

    assert(best_seq.empty() == false);

    // bitset<MAX_ROAD> nowb = bits[0];
    // for(int i=1;i<(int)best_seq.size()-1;i++){
    //     if(nowb == (nowb | bits[best_seq[i]])){
    //         i = best_seq.erase(best_seq.begin() + i) - best_seq.begin();
    //         i--;
    //     }
    // }
    // nowb = bits[0];
    // for(int i=1;i<(int)best_seq.size()-1;i++){
    //     bitset<MAX_ROAD> nxb = nowb;
    //     for(int j=i+1;j<(int)best_seq.size()-1;j++){
    //         nxb |= bits[j];
    //     }
    //     if((int)nxb.count() == road_cnt){
    //         i = best_seq.erase(best_seq.begin() + i) - best_seq.begin();
    //         i--;
    //     }
    // }
}

int scoring(vector<int> &seq){
    int ret = 0;
    int now = 0;
    for(int i=1;i<(int)seq.size();i++){
        ret += dist[now][seq[i]];
        now = seq[i];
    }
    assert(now == 0);
    return ret;
}

void solve_yaki(Timer &time){
	int cur_dist = best_dist;
    vector<int> cur_seq = best_seq;
	int m = (int)cur_seq.size() - 2;
    int R = 1000;
	int end = 27;
	end = 600;
	while(time.elapsed() < end){
		for(int _=0;_<10;_++){
            int l = randint()%m, r = randint()%m;
            while(r==l)r=randint()%m;
            l++,r++;
            swap(cur_seq[l],cur_seq[r]);
            int yaki_dist = scoring(cur_seq);
            const bool force_next = R * (end - time.elapsed()) > end*(randint()%R);
            if(chmin(best_dist, yaki_dist)){
                // for(int i=0;i<n;i++)ans[i] = cur[i];
                best_seq = cur_seq;
                cerr << "AAAAAAAAAAAA" << endl;
            }
            else if(chmin(cur_dist, yaki_dist) || force_next){
                cur_dist = yaki_dist;
            }
            else swap(cur_seq[l],cur_seq[r]);
		}
	}
}

void solve_ins(Timer &time){
    while(true){
        bool moved = false;
        for(int i=1;i<(int)best_seq.size()-1;i++){
            for(int j=0;j<(int)best_seq.size()-1;j++){
                if(i == j)continue;
                if(i == j+1)continue;
                int cur_dist = best_dist;
                cur_dist -= dist[best_seq[j]][best_seq[j+1]];
                cur_dist += dist[best_seq[j]][best_seq[i]] + dist[best_seq[i]][best_seq[j+1]];
                cur_dist -= dist[best_seq[i-1]][best_seq[i]] + dist[best_seq[i]][best_seq[i+1]];
                cur_dist += dist[best_seq[i-1]][best_seq[i+1]];
                if(chmin(best_dist, cur_dist)){
                    vector<int> cur_seq;
                    for(int k=0;k<(int)best_seq.size();k++){
                        if(k == i)continue;
                        cur_seq.push_back(best_seq[k]);
                        if(k == j) cur_seq.push_back(best_seq[i]);
                    }
                    swap(cur_seq, best_seq);
                    moved = 1;
                    // cerr << "AAAAAAAAAAAAA" << endl;
                }
            }
        }
        if(!moved) break;
    }
}

void answer(){

    string ans = "";
    int now = 0;
    for(int i=0;i<(int)best_seq.size();i++){
        ans += idou[now][best_seq[i]];
        now = best_seq[i];
    }
    assert(now==0);
    cout << ans << endl;
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    Timer time;
    init();
    solve_di(time);
    // solve_yaki(time);
    solve_ins(time);
    answer();

}
