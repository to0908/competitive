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
double bestscore = 1e18;
const ll n = 300, m = 1000, k = 100000000;
ll cur[300];
int a[300];
vector<array<ll,2>> ans;

ll score(){
    double sum = 0;
    for(int i=0;i<n;i++)sum += log2(cur[i] + 1);
    return sum;
}

void init(){
    for(ll mu=3000;mu<=1000000;mu+=3000){
        for(int i=0;i<n;i++)cur[i] = a[i];
        ll rem = m;
        vector<array<ll,2>> tt;
        while(true){
            priority_queue<array<ll,2>> pq;
            for(int i=0;i<n;i++){
                pq.push({cur[i],i});
            }
            vector<array<ll,2>> v;
            array<ll,2> now = pq.top();
            pq.pop();
            array<ll,2> pre = now;
            while(pq.size()){
                if(now[0] + now[0] < k){
                    now[0] += now[0];
                    pre = now;
                    v.push_back({now[1],now[1]});
                    continue;
                }
                if(now[0] + pq.top()[0] >= k){
                    pre = pq.top();
                    pq.pop();
                    continue;
                }
                if(now[0] + pq.top()[0] * 3LL < k)break;
                if(pq.top()[0] < mu)break;
                now[0] += pq.top()[0];
                v.push_back({now[1], pq.top()[1]});
            }
            if(now[0] + pre[0] >= k){
                v.push_back({now[1], pre[1]});
            }
            if(rem >= (int)v.size()){
                for(auto x:v){
                    tt.push_back(x);
                    cur[x[0]] += cur[x[1]];
                }
                if(cur[now[1]] >= k)cur[now[1]] -= k;
                rem -= (int)v.size();
            }
            else break;
        }
        if(chmin(bestscore, score())){
            ans = tt;
        }
    }
}


signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    cin>>a[0]>>a[0]>>a[0];
    for(int i=0;i<n;i++)cin>>a[i];

    init();
    for(int i=0;i<(int)ans.size();i++){
        cout << ans[i][0] << " " << ans[i][1] << "\n";
    }
}
