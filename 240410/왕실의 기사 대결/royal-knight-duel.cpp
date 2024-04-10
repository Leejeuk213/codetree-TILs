#include<iostream>
#include<map>
#include<queue>
#include<vector>
using namespace std;

int board[41][41] ;
int l,n,q;

int sum_ok = 1;

map <int , pair<int,int>> knight;
map <int , pair<int,int>> knight_land;
map <int , int> knight_state;
map <int , int> knight_hp;
map <int , int> knight_score;
map <int , int> knight_pushed;

int dx[4]={-1,0,1,0};
int dy[4]={0,1,0,-1};

void to_sum(int i){

    int f_x = knight[i].first;
    int f_y = knight[i].second;
    int r_x = knight[i].first + knight_land[i].first - 1;
    int r_y = knight[i].second + knight_land[i].second - 1;
    int sum = 0;
    for(int j=f_x; j<=r_x;j++){
        for(int k=f_y;k<=r_y;k++){
            if(board[j][k] == 1) sum++;
        }
    }
    knight_score[i] += sum;
    knight_hp[i] -= sum;
    if(knight_hp[i] <=0) knight_state[i] = 1;
    return ;
}
vector<int> is_knight(int x1, int y1, int x2, int y2){

    vector<int> qe;
    for(int i=1; i<=n;i++){
        if(knight_state[i] != 0) continue;
        int f_x = knight[i].first;
        int f_y = knight[i].second;
        int r_x = knight[i].first + knight_land[i].first - 1;
        int r_y = knight[i].second + knight_land[i].second - 1;
        int ok = 0;
        for(int j=x1; j<=x2;j++){
            for(int k=y1;k<=y2;k++){
                if( (j >= f_x &&  j<= r_x) && (k >= f_y && k <= r_y) ) {
                    ok = 1;
                    break;
                }
            }
            if(ok){
                qe.push_back(i);
                break;
            }
        }
    }
    return qe;
}

int is_wall(int x1, int y1, int x2, int y2){

    for(int i=x1;i<=x2;i++){
        for(int j=y1;j<=y2;j++){
            if(board[i][j] == 2) return 1;
        }
    }
    return 0;
}

int knight_mv(int target_knight, int v){

    if(knight_state[target_knight] != 0) return 0;

    int f_x = knight[target_knight].first;
    int f_y = knight[target_knight].second;
    int r_x = knight[target_knight].first + knight_land[target_knight].first  - 1;
    int r_y = knight[target_knight].second + knight_land[target_knight].second - 1; 

    int mv_f_x = f_x + dx[v];
    int mv_f_y = f_y + dy[v];
    int mv_r_x = r_x + dx[v];
    int mv_r_y = r_y + dy[v];

    // 이동하는게 벽을 넘는지
    if(mv_f_x > l || mv_f_x <=0 || mv_f_y > l || mv_f_y <=0 || mv_r_x > l || mv_r_x <=0 || mv_r_y > l || mv_r_y <=0){
        sum_ok = 0;
        return 1;
    }
    if(is_wall(mv_f_x,mv_f_y,mv_r_x,mv_r_y)){
        sum_ok = 0;
        return 1;
    }

    // 이동하는데 상대 기사가 있는지
    knight_state[target_knight] = 2;
    vector<int> qe = is_knight(mv_f_x,mv_f_y,mv_r_x,mv_r_y);
    if(!qe.empty()){
        for(int i = 0; i<qe.size();i++){
            //cout << qe[i] <<' ';
            knight_pushed[qe[i]] = 1;
            if(knight_mv(qe[i],v)) return 1;
        }
        //cout<<'\n';
    }
    
    // 없다면 해당 기사 움직임 적용시키고 리턴
    else{
        knight[target_knight].first = mv_f_x;
        knight[target_knight].second = mv_f_y;
        return 0;
    }
    if(sum_ok){
        knight[target_knight].first = mv_f_x;
        knight[target_knight].second = mv_f_y;
        return 0;
    }
    return 0;
}

int main(){

    cin >> l >> n >> q;

    for(int i= 1; i<=l;i++){

        for(int j = 1; j<=l;j++){
            cin >> board[i][j];
        }
    }


    for(int i= 1 ; i<= n; i++){
        int r , c , h ,w ,k;
        cin >> r >> c >> h >> w >>k;
        knight[i].first = r; 
        knight[i].second = c; 

        knight_land[i].first = h;
        knight_land[i].second = w;

        knight_hp[i] = k;

        knight_score[i] = 0;
        knight_state[i] = 0;
        knight_pushed[i] = 0;
    }

    for(int i= 0; i < q; i++){
        sum_ok = 1;
        int target_knight, v ;
        cin >> target_knight >> v;
        knight_mv(target_knight,v);

        if(sum_ok){
            for(int j=1;j<=n;j++){
                if(knight_pushed[j]!=0){
                    knight_pushed[j] = 0;
                    to_sum(j);
                }
                if(knight_state[j]== 2) knight_state[j] =0;
            }
        }
        else{
            for(int j=1;j<=n;j++){
                if(knight_pushed[j]!=0){
                    knight_pushed[j] = 0;
                }
                if(knight_state[j]== 2) knight_state[j] =0;
            }
        }

        // cout << '\n';
        // for(int j=1;j<=n;j++){
        //     cout << knight[j].first <<' ' << knight[j].second << '\n'; 
        //     cout << knight_hp[j] << ' ' << knight_score[j] << '\n';
        // }
        // cout<< '\n';
    }

    int cnt = 0;
    for(int i = 1; i <= n; i++){

        if(knight_state[i] != 0) continue;
        //cout << knight_score[i] <<'\n';
        cnt += knight_score[i];
    }

    cout << cnt;
    return 0;
}