#include<iostream>
#include<vector>
using namespace std;


int board[11][11];
int att[11][11];
int visited[11][11] = {0,};
int n,m,z;

int a_x,a_y;
int d_x,d_y;

int dx[4] ={0,1,0,-1};
int dy[4] ={1,0,-1,0};

int bdx[8] ={0,1,0,-1,1,1,-1,-1};
int bdy[8] ={1,0,-1,0,1,-1,1,-1};

int is_find = 0;
int is_attacked[11][11] ={0,};
vector<pair<int,int>>last_v;
int min_route = 987654321;

void re(){

    for(int i = 1; i<=n;i++){
        for(int j=1; j<=m;j++){
            if(is_attacked[i][j] == 0 && board[i][j] > 0) board[i][j] ++;
        }
    }

}

void init(){
    last_v.clear();
    min_route = 987654321;
    is_find = 0;
    for(int i = 1; i<=n;i++){
        for(int j=1; j<=m;j++){
            visited[i][j] = 0;
            is_attacked[i][j] = 0;
        }
    }
}

void attack(int x, int y, vector<pair<int,int>> v){

    if(visited[x][y] == 1) return;
    if(board[x][y] <= 0) return ;

    if(x == d_x && y == d_y){
        is_find = 1;
        if(min_route > v.size()){
            min_route = v.size();
            last_v = v;
        }
        return;
    }

    visited[x][y] = 1;

    for(int i = 0 ; i< 4; i++){

        int m_x = x + dx[i];
        int m_y = y + dy[i];

        if(m_x < 1) m_x = n;
        if(m_x > n) m_x = 1;

        if(m_y < 1) m_y = m;
        if(m_y > m) m_y = 1;

        if(board[m_x][m_y] <= 0) continue;
        vector<pair<int,int>> v2 = v;

        v2.push_back(make_pair(m_x,m_y));

        attack(m_x,m_y,v2);
    }
    visited[x][y] = 0;

    return;
}

void select_att(){

    vector<pair<int,int> > v1;

    int min = 987654321;
    // 1 공격력 검증
    for(int i= 1; i<=n;i++){
        for(int j=1;j<=m;j++){
            
            if(board[i][j] <= 0) continue;

            if(board[i][j] < min) {
                v1.clear();
                v1.push_back(make_pair(i,j));
                min = board[i][j];
            }
            else if (board[i][j] == min){
                v1.push_back(make_pair(i,j));
            }
        }  
    }
    if(v1.size() == 1) {
        a_x = v1[0].first;
        a_y = v1[0].second;
        return;
    }

    // 2 공격한 시점 검증
    vector<pair<int,int> > v2;
    int max = -1;
    for(int k = 0; k<v1.size();k++){
    
        int i = v1[k].first;
        int j = v1[k].second;
        if(att[i][j] > max) {
                v2.clear();
                v2.push_back(make_pair(i,j));
                max = att[i][j];
            }
        else if (att[i][j] == max){
            v2.push_back(make_pair(i,j));
        }
    }

    if(v2.size() == 1) {
        a_x = v2[0].first;
        a_y = v2[0].second;
        return;
    }

    // 3 행 + 열 검증
    vector<pair<int,int> > v3;
    max = -1;
    for(int k = 0; k<v2.size();k++){
    
        int i = v2[k].first;
        int j = v2[k].second;
        if(i + j> max) {
                v3.clear();
                v3.push_back(make_pair(i,j));
                max = i+j;
            }
        else if (i+j == max){
            v3.push_back(make_pair(i,j));
        }
    }
    if(v3.size() == 1) {
        a_x = v3[0].first;
        a_y = v3[0].second;
        return;
    }

    // 3 열  검증
    max = -1;
    for(int k = 0; k<v3.size();k++){
        int i = v3[k].first;
        int j = v3[k].second;
        if(j> max) {
            max = j;
            a_x = i;
            a_y = j;
        }
    }
    return;
}

void select_def(){

    vector<pair<int,int> > v1;

    int max = 0;
    // 1 공격력 검증
    for(int i= 1; i<=n;i++){
        for(int j=1;j<=m;j++){
            
            if(board[i][j] <= 0) continue;

            if(board[i][j] > max) {
                v1.clear();
                v1.push_back(make_pair(i,j));
                max = board[i][j];
            }
            else if (board[i][j] == max){
                v1.push_back(make_pair(i,j));
            }
        }  
    }
    if(v1.size() == 1) {
        d_x = v1[0].first;
        d_y = v1[0].second;
        return;
    }

    // 2 공격한 시점 검증
    vector<pair<int,int> > v2;
    int min = 987654321;
    for(int k = 0; k<v1.size();k++){
    
        int i = v1[k].first;
        int j = v1[k].second;
        if(att[i][j] < min) {
                v2.clear();
                v2.push_back(make_pair(i,j));
                min = att[i][j];
            }
        else if (att[i][j] == min){
            v2.push_back(make_pair(i,j));
        }
    }

    if(v2.size() == 1) {
        d_x = v2[0].first;
        d_y = v2[0].second;
        return;
    }

    // 3 행 + 열 검증
    vector<pair<int,int> > v3;
    min = 987654321;
    for(int k = 0; k<v2.size();k++){
    
        int i = v2[k].first;
        int j = v2[k].second;
        if(i + j < min) {
                v3.clear();
                v3.push_back(make_pair(i,j));
                min = i+j;
            }
        else if (i+j == min){
            v3.push_back(make_pair(i,j));
        }
    }
    if(v3.size() == 1) {
        d_x = v3[0].first;
        d_y = v3[0].second;
        return;
    }

    // 3 열  검증
    min = 987654321;
    for(int k = 0; k<v3.size();k++){
        int i = v3[k].first;
        int j = v3[k].second;
        if(j < min) {
            min = j;
            d_x = i;
            d_y = j;
        }
    }
    return;
}

void bomb(){

    board[d_x][d_y] -= board[a_x][a_y];
    for(int i = 0; i< 8; i++){

        int m_x = d_x + bdx[i];
        int m_y = d_y + bdy[i];

        if(m_x < 1) m_x = n;
        if(m_x > n) m_x = 1;

        if(m_y < 1) m_y = m;
        if(m_y > m) m_y = 1;
        board[m_x][m_y] -= board[a_x][a_y]/2;
        is_attacked[m_x][m_y] = 1;
    }

    is_attacked[a_x][a_y] = 1;
    is_attacked[d_x][d_y] = 1;

    return ;
}

int main(){

    // vector<int> v;

    cin >> n >> m >> z ;

    for(int i = 1; i<=n;i++){
        for(int j= 1; j<=m;j++){
            cin >> board[i][j];
            att[i][j] = 0;
        }
    }

    for(int i = 1; i<=z; i++){
        
        init();
        select_att();
        select_def();

        board[a_x][a_y] +=n+m;
        att[a_x][a_y] = i;
        vector<pair<int,int>>v;
        attack(a_x,a_y,v);

        if(is_find == 0) bomb();
        else{
            board[d_x][d_y] -= (board[a_x][a_y]-board[a_x][a_y] / 2 );
            is_attacked[a_x][a_y] = 1;
            is_attacked[d_x][d_y] = 1;
            for(int i = 0 ; i<last_v.size();i++){
                // cout << last_v[i].first << ' '  << last_v[i].second << '\n';
                board[last_v[i].first][last_v[i].second] -= board[a_x][a_y] / 2;
                is_attacked[last_v[i].first][last_v[i].second] = 1;
            }
        }
        re();

        // cout << '\n';
        // for(int j = 1; j<=n;j++){
        //     for(int k = 1; k<=m;k++){
        //         cout << board[j][k] << ' ';
        //     }
        //     cout << '\n';
        // }

        // cout << '\n';
        // for(int j = 1; j<=n;j++){
        //     for(int k = 1; k<=m;k++){
        //         cout << is_attacked[j][k] << ' ';
        //     }
        //     cout << '\n';
        // }

    }

    int result = 0;
    
    for(int i = 1; i<=n;i++){
        for(int j= 1; j<=m;j++){
            if(result < board[i][j]) result = board[i][j];
        }
    }

    cout << result;
    return 0;
}