#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define totori signed
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
ll dx[4]={0,1,-1,0};
ll dy[4]={1,0,0,-1};


using lint = long long;
constexpr int MOD = 1000000007, INF = 1010101010;
constexpr lint LINF = 1LL << 60;
 
int id, n = 100, k=9;
template<class T>
vector<T> make_vec(size_t s, T val) { return vector<T>(s, val); }
template<class... Size>
auto make_vec(size_t s, Size... tail) {
    return vector<decltype(make_vec(tail...))>(s, make_vec(tail...));
}
auto color = make_vec(n, n, 0);
auto base = make_vec(n, n, 0);
auto memo = make_vec(n, n, 0);

vector<string> s(100);


int check(array<int, 3> v) {
    int Y = v[0] - 1;
    int X = v[1] - 1;
    int C = v[2];
 
    color = base;
    int FirstC = color[Y][X];
    if (FirstC == C) return 0;
 
    queue<pair<int, int>> q;
    q.push(make_pair(Y, X));
    color[Y][X] = C;
 
    int point = 0;
    vector<int> vy = { 1, 0, -1, 0 };
    vector<int> vx = { 0, 1, 0, -1 };
    while (!q.empty()) {
        auto now = q.front(); q.pop();
        int nowY = now.first;
        int nowX = now.second;
 
        for (int k = 0; k < 4; k++) {
            int nextY = nowY + vy[k];
            int nextX = nowX + vx[k];
            if (nextY < 0 || nextY >= n) continue;
            if (nextX < 0 || nextX >= n) continue;
            if (color[nextY][nextX] == FirstC) {
                color[nextY][nextX] = C;
                q.push(make_pair(nextY, nextX));
            }
            if (color[nextY][nextX] == C) {
                point++;
            }
        }
    }

 
    return point;
}

int check_last(vector<array<int,3>> v){
    int color[100][100];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            color[i][j] = base[i][j];
        }
    }
    int Q = v.size();
        for (int i = 0; i < Q; i++){
            int Y = v[i][0]-1;
            int X = v[i][1]-1;
            int C = v[i][2];

            int FirstC = color[Y][X];

            if(C == FirstC) continue;

            queue<pair<int, int>> q;
            q.push(make_pair(Y, X));
            color[Y][X] = C;
            
            vector<int> vy = {1, 0, -1, 0};
            vector<int> vx = {0, 1, 0, -1};
            while(!q.empty()){
                auto now = q.front(); q.pop();
                int nowY = now.first;
                int nowX = now.second;

                for(int j = 0; j < 4; j++){
                    int nextY = nowY + vy[j];
                    int nextX = nowX + vx[j];
                    if(nextY < 0 || nextY >= n) continue;
                    if(nextX < 0 || nextX >= n) continue;
                    if(color[nextY][nextX] == FirstC){
                        color[nextY][nextX] = C;
                        q.push(make_pair(nextY, nextX));
                    }
                }
            }
        }
        
        int point = 0;
        for(int i = 1; i <= k; i++){
            int cnt = 0;
            for(int y = 0; y < n; y++){
                for(int x = 0; x < n; x++){
                    if(color[y][x] == i) cnt++;
                }
            }
            point = max(point, cnt);
        }
        point *= 100;
        point -= Q;
        //cerr << point << endl;
        return point;
}

class UnionFind{
    public:
    vector<int> parent;

    UnionFind(int N){
        parent = vector<int>(N,-1);
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

int f(int x,int y){
    return x*100 + y;
}

UnionFind uni(10000);


totori main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    //ifstream cin("in.txt");
    //ofstream cout("out.txt");
    cin>>id>>id>>id;
    for(int i=0;i<n;i++){
        cin>>s[i];
        for(int j=0;j<n;j++){
            base[i][j] = s[i][j] - '0';
        }
    }

    vector<array<int, 3>> xyc;
    {    
        while (true) {
    
            int now = 0;
            for (int c = 1; c <= 9; c++) {
                if (chmax(now, check(array<int, 3>{n / 2, n / 2, c}))) {
                    memo = color;
                }
            }
    
            base = memo;
            xyc.emplace_back(array<int, 3>{n / 2, n / 2, base[n / 2 - 1][n / 2 - 1]});
    
            int point = 0;
            for (int i = 1; i <= k; i++) {
                int cnt = 0;
                for (int y = 0; y < n; y++) {
                    for (int x = 0; x < n; x++) {
                        if (base[y][x] == i) cnt++;
                    }
                }
                point = max(point, cnt);
            }
            if (point == n * n) break;
        }
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            base[i][j] = s[i][j] - '0';
        }
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int y=0;y<4;y++){
                int nx = i + dx[y], ny = j + dy[y];
                if(nx>=0 && nx<n && ny>=0 && ny<n && s[nx][ny] == s[i][j]){
                    uni.unite(f(i,j),f(nx,ny));
                }
            }
        }
    }
    int m = 0;
    int h=n,w=n;
    set<int> st[10000];
    int par[10000]={};
    int pos[10000]={};
    set<int> rem;
    int color2[10000]={};
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(f(i,j) == uni.root(f(i,j))){
                par[m] = f(i,j);
                pos[f(i,j)] = m;
                rem.insert(m);
                color2[m] = base[i][j];
                m++;
            }
        }
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<4;k++){
                int nx = i + dx[k], ny = j + dy[k];
                if(nx>=0 && nx<h && ny>=0 && ny<w && s[nx][ny] != s[i][j]){
                    int r1 = uni.root(f(i,j));
                    int r2 = uni.root(f(nx,ny));
                    st[pos[r1]].insert(pos[r2]);
                }
            }
        }
    }
    vector<array<int,3>> ans;
    bool used[m]={};

    while(rem.size() > 1){
        int ma = 0;
        int r,col;
        for(auto i:rem){
            int cnt[10]={};
            vector<int> era;
            for(auto j:st[i]){
                if(used[j]){
                    era.push_back(j);
                }
            }
            for(auto j:era){
                st[i].erase(j);
            }
            for(auto j:st[i]){
                cnt[color2[j]]++;
            }
            for(int j=0;j<10;j++){
                if(chmax(ma,cnt[j])){
                    r = i;
                    col = j;
                }
            }
        }
        ans.push_back({uni.root(par[r])/100+1, uni.root(par[r])%100+1, col});
        color2[r] = col;

        set<int> era;
        for(auto i:st[r]){
            if(color2[i] == col){
                era.insert(i);
                used[i] = 1;
            }
        }
        for(auto i:era){
            st[r].erase(i);
            rem.erase(i);
            for(auto j:st[i]){
                if(!used[j]){
                    if(j == r)continue;
                    st[j].insert(r);
                    st[r].insert(j);
                }
            }
        }
    }

    
    if(ans.size() < xyc.size()){
        cout << ans.size() << endl;
        for(auto i:ans){
            cout << i[0] << " " << i[1] << " " << i[2] << endl;
        }
    }
    else {
        cout << xyc.size() << "\n";
        for (const auto& [y, x, c] : xyc) cout << y << " " << x << " " << c << "\n";
    }
    

    
}
