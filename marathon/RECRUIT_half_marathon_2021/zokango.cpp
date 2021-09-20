#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define all(x) (x).begin(),(x).end()
template<typename T1,typename T2> bool chmin(T1 &a,T2 b){if(a<=b)return 0; a=b; return 1;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){if(a>=b)return 0; a=b; return 1;}
const int dx[] = {-1,1,0,0}, dy[] = {0,0,-1,1};
const int ddx[] = {0,0,1,1}, ddy[] = {1,1,0,0};

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

///////////////// param
const int n = 16, m = 5000, t = 1000;
const int not_connected_ok = 5;
const int broad = 30; // 1日にbroad個まで見る
const int buy_day = 850; // day < buy_day -> always buy
const double ignore_value = 2.5; // V <= pow(2,S/100 - ignore_value) は無視
/////////////////

int veg_furute[t][256];
vector<array<int,3>> veg_start[t]; // [start_day]{h, w, e, value}
vector<int> veg_end[t]; // [end_day]{h, w}
vector<vector<int>> ans; 
int turn = 0;
int price[256], price_sum[257];

array<int,2> coord[256];

struct State{
    int money=1; // そのターン開始時に持っているお金
    bool connected = false;
    vector<int> machine;
    bitset<256> is_machine;
    bitset<256> croped; // その場所の野菜が収穫されたか。trueで収穫できない。新しい野菜が来たらfalseにする
    int from=0; // 遷移前の状態のindexをもっておく。使用したStateの操作(ope)は日にちごとにvectorに突っ込んでおく
    int ope=0; // 前回の操作のハッシュ。buyは-1 * tohash(x,y) -1, moveは tohash(from)*256 + tohash(to)
    int bonus = 0; // 最後の操作についてボーナスを加える
    State(){};
    bool operator<(const State s) const{
        return money + price_sum[(int)machine.size()] + bonus
                < s.money + price_sum[(int)s.machine.size()] + s.bonus;
    }

    void good_morning(){
        bonus = 0;
        for(auto [ha,e,va]:veg_start[turn]){
            croped[ha] = 0;
        }
    }
};

array<int,2> tocoord(int x){
    return coord[x];
}

int tohash(int x, int y){
    return x * n + y;
}

vector<int> hash_to_ope(int hash){
    if(hash < 0){
        hash = -hash - 1;
        return {coord[hash][0], coord[hash][1]};
    }
    if(hash > 1e6){
        return {-1};
    }
    array<int,2> to = tocoord(hash%256);
    hash >>= 8;
    array<int,2> from = tocoord(hash);
    return {from[0],from[1],to[0],to[1]};
}

int buy_hash(int hash){
    return -hash-1;
}

int move_hash(int from_hash, int to_hash){
    return from_hash * 256 + to_hash;
}

int value[256];
int End[256];

void good_morning(){
    for(auto [ha,e,v]:veg_start[turn]){
        value[ha] = v;
        End[ha] = e;
    }
}

void good_night(){
    for(int ha:veg_end[turn]){
        value[ha] = 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////////

void init(){
    int x = 0;
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            coord[x] = {i,j};
            if(x){
                price[x-1] = x*x*x;
                price_sum[x] += price[x-1];
            }
            ++x;
        }
    }
}

priority_queue<State> pq, pre;
int opes[t+1][broad+1]; // 日付ごとにbeamで見た操作の保持。
int froms[t+1][broad+1]; // 日付事に、前の日のどこから来たか
int used[256];
int called_turn_end = 0;

bool crop_check(State &S, int &pos, int &get){
    if(!S.croped[pos] && value[pos]){
        get += value[pos];
        S.croped[pos] = 1;
        return true;
    }
    return false;
}

// O(machine), not connectedは定数倍重め
// Sの状態からその日の終わりの処理(収穫)を行う
int turn_end(State &S){

    if(S.connected){
        int get = 0, cnt = S.machine.size(), ret = 0;
        vector<int> crop;
        for(auto ha:S.machine){
            if(crop_check(S,ha,get)){
                crop.emplace_back(ha);
            }
        }
        ret = get * cnt;
        S.money += ret;
        pq.push(S);
        S.money -= ret;
        for(auto ha:crop) S.croped[ha] = 0;
        return ret;
    }

    ++called_turn_end;
    int ret = 0;
    bool con_tmp = S.connected;
    vector<int> crop;
    queue<int> q;
    for(int &pos_hash:S.machine){
        if(used[pos_hash] == called_turn_end) continue;
        int cnt = 0, get = 0;
        q.push(pos_hash);
        used[pos_hash] = called_turn_end;
        while(q.size()){
            int ha = q.front();
            q.pop();
            auto [x,y] = tocoord(ha);
            cnt++;
            if(crop_check(S,ha,get)){
                crop.emplace_back(ha);
            }
            if(x && S.is_machine[ha-16] && used[ha-16] != called_turn_end){
                used[ha-16] = called_turn_end;
                q.push(ha-16);
            }
            if(x<n-1 && S.is_machine[ha+16] && used[ha+16] != called_turn_end){
                used[ha+16] = called_turn_end;
                q.push(ha+16);
            }
            if(y && S.is_machine[ha-1] && used[ha-1] != called_turn_end){
                used[ha-1] = called_turn_end;
                q.push(ha-1);
            }
            if(y<n-1 && S.is_machine[ha+1] && used[ha+1] != called_turn_end){
                used[ha+1] = called_turn_end;
                q.push(ha+1);
            }
        }
        ret += get * cnt;
        if(cnt == (int)S.machine.size()){
            S.connected = 1;
            break;
        }
        else{
            S.connected = 0;
        }
    }
    S.money += ret;
    pq.push(S);
    S.money -= ret;
    for(auto ha:crop) S.croped[ha] = 0;
    S.connected = con_tmp;
    return ret;
}

int Score_cnt[256], Score_par[256], Score_get[256];
int Score_value[256]; // そのターンでcropされたかどうか
int Score_score_sum, Score_cnt_min, Score_cnt_min_par;
int Score_get_sum; // cnt倍してない値(get sum)
void score_calc(State &S, bool crop=true){
    if(S.connected){
        int score = 0;
        if(crop){
            for(auto ha:S.machine){
                if(crop_check(S,ha,score)){
                    Score_value[ha] = value[ha];
                }
                else Score_value[ha] = 0;
            }
        }
        for(auto ha:S.machine){
            Score_cnt[ha] = (int)S.machine.size();
            Score_get[ha] = score; 
            Score_par[ha] = 0;
        }
        Score_get_sum = score;
        Score_score_sum = score * (int)S.machine.size();
        return;
    }
    // else if(crop) return;

    ++called_turn_end;
    int ret = 0, p = 0;
    queue<int> q;
    Score_cnt_min = 1000;
    for(int &pos_hash:S.machine){
        if(used[pos_hash] == called_turn_end) continue;
        int c = 0, g = 0;
        q.push(pos_hash);
        used[pos_hash] = called_turn_end;
        vector<int> chi;
        while(q.size()){
            int ha = q.front();
            q.pop();
            auto [x,y] = tocoord(ha);
            ++c;
            chi.emplace_back(ha);
            if(crop){
                crop_check(S,ha,g);
            }
            if(x && S.is_machine[ha-16] && used[ha-16] != called_turn_end){
                used[ha-16] = called_turn_end;
                q.push(ha-16);
            }
            if(x<n-1 && S.is_machine[ha+16] && used[ha+16] != called_turn_end){
                used[ha+16] = called_turn_end;
                q.push(ha+16);
            }
            if(y && S.is_machine[ha-1] && used[ha-1] != called_turn_end){
                used[ha-1] = called_turn_end;
                q.push(ha-1);
            }
            if(y<n-1 && S.is_machine[ha+1] && used[ha+1] != called_turn_end){
                used[ha+1] = called_turn_end;
                q.push(ha+1);
            }
        }
        ret += g * c;
        if(chmin(Score_cnt_min, c)){
            Score_cnt_min_par = p;
        }
        for(auto ch:chi){
            Score_cnt[ch] = c;
            Score_get[ch] = g;
            Score_par[ch] = p;
        }
        ++p;
    }
    Score_score_sum = ret;
    return;
}

void buy_connected(State &S, int pos){
    S.machine.back() = pos;
    S.is_machine[pos] = 1;
    S.ope = buy_hash(pos);
    bool crp = S.croped[pos];
    int g = Score_get_sum;
    crop_check(S,pos,g);
    g *= (int)S.machine.size();

    S.money += g;
    pq.push(S);
    S.money -= g;
    S.is_machine[pos] = 0;
    S.croped[pos] = crp;
}

// TODO: bugfix  <- 多分バグってません、多分
void buy_not_connected(State &S, int &pos){
    S.machine.back() = pos;
    S.ope = buy_hash(pos);
    S.is_machine[pos] = 1;
    int cnt = 1, get = 0, mi = 0;
    int tmp = S.croped[pos];
    bool connectmp = S.connected;
    crop_check(S,pos,get);
    auto [x,y] = coord[pos];
    bool root[(int)S.machine.size()] = {};
    for(int i=0;i<4;i++){
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(nx < 0 || ny < 0 || nx>=n || ny>=n) continue;
        int ha = tohash(nx,ny);
        if(S.is_machine[ha] && !root[Score_par[ha]]){
            root[Score_par[ha]] = 1;
            cnt += Score_cnt[ha];
            get += Score_get[ha];
            mi += Score_cnt[ha] * Score_get[ha];
        }
    }
    get = cnt * get - mi + Score_score_sum;
    S.money += get;
    if(S.is_machine.size() == cnt){
        S.connected = 1;
    }
    else S.connected = 0;

    pq.push(S);

    S.is_machine[pos] = 0;
    S.money -= get;
    S.croped[pos] = tmp;
    S.connected = connectmp;
}


void buy(State &S){
    score_calc(S);
    S.money -= price[(int)S.machine.size()];
    S.machine.emplace_back(-1);
    if((int)S.machine.size() < not_connected_ok or S.connected==false){
        for(int pos=0;pos<256;++pos){
            if(S.is_machine[pos]) continue;
            buy_not_connected(S, pos);
        }
    }
    else{
        bitset<256> visited;
        for(int j=0;j<(int)S.machine.size()-1;++j){
            int &ma2 = S.machine[j];
            auto [x,y] = tocoord(ma2);
            if(x && S.is_machine[ma2-16] == 0 && !visited[ma2-16]){
                buy_connected(S,ma2-16);
                visited[ma2-16] = 1;
            }
            if(x<n-1 && S.is_machine[ma2+16] == 0 && !visited[ma2+16]){
                buy_connected(S,ma2+16);
                visited[ma2+16] = 1;
            }
            if(y && S.is_machine[ma2-1] == 0 && !visited[ma2-1]){
                buy_connected(S,ma2-1);
                visited[ma2-1] = 1;
            }
            if(y<n-1 && S.is_machine[ma2+1] == 0 && !visited[ma2+1]){
                buy_connected(S,ma2+1);
                visited[ma2+1] = 1;
            }
        }
    }
}

vector<int> pred_value(256), emp_vec(256);
int dx1[] = {-1,0,0,1}, dy1[] = {0,-1,1,0};
int dx2[] = {-2,-1,-1,0,0,1,1,2}, dy2[] = {0,-1,1,-2,2,-1,1,0};
int dx3[] = {-3,-2,-2,-1,-1,0,0,1,1,2,2,3};
int dy3[] = {0,-1,1,-2,2,-3,3,-2,2,-1,1,0};
const int dep = 3;
void _pred_val(State &S, int i, int j){
    int pos = tohash(i,j);
    pred_value[pos] = 0;
    if(S.is_machine[pos])return;
    for(int d=0;d<4;d++){
        int nx = i + dx1[d];
        int ny = j + dy1[d];
        if(nx<0 or ny<0 or nx>=n or ny>=n)continue;
        int npos = tohash(nx,ny);
        if(S.is_machine[npos])continue;
        // for(int dp=1;dp<=dep && turn+dp<t;dp++){
        //     if(veg_furute[turn+dp][npos]) pred_value[pos] += veg_furute[turn+dp][npos] / (dp+1);
        // }
        if(!value[npos] or S.croped[npos] or End[npos]<turn+1) continue;
        pred_value[pos] += value[npos];
    }
    for(int d=0;d<8;d++){
        int nx = i + dx2[d];
        int ny = j + dy2[d];
        if(nx<0 or ny<0 or nx>=n or ny>=n)continue;
        int npos = tohash(nx,ny);
        if(S.is_machine[npos]) continue;
        // for(int dp=2;dp<=dep && turn+dp<t;dp++){
        //     if(veg_furute[turn+dp][npos]) pred_value[pos] += veg_furute[turn+dp][npos] / (dp);
        // }
        if(!value[npos] or S.croped[npos] or End[npos]<turn+2) continue;
        pred_value[pos] += value[npos] * 0.5;
    }
    for(int d=0;d<12;d++){
        int nx = i + dx3[d];
        int ny = j + dy3[d];
        if(nx<0 or ny<0 or nx>=n or ny>=n)continue;
        int npos = tohash(nx,ny);
        if(S.is_machine[npos]) continue;
        // for(int dp=3;dp<=dep && turn+dp<t;dp++){
        //     if(veg_furute[turn+dp][npos]) pred_value[pos] += veg_furute[turn+dp][npos] / (dp);
        // }
        if(!value[npos] or S.croped[npos] or S.is_machine[npos] or End[npos]<turn+3) continue;
        pred_value[pos] += value[npos] * 0.33;
    }
}

void predict_value(State &S, bool must_connected){

    if(S.connected && must_connected){
        // 連結回りだけでOK
        ++called_turn_end;
        pred_value = emp_vec;
        for(int ha:S.machine){
            auto [x,y] = tocoord(ha);
            if(x && used[ha-16] != called_turn_end){
                used[ha-16] = called_turn_end;
                _pred_val(S,x-1,y);
            }
            if(x<n-1 && used[ha+16] != called_turn_end){
                used[ha+16] = called_turn_end;
                _pred_val(S,x+1,y);
            }
            if(y && used[ha-1] != called_turn_end){
                used[ha-1] = called_turn_end;
                _pred_val(S,x,y-1);
            }
            if(y<n-1 && used[ha+1] != called_turn_end){
                used[ha+1] = called_turn_end;
                _pred_val(S,x,y+1);
            }
        }
        return;
    }

    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            _pred_val(S,i,j);
        }
    }
}

int dd[] = {-16,16,-1,1};
void move_operate(State &S, int &ma2, int &ma, int &tmp, bitset<256> &visited, int d){
    ma = ma2 + dd[d];
    S.is_machine[ma] = 1;
    S.ope = move_hash(tmp, ma);
    int bonus = pred_value[ma];
    S.bonus += bonus;
    turn_end(S);
    S.bonus -= bonus;
    S.is_machine[ma] = 0;
    visited[ma] = 1;
}

void move_operate_connected(State &S, int &ma2, int &ma, int &tmp, bitset<256> &visited, int d){
    // tmp -> ma2 + dd[d]
    ma = ma2 + dd[d];
    S.is_machine[ma] = 1;
    S.ope = move_hash(tmp, ma);
    // int bonus = bon[turn][ma2][d] + pred_value[ma];
    int bonus = pred_value[ma];
    S.bonus += bonus;
    int get = Score_score_sum;
    get -= Score_value[tmp] * (int)S.machine.size();
    int crp_tmp = S.croped[ma];
    int g=0;
    if(crop_check(S,ma,g)){
        get += g * (int)S.machine.size();
    }
    S.money += get;
    pq.push(S);
    S.money -= get;
    S.bonus -= bonus;
    S.is_machine[ma] = 0;
    S.croped[ma] = crp_tmp;
    visited[ma] = 1;
}

void move_4(State &S, int &ma2, int &ma, int &tmp, bitset<256> &visited, bool connected=false){
    auto [x,y] = tocoord(ma2);
    if(x && S.is_machine[ma2-16] == 0 && ma2-16!=tmp && !visited[ma2-16]){
        if(!connected) move_operate(S,ma2,ma,tmp,visited, 0);
        else move_operate_connected(S,ma2,ma,tmp,visited, 0);
    }
    if(x<n-1 && S.is_machine[ma2+16] == 0 && ma2+16!=tmp && !visited[ma2+16]){
        if(!connected) move_operate(S,ma2,ma,tmp,visited, 1);
        else move_operate_connected(S,ma2,ma,tmp,visited, 1);
    }
    if(y && S.is_machine[ma2-1] == 0 && ma2-1!=tmp && !visited[ma2-1]){
        if(!connected) move_operate(S,ma2,ma,tmp,visited, 2);
        else move_operate_connected(S,ma2,ma,tmp,visited, 2);
    }
    if(y<n-1 && S.is_machine[ma2+1] == 0 && ma2+1!=tmp && !visited[ma2+1]){
        if(!connected) move_operate(S,ma2,ma,tmp,visited, 3);
        else move_operate_connected(S,ma2,ma,tmp,visited, 3);
    }
}

void move(State &S){
    predict_value(S, (int)S.machine.size() >= not_connected_ok);
    if(!S.connected && (int)S.machine.size() >= not_connected_ok){
        score_calc(S, false); // 実装を分ける？
    }
    else if(S.connected && (int)S.machine.size() >= not_connected_ok){
        score_calc(S);
    }
    for(int i=0;i<(int)S.machine.size();++i){
        int &ma = S.machine[i];
        int tmp = S.machine[i];
        S.is_machine[tmp] = 0;
        if((int)S.machine.size() < not_connected_ok){
            for(int ha=0;ha<256;++ha){
                if(ha == tmp) continue;
                if(S.is_machine[ha]) continue;
                S.is_machine[ha] = 1;
                ma = ha;
                S.ope = move_hash(tmp, ha);
                S.connected = false;
                int tmpb = S.bonus;
                int can_get = 0;
                // bonus 分けた方がいいかも
                if(S.machine.size() > 1){
                    auto [x,y] = coord[ha];
                    int ma = 0, mi = 1e9;
                    for(int d=0;d<4;d++){
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if(nx < 0 or ny < 0 or nx>=n or ny>=n)continue;
                        int npos = tohash(nx,ny);
                        if(End[npos]>turn && value[npos] && !S.croped[npos] && !S.is_machine[npos]){
                            S.bonus += value[npos];
                            chmax(ma,value[npos]);
                            chmin(mi,value[npos]); 
                        }
                        if(S.is_machine[npos] && value[npos]) can_get += value[npos];
                        else if(S.is_machine[npos])can_get++;
                    }
                    if(mi > 1e6)mi = 0;
                    S.bonus += ma - mi;
                    S.bonus /= 2;
                    S.bonus += can_get;
                }
                if(!S.croped[ha] && value[ha] + S.money >= price[S.machine.size()])S.bonus += 5;
                turn_end(S);
                S.bonus = tmpb;
                S.is_machine[ha] = 0;
            }
        }
        else if(!S.connected){
            if(Score_cnt_min_par != Score_par[tmp]){
                S.is_machine[tmp] = 1;
                ma = tmp;
                continue;
            }
            bitset<256> visited;
            for(int j=0;j<(int)S.machine.size();++j){
                if(i == j)continue;
                move_4(S, S.machine[j], ma, tmp, visited);
            }
        }
        else{
            int adj = 0;
            auto [x,y] = tocoord(tmp);
            if(x && S.is_machine[tmp-16])adj++;
            if(x<n-1 && S.is_machine[tmp+16])adj++;
            if(y && S.is_machine[tmp-1])adj++;
            if(y<n-1 && S.is_machine[tmp+1])adj++;
            bool keep = (adj==1);
            if(adj>1){
                keep = 1;
                if(x && S.is_machine[tmp-16]){
                    if(y && S.is_machine[tmp-1] && !S.is_machine[tmp-17]) keep = 0;
                    if(y<n-1 && S.is_machine[tmp+1] && !S.is_machine[tmp-15]) keep = 0;
                }
                else if(x<n-1 && S.is_machine[tmp+16]){
                    if(y && S.is_machine[tmp-1] && !S.is_machine[tmp+15]) keep = 0;
                    if(y<n-1 && S.is_machine[tmp+1] && !S.is_machine[tmp+17]) keep = 0;
                }
                if(keep){
                    if(x && S.is_machine[tmp-16] && x<n-1 && S.is_machine[tmp+16]){
                        if(adj == 2)keep=0;
                        else if(y && S.is_machine[tmp-1] && S.is_machine[tmp+15] && S.is_machine[tmp-17]);
                        else if(y<n-1 && S.is_machine[tmp+1] && S.is_machine[tmp+17] && S.is_machine[tmp-15]);
                        else keep = 0;
                    }
                    if(y && S.is_machine[tmp-1] && y<n-1 && S.is_machine[tmp+1]){
                        if(adj == 2) keep = 0;
                        if(x && S.is_machine[tmp-16] && S.is_machine[tmp-16-1] && S.is_machine[tmp-16+1]);
                        if(x<n-1 && S.is_machine[tmp+16] && S.is_machine[tmp+16-1] && S.is_machine[tmp+16+1]);
                        else keep = 0;
                    }
                }
                
            }
            if(keep){
                bitset<256> visited;
                for(int j=0;j<(int)S.machine.size();++j){
                    if(i == j)continue;
                    move_4(S, S.machine[j], ma, tmp, visited, true);
                }
            }
        }
        S.is_machine[tmp] = 1;
        ma = tmp;
    }
}

priority_queue<State> emp_que;
int INF = (1<<24);

void action(){
    int bro = 0;
    while(pre.size() && bro < broad){
        State S = pre.top();
        pre.pop();
        S.good_morning();
        froms[turn][bro] = S.from;
        S.from = bro;
        opes[turn][bro] = S.ope;

        if(turn < buy_day && S.money >= price[(int)S.machine.size()]){
            buy(S);
        }
        else {
            move(S);
        }
        bro++;
    }
}


signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);

    Timer timer;

    int a; cin>>a>>a>>a;
    a = 0;
    for(int i=0;i<m;++i){
        int R,C,S,E,V;
        cin>>R>>C>>S>>E>>V;
        if(V > pow(2,(double)S/100-ignore_value)){
            ++a;
            int ha = tohash(R,C);
            veg_start[S].push_back({ha,E,V});
            veg_end[E].emplace_back(ha);
            veg_furute[S][ha] = V;
        }
        // veg_start[S].push_back({tohash(R,C),E,V});
        // veg_end[E].emplace_back(tohash(R,C));
    }
    init();
    pq.push(State{});
    while(turn != t){
        good_morning();
        pre = emp_que;
        swap(pq,pre);
        action();
        good_night();
        turn++;
    }

    // ここで答えの操作列の復元を行う
    State S = pq.top();
    int from = S.from;
    int ope = S.ope;
    // cerr << "money: " << S.money << endl;
    while(turn > 0){
        ans.emplace_back(hash_to_ope(ope));
        turn--;
        ope = opes[turn][from];
        from = froms[turn][from];
    }
    reverse(all(ans));

    for(auto vec:ans){
        for(int i=0;i<(int)vec.size();++i){
            if(i)cout << " ";
            cout << vec[i];
        }
        cout << "\n";
    }
    
    cerr << "time: " << timer.elapsed() << endl;
}
