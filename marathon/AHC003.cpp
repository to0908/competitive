// https://atcoder.jp/contests/ahc003/submissions/23022040
#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0}; // RDLU
long double eps = 1e-9;
long double pi = acos(-1);
unsigned int randint() {
    static unsigned int tx = 123456789, ty=362436069, tz=521288629, tw=88675123;
    unsigned int tt = (tx^(tx<<11));
    tx = ty; ty = tz; tz = tw;
    return ( tw=(tw^(tw>>19))^(tt^(tt>>8)) );
}

const int n = 30;
int h[n][n-1], v[n-1][n]; // ?, vertical?
int ch[n][n-1], cv[n-1][n]; // ?, vertical?
const int L = 1300, R = 8700;

void init_weight(){
    for(int i=0;i<n;i++){
        int hd = randint()%7000 + L;
        int vd = randint()%7000 + L;
        for(int j=0;j<n-1;j++){
            h[i][j] = hd + randint()%2800 - 1400;
            chmax(h[i][j],L);
            chmin(h[i][j],R);
            v[j][i] = vd + randint()%2800 - 1400;
            chmax(v[j][i],L);
            chmin(v[j][i],R);

            // h[i][j] = v[j][i] = R-L;
            // h[i][j] = v[j][i] = 1;
        }
    }
}

const int INF = 1e9;
int sx,sy,gx,gy;
string ans;

int dijkstra(){
    ans = "";
    vector<vector<int>> dp(n,vector<int>(n,INF));
    vector<vector<int>> rp(n,vector<int>(n,INF));
    priority_queue<array<int,3>,vector<array<int,3>>,greater<array<int,3>>> pq;
    pq.push({0,sx,sy});
    dp[sx][sy] = 0;
    rp[gx][gy] = 0;
    while(pq.size()){
        auto [d,x,y] = pq.top();
        pq.pop();
        if(d > dp[x][y])continue;
        for(int k=0;k<4;k++){ // RDLU
            int nx = x + dx[k], ny = y + dy[k];
            if(nx>=0 && nx<n && ny>=0 && ny<n){
                if(k == 0){
                    if(chmin(dp[nx][ny],dp[x][y] + h[x][y]))pq.push({dp[nx][ny],nx,ny});
                }// R
                else if(k == 1){
                    if(chmin(dp[nx][ny],dp[x][y] + v[x][y]))pq.push({dp[nx][ny],nx,ny});
                } // D
                else if(k == 2){
                    if(chmin(dp[nx][ny],dp[x][y] + h[x][y-1]))pq.push({dp[nx][ny],nx,ny});
                } // L
                else {
                    if(chmin(dp[nx][ny],dp[x][y] + v[x-1][y]))pq.push({dp[nx][ny],nx,ny});
                } // U
            }
        }
    }
    pq.push({0,gx,gy});
    while(pq.size()){
        auto [d,x,y] = pq.top();
        pq.pop();
        if(d > rp[x][y])continue;
        for(int k=0;k<4;k++){ // RDLU
            int nx = x + dx[k], ny = y + dy[k];
            if(nx>=0 && nx<n && ny>=0 && ny<n){
                if(k == 0){
                    if(chmin(rp[nx][ny],rp[x][y] + h[x][y]))pq.push({rp[nx][ny],nx,ny});
                }// R
                else if(k == 1){
                    if(chmin(rp[nx][ny],rp[x][y] + v[x][y]))pq.push({rp[nx][ny],nx,ny});
                } // D
                else if(k == 2){
                    if(chmin(rp[nx][ny],rp[x][y] + h[x][y-1]))pq.push({rp[nx][ny],nx,ny});
                } // L
                else {
                    if(chmin(rp[nx][ny],rp[x][y] + v[x-1][y]))pq.push({rp[nx][ny],nx,ny});
                } // U
            }
        }
    }
    queue<array<int,2>> q;
    q.push({sx,sy});
    while(q.size()){
        auto [x,y] = q.front();
        q.pop();
        if(x == gx && y == gy)break;
        for(int k=0;k<4;k++){
            int nx = x + dx[k], ny = y + dy[k];
            int d=0;
            char c = 'a';
            if(k == 0){
                d = h[x][y];
                c = 'R';
            }// R
            else if(k == 1){
                d = v[x][y];
                c = 'D';
            } // D
            else if(k == 2){
                d = h[x][y-1];
                c = 'L';
            } // L
            else {
                d = v[x-1][y];
                c = 'U';
            } // U
            // cerr << dp[gx][gy] << ": " <<  dp[x][y] << " " << rp[nx][ny] << " " << d << endl;
            if(nx>=0 && nx<n && ny>=0 && ny<n && dp[gx][gy] == dp[x][y] + rp[nx][ny] + d){
                q.push({nx,ny});
                ans += c;
                break;
            }
        }
    }
    cout << ans << endl;
    return dp[gx][gy];
}

int line = 120;
void update(int pred, int dist){
    int sum = (dist - pred);
    int mhd = ans.size();
    int dif=0;
    for(int j=0;j<3;j++){
        int x = sx, y = sy;
        dif = sum / mhd;
        if(dif == 0)break;
        for(int i=0;i<mhd;i++){
            if(ans[i] == 'L'){
                if(j == 2 || ch[x][y-1]<line){
                    sum -= dif;
                    h[x][y-1] += dif;
                    ch[x][y-1]++;
                    if(h[x][y-1] < L){
                        sum += L - h[x][y-1];
                        h[x][y-1] = L;
                        if(i!=mhd-1) dif = sum / (mhd-i-1);
                    }
                    if(h[x][y-1] > R){
                        sum += h[x][y-1] - R;
                        h[x][y-1] = R;
                        if(i!=mhd-1) dif = sum / (mhd-i-1);
                    }
                }
                y--;
            }
            else if(ans[i] == 'R'){
                if(j == 2 || ch[x][y]<line){
                    sum -= dif;
                    h[x][y] += dif;
                    ch[x][y]++;
                    if(h[x][y] < L){
                        sum += L - h[x][y];
                        h[x][y] = L;
                        if(i!=mhd-1) dif = sum / (mhd-i-1);
                    }
                    if(h[x][y] > R){
                        sum += h[x][y] - R;
                        h[x][y] = R;
                        if(i!=mhd-1) dif = sum / (mhd-i-1);
                    }
                }
                y++;
            }
            else if(ans[i] == 'U'){
                if(j == 2 || cv[x-1][y] < line){
                    sum -= dif;
                    v[x-1][y] += dif;
                    cv[x-1][y]++;
                    if(v[x-1][y] < L){
                        sum += L - v[x-1][y];
                        v[x-1][y] = L;
                        if(i!=mhd-1) dif = sum / (mhd-i-1);
                    }
                    if(v[x-1][y] > R){
                        sum += v[x-1][y] - R;
                        v[x-1][y] = R;
                        if(i!=mhd-1) dif = sum / (mhd-i-1);
                    }
                }
                x--;
            }
            else if(ans[i] == 'D'){
                if(j == 2 || cv[x][y]<line){
                    sum -= dif;
                    v[x][y] += dif;
                    cv[x][y]++;
                    if(v[x][y] < L){
                        sum += L - v[x][y];
                        v[x][y] = L;
                        if(i!=mhd-1) dif = sum / (mhd-i-1);
                    }
                    if(v[x][y] > R){
                        sum += v[x][y] - R;
                        v[x][y] = R;
                        if(i!=mhd-1) dif = sum / (mhd-i-1);
                    }
                }
                x++;
            }
        }
    }
}

bool randwalk(){
    ans = "";
    bool f = true;
    int x = sx, y = sy;
    if(abs(sx-gx)<3){
        int d = abs(sx-gx);
        while(d--){
            if(f && ch[x][y-(sy>gy)]<line){
                for(int i=0;i<abs(sy-gy);i++){
                    if(sy>gy)ans += 'L';
                    else ans += 'R';
                }
                f = false;
            }
            if(sx>gx)ans += 'U', x--;
            else ans += 'D', x++;
        }
        if(f && ch[x][y-(sy>gy)]<line){
            for(int i=0;i<abs(sy-gy);i++){
                if(sy>gy)ans += 'L';
                else ans += 'R';
            }
            f = false;
        }
    }
    else{ // abs(sy-gy)<3
        int d = abs(sy-gy);
        while(d--){
            if(f && cv[x-(sx>gx)][y]<line){
                for(int i=0;i<abs(sx-gx);i++){
                    if(sx>gx)ans += 'U';
                    else ans += 'D';
                }
                f = false;
            }
            if(sy>gy)ans += 'L', y--;
            else ans += 'R', y++;
        }
        if(f && cv[x-(sx>gx)][y]<line){
            for(int i=0;i<abs(sx-gx);i++){
                if(sx>gx)ans += 'U';
                else ans += 'D';
            }
            f = false;
        }
    }
    if(f == false)cout << ans << endl;
    return f;
}

void randupdate(int dist){
    int mhd = ans.size();
    int dif = dist / mhd;
    int x = sx, y = sy;
    if(abs(sx-gx)<3){
        int xx=-1;
        for(int i=0;i<mhd;i++){
            if(ans[i] == 'L'){
                xx = x;
                int pls = line / (abs(sx-gx) + 1);
                pls /= 2;
                ch[x][y-1]+=pls;
                h[x][y-1] = (h[x][y-1] * (ch[x][y-1]-pls)+ dif*pls) / ch[x][y-1];
                y--;
            }
            else if(ans[i] == 'R'){
                xx = x;
                int pls = line / (abs(sx-gx) + 1);
                pls /= 2;
                ch[x][y]+=pls;
                h[x][y] = (h[x][y] * (ch[x][y]-pls) + dif*pls) / ch[x][y];
                y++;
            }
            else if(ans[i] == 'U'){
                cv[x-1][y]++;
                v[x-1][y] = (v[x-1][y] * (cv[x-1][y]-1)+ dif) / cv[x-1][y];
                x--;
            }
            else if(ans[i] == 'D'){
                cv[x][y]++;
                v[x][y] = (v[x][y] * (cv[x][y]-1)+ dif) / cv[x][y];
                x++;
            }
        }
        for(int i=0;i<n-1;i++){
            int pls = line / (abs(sx-gx) + 1);
            pls /= 2;
            ch[xx][i] += pls;
            h[xx][i] = (h[xx][i] * (ch[xx][i]-pls)+ dif*pls) / ch[xx][i];
        }
    }
    else if(abs(sy-gy)<3){
        int yy = -1;
        for(int i=0;i<mhd;i++){
            if(ans[i] == 'U'){
                yy = y;
                int pls = line / (abs(sy-gy) + 1);
                cv[x-1][y]+=pls;
                v[x-1][y] = (v[x-1][y] * (cv[x-1][y]-pls)+ dif*pls) / cv[x-1][y];
                x--;
            }
            else if(ans[i] == 'D'){
                yy = y;
                int pls = line / (abs(sy-gy) + 1);
                cv[x][y]+=pls;
                v[x][y] = (v[x][y] * (cv[x][y]-pls)+ dif*pls) / cv[x][y];
                x++;
            }
            else if(ans[i] == 'L'){
                ch[x][y-1]++;
                h[x][y-1] = (h[x][y-1] * (ch[x][y-1]-1)+ dif) / ch[x][y-1];
                y--;
            }
            else if(ans[i] == 'R'){
                ch[x][y]++;
                h[x][y] = (h[x][y] * (ch[x][y]-1) + dif) / ch[x][y];
                y++;
            }
        }
        for(int i=0;i<n-1;i++){
            int pls = line / (abs(sy-gy) + 1);
            pls /= 2;
            cv[i][yy] += pls;
            v[i][yy] = (v[i][yy] * (cv[i][yy]-pls)+ dif*pls) / cv[i][yy];
        }
    }
    else{
        for(int i=0;i<mhd;i++){
            if(ans[i] == 'L'){
                ch[x][y-1]++;
                h[x][y-1] = (h[x][y-1] * (ch[x][y-1]-1)+ dif) / ch[x][y-1];
                y--;
            }
            else if(ans[i] == 'R'){
                ch[x][y]++;
                h[x][y] = (h[x][y] * (ch[x][y]-1) + dif) / ch[x][y];
                y++;
            }
            else if(ans[i] == 'U'){
                cv[x-1][y]++;
                v[x-1][y] = (v[x-1][y] * (cv[x-1][y]-1)+ dif) / cv[x-1][y];
                x--;
            }
            else if(ans[i] == 'D'){
                cv[x][y]++;
                v[x][y] = (v[x][y] * (cv[x][y]-1)+ dif) / cv[x][y];
                x++;
            }
        }
    }
}

int iter = 0;
void query(){
    iter++;
    cin>>sx>>sy>>gx>>gy;
    int pred = -1;
    if(iter<350 && (abs(sx-gx)<3 || abs(sy-gy)<3)){
        bool f = randwalk();
        if(f)pred = dijkstra();
        int dist;
        cin>>dist;
        if(f)update(pred,dist);
        else randupdate(dist);
    }
    else{
        pred = dijkstra();
        int dist;
        cin>>dist;
        update(pred,dist);
    }
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    init_weight();
    for(int i=0;i<1000;i++)query();
}
