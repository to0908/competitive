// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
// #pragma GCC optimize("unroll-loops")
#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define endl '\n'
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 back){if(a<=back)return 0; a=back; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 back){if(a>=back)return 0; a=back; return 1;}
int dx[4]={-1,1,0,0}, dy[4]={0,0,-1,1};
long double eps = 1e-9;
long double pi = acos(-1);


class UnionFind{
    public:
    //親の番号を格納する。親だった場合は-(その集合のサイズ)
    vector<int> parent;

    UnionFind(int N){
        parent = vector<int>(N,-1);
    }
    void resize(int N){
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

vector<vector<array<int,2>>> v;
int n,m;
int ma;

bool debug = 1;

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    ifstream cin("F");
    ofstream cout("out.txt");
    while(cin>>n>>m,n){
        v = vector<vector<array<int,2>>>(100010);
        vector<int> ans;
        ma = 0;
        for(int i=0;i<m;i++){
            int a,b,c;
            cin>>a>>b>>c;
            a--,b--,c--;
            chmax(ma,c);
            v[c].push_back({a,b});
        }
        if(m == 0){
            for(int i=1;i<=n;i++){
                cout << i << " ";
            }
            cout << endl;
            continue;
        }
        UnionFind uni(n), kani(n);
        bool rem[n];
        for(int i=0;i<n;i++)rem[i] = 1;

        // 強度[i,ma]
        for(int i=0;i<=ma;i++){
            uni.resize(n);
            kani.resize(n);
            int us=1,ks=1;
            if(v[i].size() == 0)continue;
            for(int t=ma;t>=i;t--){
                for(auto e:v[t]){
                    if(rem[e[0]] == false || rem[e[1]] == false){
                        continue;
                    }
                    // uni -> [i,ma]
                    uni.unite(e[0],e[1]);
                    // kani -> [i+1,ma]
                    if(t!=i)kani.unite(e[0],e[1]);
                    chmax(us,uni.size(e[0]));
                    chmax(ks,kani.size(e[0]));

                }
            }

            if(ks == 1){
                for(int j=0;j<n;j++){
                    if(rem[j])ans.push_back(j);
                }
                break;
            }
            for(int j=0;j<n;j++){
                if(kani.size(j) != ks)rem[j] = false;
            }
        }
        assert(ans.size() != 0);
        for(auto i:ans)cout << i+1 << " ";
        cout << endl;
    }

}
