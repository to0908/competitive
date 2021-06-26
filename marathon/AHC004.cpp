#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")
#include<bits/stdc++.h> 
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0};
long double eps = 1e-9;
long double pi = acos(-1);
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

int n,m;
vector<string> ans(20);
vector<string> s;
vector<string> sini;
vector<int> cnt;
void init(){
	for(int i=0;i<m;i++)cnt.push_back(1);
	s.resize(m);
	sini.resize(m);
	// for(int i=0;i<20;i++){
	// 	for(int j=0;j<20;j++){
	// 		ans[i] += '.';
	// 	}
	// }
}

// 完全に被っているのを削除
void dup(){
	bool e[m]={};
	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++){
			if(i == j)continue;
			if(e[j]) continue;
			for(int l=0;l<(int)s[j].size();l++){
				if(s[i] == s[j].substr(l,(int)s[i].size())){
					e[i] = 1;
					cnt[j]+=cnt[i];
					break;
				}
			}
			if(e[i])break;
		}
	}
	vector<string> tmp;
	vector<int> tmpc;
	for(int i=0;i<m;i++)if(e[i] == 0){
		tmp.push_back(s[i]);
		tmpc.push_back(cnt[i]);
	}
	swap(cnt,tmpc);
	swap(s,tmp);
}

// 部分的にかぶっているのをマージ, cntの計算でどこかバグってます、助けて～～
void marge(){
	int m = s.size();
	int len[m]={};
	int cnt_tmp[m]={};
	array<int,2> root[m]={};
	for(int i=0;i<m;i++){
		len[i] = s[i].size();
		cnt_tmp[i] = cnt[i];
		root[i] = {i,i};
	}
	priority_queue<array<int,3>> pq;
	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++){
			if(i == j)continue;
			for(int k=0;k<(int)s[i].size();k++){
				if(s[i].substr(k) == s[j].substr(0,(int)s[i].size()-k)){
					pq.push({(int)s[i].size()-k,i,j});
					break;
				}
			}
		}
	}
	while(pq.size()){
		auto [sz,i,j] = pq.top(); pq.pop();
		if(j != root[j][0])continue;
		int r = i;
		while(i != root[i][0]){
			i = root[i][0];
		}
		if(r != root[i][1])continue;
		if(len[i] + len[j] - sz > 20){
			int hami = len[i] + len[j] - sz - 20;
			if(s[i].substr(0,hami) == s[j].substr(len[j]-hami)){
				root[i][1] = root[j][1];
				root[j][0] = i;
				len[i] = 20;
				cnt_tmp[i] += cnt_tmp[j];
				cnt_tmp[j] = 0;
				int p = 0;
				while((int)s[i].size()<20)s[i] += s[j][sz+p],p++;
			}
			continue;
		}
		root[i][1] = root[j][1];
		root[j][0] = i;
		len[i] += len[j] - sz;
		cnt_tmp[i] += cnt_tmp[j];
		cnt_tmp[j] = 0;
		s[i] += s[j].substr(sz);
	}
	vector<string> tmp;
	cnt.clear();
	for(int i=0;i<m;i++){
		if(i == root[i][0]){
			cnt.push_back(cnt_tmp[i]);
			tmp.push_back(s[i]);
		}
	}
	swap(s,tmp);
}

int scoring(vector<string> &ans){
	int cnt = 0;
	int m = sini.size();
	bool used[m]={};
	for(int i=0;i<m;i++){
		for(int x=0;x<n+13;x++){
			string tate = "", yoko = "";
			for(int y=0;y<n+13;y++){
				yoko += ans[x%20][y%20];
				if(yoko.size()>sini[i].size() && yoko.substr(y-(int)sini[i].size(),(int)sini[i].size()) == sini[i]){
					used[i] = 1;
					break;
				}
				tate += ans[y%20][x%20];
				if(tate.size()>sini[i].size() && tate.substr(tate.size()-sini[i].size(),sini[i].size()) == sini[i]){
					used[i] = 1;
					break;
				}
			}
			if(used[i])break;
		}
	}
	for(int i=0;i<m;i++)cnt+=used[i];
	return cnt;
}


void solve(){
	int m = s.size(); 
	array<int,2> id[m];
	for(int i=0;i<m;i++)id[i] = {cnt[i],i};
	sort(id,id+m,greater<array<int,2>>());
	int h = 0;
	bool used[m]={};
	for(int ix=0;ix<m;ix++){
		int i = id[ix][1];
		if(ans[h].size() + s[i].size()>20)h++;
		if(h==20)break;
		ans[h] += s[i];
		used[i] = 1;
		if(ix+1!=(int)s.size() && ans[h].size() + s[id[ix+1][1]].size() <= 20){
			ans[h] += s[id[ix+1][1]];
			ix++;
			used[id[ix][1]]=1;
		}
	}
	sort(all(ans),[](const string& lh, const string& rh){return rh.size() > lh.size();});
	bool canShift[n]={};
	for(int i=0;i<20;i++){
		if(ans[i].size() == 20)canShift[i] = 1;
		while(ans[i].size()<20)ans[i]+='.';
	}
	if(n < (int)s.size()){
		for(int ix=0;ix<m;ix++){
			int i = id[ix][1];
			if(used[i])continue;
			int r = 0;
			while(r<20){
				bool ok = 1;
				for(int j=0;j<(int)s[i].size();j++){
					if(ans[j][r]!='.')ok=false;
				}
				if(ok){
					for(int j=0;j<(int)s[i].size();j++){
						ans[j][r] = s[i][j];
					}
					break;
				}
				else{
					r++;
				}
			}
		}
	}
	int stscore = scoring(ans);
	// シフトできるところ頑張る、あとswapも
	vector<int> shift;
	for(int i=0;i<n;i++)if(canShift[i])shift.push_back(i);
	int curscore = stscore;
	int bestscore = curscore;
	vector<string> cur(20);
	for(int i=0;i<n;i++){
		cur[i] = ans[i];
	}
	Timer time;
	m = shift.size();
	int R = 1000;
	int end = 27;
	end = 2750;
	while(time.elapsed() < end){
		for(int _=0;_<10;_++){
			int t = randint() & 1;
			// swap
			if(t){
				int l = randint()%m, r = randint()%m;
				while(r==l)r=randint()%m;
				swap(cur[shift[l]],cur[shift[r]]);
				int score = scoring(cur);
				const bool force_next = R * (end - time.elapsed()) > end*(randint()%R);
				if(chmax(bestscore,score)){
					for(int i=0;i<n;i++)ans[i] = cur[i];
				}
				else if(chmax(curscore,score) || force_next){
					curscore = score;
				}
				else swap(cur[shift[l]],cur[shift[r]]);
			}
			// shift
			else{
				int p = randint() % m;
				int k = randint() % 10;
				// cur[shift[p]]をkだけ右にshiftさせる
				string tmp = cur[shift[p]];
				for(int i=0;i<20;i++)cur[shift[p]][i] = tmp[(i+k)%20];
				int score = scoring(cur);
				const bool force_next = R * (end - time.elapsed()) > end*(randint()%R);
				if(chmax(bestscore,score)){
					for(int i=0;i<n;i++)ans[i] = cur[i];
				}
				else if(chmax(curscore,score) || force_next){
					curscore = score;
				}
				else cur[shift[p]] = tmp;
			}
		}
	}
	cerr << "YAKINAMASHI: " << scoring(ans) - stscore << endl;
}

// 隙間があったら埋めます。焼きなまし、めんどくさそうなので、これで妥協(え～～)
void umeru(){
	int m = sini.size();
	bool used[m]={};
	for(int i=0;i<m;i++){
		for(int x=0;x<n+13;x++){
			string tate = "", yoko = "";
			for(int y=0;y<n+13;y++){
				yoko += ans[x%20][y%20];
				if(yoko.size()>sini[i].size() && yoko.substr(y-(int)sini[i].size(),(int)sini[i].size()) == sini[i]){
					used[i] = 1;
					break;
				}
				tate += ans[y%20][x%20];
				if(tate.size()>sini[i].size() && tate.substr(tate.size()-sini[i].size(),sini[i].size()) == sini[i]){
					used[i] = 1;
					break;
				}
			}
			if(used[i])break;
		}
	}
	s.clear();
	for(int i=0;i<m;i++)if(!used[i])s.push_back(sini[i]);
	if(s.size()) sort(all(s),[](const string& lh, const string& rh){return rh.size() > lh.size();});
	m = s.size();
	for(int i=0;i<m;i++){
		bool pos = false;
		for(int x=0;x<n;x++){
			if(pos)break;
			for(int y=0;y<n;y++){
				if(pos)break;
				if(ans[x][y] == '.'){
					bool ok = true;
					for(int k=y;k<y+(int)s[i].size();k++){
						if(ans[x][k%20] != '.' && ans[x][k%20] != s[i][k-y]){
							ok=false;
							break;
						}
					}
					if(ok){
						for(int k=y;k<y+(int)s[i].size();k++){
							ans[x][k%20] = s[i][k-y];
						}
						pos = 1;
						continue;
					}
					ok = true;
					for(int k=x;k<(int)s[i].size()+x;k++){
						if(ans[k%20][y] != '.' && ans[k%20][y] != s[i][k-x]){
							ok=false;
							break;
						}
					}
					if(ok){
						for(int k=x;k<(int)s[i].size()+x;k++){
							ans[k%20][y] = s[i][k-x];
						}
						pos = 1;
						continue;
					}
				}
			}
		}
	}
}

void debug(int x){
	cerr << x << " " << s.size() << endl;
	m = s.size();
	vector<int> len(m);
	for(int i=0;i<m;i++)len[i] = s[i].size();
	sort(all(len));
	int sum = 0;
	for(int i=0;i<m;i++){
		// cerr << len[i] << " " << cnt[i] << endl;
		sum += cnt[i];
	}
	cerr << sum - x << endl;
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    cin>>n>>m;
	int x = m;
	init();
	for(int i=0;i<m;i++){
		cin>>s[i];
		sini[i] = s[i];
	}
	dup();
	marge();
	solve();
	debug(x);
	int score = scoring(ans);
	umeru();
	score = scoring(ans) - score;
	cerr << "SCORE: " << score << endl;
	for(int i=0;i<n;i++){
		if(score!=x){
			for(int j=0;j<n;j++){
				if(ans[i][j] == '.'){
					ans[i][j] = 'A' + randint()%8;
				}
			}
		}
		cout << ans[i] << "\n";
	}

	
}
