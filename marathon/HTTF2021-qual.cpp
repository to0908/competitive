#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0};
long double eps = 1e-9;
long double pi = acos(-1);


bool local = 0;

const int h = 20, w = 20;
const int n = 100;
int x[n],y[n];
bool board[h][w];

struct S{
    int x,y;
};

/*
解法
5*5で1区画として、4*4区画に分割
 0   3  4  5
 1   2  7  6
14  15  8  9
13  12 11 10

1区間の中のカード+次の区間への距離でbitDPを行い、区間内の回収+次の区間への移動を最適化
→適当にカードを再配置+回収
→時間いっぱい焼きなましで再配置+回収の最適化
*/

const int range = 5;
const S perm[] = {
    {0,0},{1,0},{1,1},{0,1},{0,2},{0,3},
    {1,3},{1,2},{2,2},{2,3},{3,3},{3,2}
};


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

string ans = "";

void move(string &s, S now, int tox, int toy, char c){
    if(c=='I')board[tox][toy] = false;
    else board[tox][toy] = true;

    if(tox < now.x){
        for(int j=0;j<now.x - tox;j++) s += "U";
    }
    else {
        for(int j=0;j<tox - now.x;j++) s += 'D';
    }
    if(toy < now.y){
        for(int j=0;j<now.y - toy;j++) s += "L";
    }
    else {
        for(int j=0;j<toy - now.y;j++) s += 'R';
    }
    s += c;
}



int bestscore;
int base_cost;
string best_str;
string base_str;
vector<S> emp;
stack<int> st_tmp;
S start;
vector<S> bestseq;

// 焼きなまし
int yakinamashi(){
    Timer time;
    int end = 2975;
    int curscore = bestscore;
    vector<S> curseq = emp;
    bestseq = emp;
    int k = st_tmp.size();
    int m = emp.size();
    while(time.elapsed() < end){
        for(int _=0;_<1000;_++){
            int l = randint() % m, r = randint()%m;
            while(l==r)r=randint()%m;
            swap(curseq[l],curseq[r]);
            S now = start;
            int score = 0;
            stack<int> st = st_tmp;

            // 置く
            string ret = "";
            for(int i=0;i<k;i++){
                int p = st.top();
                st.pop();
                x[p] = curseq[i].x, y[p] = curseq[i].y;
                score += abs(now.x - x[p]) + abs(now.y - y[p]);
                now = {x[p],y[p]}; 
            }
            // 回収
            for(int i=0;i<n;i++){
                score += abs(x[i] - now.x) + abs(y[i] - now.y);
                now = {x[i],y[i]};
            }
            int R = 1000;
            const bool force_next = R * (end - time.elapsed()) > end*(randint()%R);
            if(chmin(bestscore,score+base_cost)){
                bestseq = curseq;
            }
            else if(force_next || score<curscore){
                curscore = score;
            }
            else swap(curseq[l],curseq[r]);
        }
    }
    return bestscore;
}

int solve(){
    int cost = 0;
    string ret = "";
    S now = {0,0};
    int seg = 0;
    stack<int> st;
    int m = 0;
    vector<vector<int>> v;
    int sz = 0;
    while(seg < 12){
        vector<int> vt;
        for(int i=0;i<n;i++){
            if(x[i]<perm[seg].x*range || x[i]>=(perm[seg].x+1)*range)continue;
            if(y[i]<perm[seg].y*range || y[i]>=(perm[seg].y+1)*range)continue;
            if(x[i] == 9 && y[i] <= 10)continue;
            if(y[i] == 10 && x[i] >= 9)continue;
            vt.push_back(i);
        }

        // もしmが20個くらいになると、2^m * m^2 = 4*10^8 なので、きつい
        // 提出してみると、一瞬で終わったのでそんなケースは存在しない。勝った！ｗ
        chmax(m,(int)vt.size());
        if((int)vt.size() != 0){
            sz++;
            v.push_back(vt);
        }
        seg++;
    }
    {
        // bitDPで最適な回り方を見つける
        int dp[sz][(1<<m)][m];
        for(int k=0;k<sz;k++)for(int i=0;i<(1<<m);i++)for(int j=0;j<m;j++)dp[k][i][j] = 1e9;
        for(int i=0;i<(int)(v[0].size());i++)dp[0][(1<<i)][i] = abs(x[v[0][i]] - now.x) + abs(y[v[0][i]] - now.y);
        for(int k=0;k<sz;k++){
            int vs = v[k].size();
            if(k){
                int ps = v[k-1].size();
                int bit = (1<<ps) - 1;
                for(int i=0;i<ps;i++){
                    for(int j=0;j<vs;j++){
                        chmin(dp[k][(1<<j)][j],dp[k-1][bit][i] + abs(x[v[k-1][i]] - x[v[k][j]]) +  abs(y[v[k-1][i]] - y[v[k][j]]));
                    }
                }
            }
            for(int bit=0;bit<(1<<vs);bit++){
                for(int i=0;i<vs;i++){
                    if(bit & (1<<i)){
                        for(int j=0;j<vs;j++){
                            if(bit & (1<<j)){
                                if(i==j)continue;
                                chmin(dp[k][bit][i],dp[k][bit^(1<<i)][j] + abs(x[v[k][i]] - x[v[k][j]]) + abs(y[v[k][i]] - y[v[k][j]]));
                            }
                        }
                    }
                }
            }
        }
        // bitDPの復元
        stack<int> bst;
        int pre=-1;
        int mi = 1e9;
        int vs = v[sz-1].size();
        int bit = (1<<vs) - 1;
        for(int i=0;i<(int)v[sz-1].size();i++){
            if(chmin(mi,dp[sz-1][bit][i])){
                pre=i;
            }
        }
        for(int k=sz-1;k>=0;k--){
            int vs = v[k].size();
            int bit = (1<<vs) - 1;
            bit ^= (1<<pre);
            bst.push(v[k][pre]);
            while(bit){
                int las = -1;
                int mi = 1e9;
                for(int i=0;i<vs;i++){
                    if(bit & (1<<i)){
                        int dist = dp[k][bit][i] + abs(x[v[k][i]] - x[v[k][pre]]) + abs(y[v[k][i]] - y[v[k][pre]]);
                        if(dp[k][bit^(1<<pre)][pre] == dist){
                            las = i;
                            chmin(mi,abs(x[v[k][i]] - x[v[k][pre]]) + abs(y[v[k][i]] - y[v[k][pre]]));
                        }
                    }
                }
                pre = las;
                assert(las!=-1);
                bst.push(v[k][las]);
                bit -= (1<<las);
            }
            if(k){
                int las = -1;
                // dp[k][(1<<pre)][pre] <- dp[k-1][1111][hoge]
                int ps = v[k-1].size();
                for(int i=0;i<ps;i++){
                    int dist = dp[k-1][(1<<ps)-1][i];
                    dist += abs(x[v[k-1][i]] - x[v[k][pre]]) + abs(y[v[k-1][i]] - y[v[k][pre]]);
                    if(dp[k][(1<<pre)][pre] == dist){
                        las = i;
                    }
                }
                assert(las!=-1);
                pre = las;
            }
        }
        while(bst.size()){
            int p = bst.top();
            bst.pop();
            cost += abs(x[p] - now.x) + abs(y[p] - now.y);
            move(ret,now,x[p],y[p],'I');
            now = {x[p],y[p]};
            st.push(p);
        }
    }


    base_cost = cost;
    base_str = ret;
    st_tmp = st;
    start = now;

    // 左下10*10マスに敷き詰める。ここがかなり無駄(回収でめっちゃぐるぐるするので)
    // greedy
    for(int i=19;i>=10;i--){
        for(int j=0;j<10;j++){
            if(st.empty())break;
            if(i&1){
                if(board[i][9-j] == false){
                    emp.push_back({i,9-j});
                    cost += abs(i - now.x) + abs(9-j - now.y);
                    move(ret,now,i,9-j,'O');
                    now = {i,9-j};
                    int p = st.top();
                    x[p] = i, y[p] = 9-j;
                    st.pop();
                }
            } 
            else if(board[i][j] == false){
                emp.push_back({i,j});
                cost += abs(i - now.x) + abs(j - now.y);
                move(ret,now,i,j,'O');
                now = {i,j};
                int p = st.top();
                x[p] = i, y[p] = j;
                st.pop();
            }
        }
    }

    // ここ、しない方が良い？ わからん
    //{
    //    for(int i=0;i<=10;i++)if(board[9][i] == false)emp.push_back({9,i});
    //    for(int i=10;i<=19;i++)if(board[i][10] == false)emp.push_back({i,10});
    //}

    // 最後は0~100なので.
    for(int i=0;i<n;i++){
        cost += abs(x[i] - now.x) + abs(y[i] - now.y);
        move(ret,now,x[i],y[i],'I');
        now = {x[i],y[i]};
    }
    bestscore = cost;
    ans = ret;
    if(chmin(cost,yakinamashi())){
        ans = base_str;
        now = start;
        st = st_tmp;
        int k = st.size();
        for(int i=0;i<k;i++){
            int p = st.top();
            st.pop();
            x[p] = bestseq[i].x, y[p] = bestseq[i].y;
            move(ans,now,x[p],y[p],'O');
            now = {x[p],y[p]}; 
        }
        for(int i=0;i<n;i++){
            move(ans,now,x[i],y[i],'I');
            now = {x[i],y[i]};
        }
    }
    return cost;
}

vector<int> MAX_SCORE = {3283,3238,3220,3237,3205};


signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);


    int q = 1;
    int sum = 0;
    for(int i=0;i<q;i++){
        // for(int j=0;j<h;j++)for(int k=0;k<w;k++)board[i][j] = 0;
        // string inputfile = "testCase_" + to_string(i) + ".txt";
        // ifstream cin(inputfile);
        // ofstream cout("out.txt");
        Timer time;
        for(int i=0;i<n;i++){
            cin>>x[i]>>y[i];
            board[x[i]][y[i]] = 1;
        }
        int cost = solve();
        sum += 4000 - cost;
        cout << ans << endl;
        cerr << "MAX: " << MAX_SCORE[i] << endl;
        cerr << "SCORE: " << 4000 - cost << endl;
        cerr << time.elapsed() << " [ms]" << endl << endl;
        emp.clear();
    }
    cerr << "COMPARE: " << sum << " " << accumulate(all(MAX_SCORE),0) << endl;
}
