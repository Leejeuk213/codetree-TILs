#include<iostream>
#include<map>
using namespace std ; 

map <int, pair<int,int> > santa;
map <int, pair<int,int> > santa_state;

// 0 : default 1 : stun 2: retire

int santa_score[31] = {0,};

int n,m,c,p,d;

int R_r, R_c ;

int r_dx[8] = {-1,0,1,0,-1,-1,1,1};
int r_dy[8] = {0,1,0,-1,-1,1,-1,1};

// 상우하좌 우선순위
int santa_dx[4] = {-1,0,1,0};
int santa_dy[4] = {0,1,0,-1};

int distance(int x1, int y1, int x2 , int y2){

    int result = 0;

    int x_dis = x1 - x2 ;

    int y_dis = y1 - y2;

    result = (x_dis * x_dis) + (y_dis * y_dis);

    return result;
}

int interact(int x, int y, int cur, int vector){

    int is_interact = 0;
    for(int i= 1; i <= p; i++){

        if(i == cur || santa_state[i].first == 2) continue;
        if(santa[i].first == x && santa[i].second == y ){

            is_interact = 1;
            x = santa[i].first + r_dx[vector];
            y = santa[i].second + r_dy[vector];

            if( x <= 0 || x > n || y <= 0 || y > n ){

                santa_state[i].first = 2; 
                return 1;
            }
            santa[i].first = x;
            santa[i].second = y;
            interact(x,y,i,vector);
        }

        if(is_interact) return 1;
    }

    return 0 ;
}

void colision(int power, int vector){

    int target_santa = 0;
    int is_colision = 0;
    // 탐지
    for(int i = 1; i<= p; i++){

        if(santa[i].first == R_r && santa[i].second == R_c && santa_state[i].first != 2) {
            is_colision = 1;
            santa_score[i] += power;
            target_santa = i;
            break;
        }
    }

    if(is_colision){
        santa_state[target_santa].first = 1;
        santa_state[target_santa].second = 0;

        int x = santa[target_santa].first + power * r_dx[vector]; 
        int y = santa[target_santa].second +  power * r_dy[vector]; 
        if( x <= 0 || x > n || y <= 0 || y > n ){
            
            santa_state[target_santa].first = 2; 
            return;
        }
        santa[target_santa].first = x;
        santa[target_santa].second = y; 
        interact(x,y,target_santa,vector);
    
    }
    return;
}

void ru_move(){

    int target_santa = 0;
    int min_dis = 987654321 ;
    // 가장 가까운 산타 결정
    for(int i=1; i <= p ; i ++){
        if(santa_state[i].first == 2) continue;
        int dis = distance(santa[i].first,santa[i].second,R_r,R_c);
        if(dis < min_dis){
            min_dis = dis;
            target_santa = i;
        }
        else if(dis == min_dis){

            if(santa[target_santa].first > santa[i].first) target_santa = target_santa;
            else if(santa[target_santa].first < santa[i].first) target_santa = i;   
            else if(santa[target_santa].first == santa[i].first) {
                if(santa[target_santa].second > santa[i].second) target_santa = target_santa;
                else if(santa[target_santa].second < santa[i].second) target_santa = i;
            }
        }
    }

    // 산타를 향해 돌진

    int target_distance = 0;
    min_dis = 987654321;
    int vector = 0; 
    for(int i=0; i< 8; i++){

        int x = R_r + r_dx[i];
        int y = R_c + r_dy[i];

        if( x <= 0 || x > n || y <= 0 || y > n ) continue;

        int dis = distance(santa[target_santa].first,santa[target_santa].second,x,y);

        if(dis < min_dis) { 
            target_distance = i;
            min_dis = dis;
            vector = i;
        }
    }
    R_r += r_dx[target_distance];
    R_c += r_dy[target_distance];
    // 충돌
    
    colision(c,vector);


    return ;
}

int is_santa_here(int x, int y, int index){
    for (int i=1; i<=p; i++){
        if(i == index || santa_state[i].first == 2) continue;
        if(santa[i].first == x && santa[i].second == y) return 1;
    }
    return 0 ;
}

void santa_move(){

    for(int i= 1; i<= p ; i++){

        if(santa_state[i].first != 0) continue;

        int vector = 0;
        int now_distance = distance(santa[i].first,santa[i].second,R_r,R_c);
        int min_dis = 987654321;
        int is_can_move = 0;
        for(int j=0; j<4; j++){
            
            int x = santa[i].first + santa_dx[j];
            int y = santa[i].second + santa_dy[j];

            if( x <= 0 || x > n || y <= 0 || y > n  || is_santa_here(x,y,i) ) continue;

            int dis = distance(x,y,R_r,R_c);

            if(dis < now_distance && dis < min_dis){
                is_can_move = 1;
                vector = j;
                min_dis = dis;
            }
        }
        if(is_can_move){
            santa[i].first += santa_dx[vector];
            santa[i].second += santa_dy[vector];
        }
        // 충돌
        colision(d,(vector+2)%4);
    }

    return ;
}


int main(){

    cin >> n >> m >> p>> c >> d ; 

    cin >> R_r >> R_c ;

    for(int i = 1 ; i <= p; i++ ){
        int santa_n , santa_r, santa_c ;
        cin >> santa_n >> santa_r >> santa_c ;
        santa[santa_n].first = santa_r;
        santa[santa_n].second = santa_c;
    }


    for(int i= 0; i<m; i++ ){

        int is_finished = 1;
        for(int j = 1; j<=p; j++){
            if(santa_state[j].first == 1){
                santa_state[j].second ++;
                if(santa_state[j].second >= 2){
                    santa_state[j].first = 0;
                    santa_state[j].second = 0;
                }
            }
            if(santa_state[j].first != 2){
                is_finished = 0;
            }
        }

        if(is_finished) break;
        ru_move();

        santa_move();

        for(int j= 1; j<=p; j++){
            if(santa_state[j].first != 2){
                santa_score[j] ++;
            }
        }

        // cout << '\n';

        // cout << R_r <<' '<< R_c << '\n';

        // for(int j= 1; j<= p; j++){

        // cout << santa[j].first << ' ' << santa[j].second << '\n';
        // }

        // for(int j= 1; j<= p; j++){

        // cout << santa_score[j] << ' ';
        // }
        // cout << '\n';
    }

    for(int i= 1; i<= p; i++){

        cout << santa_score[i] << ' ';
    }

    return 0 ;
}