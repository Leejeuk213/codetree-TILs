#include<iostream>
#include<map>
#include<vector>
using namespace std;
int maze[11][11];
int n,m,z;
map<int,pair<int,int>> runner;
map<int,int> runner_state; // 1이면 퇴장
map<int,int> runner_score; // 1이면 퇴장

int dx[4]={1,-1,0,0};
int dy[4]={0,0,-1,1};

int e_x,e_y;

int result = 0;

int is_over(){

    for(int i = 1; i<=m;i++){
        if(runner_state[i] == 0)return 0;
    }

    return 1;
}

int distance(int x2, int y2){
    int x = x2 - e_x;
    int y = y2 - e_y;
    if(x < 0) x *= -1;
    if(y < 0) y *= -1;
    int dis = x+y;
    return dis;
}
// 브루트포스로 하자 정사각형 하나씩 만들어오면서 있는지 체크 있으면 바로 그걸로 ㄱ
// 맵은 10x 10이라 불가능한 시간은 아니다.

vector<int> is_here(int x1, int x2, int y1, int y2){

    int is_runner = 0;
    int is_end = 0;

    vector<int> v;
    for(int i = x1; i<=x2; i++){
        for(int j = y1; j<=y2; j++){

            if(i == e_x && j == e_y) is_end = 1;

            for(int k=1;k<=m;k++){
                if(runner[k].first == i && runner[k].second == j){
                    is_runner = 1;
                    v.push_back(k);
                } 
            }

            if(is_runner && is_end) return v;
        }
    }

    vector<int> trash;
    return trash;
}
void maze_rotate(){
    int is_find = 0;
    int f_x = 0;
    int f_y = 0;
    int r_x = 0;
    int r_y = 0;
    int size = 0;
    vector<int> v;
    for(int k= 2; k<=n;k++){
        for(int i = 1; i <=n-k+1; i++){
            for(int j=1; j<=n-k+1; j++){

                v = is_here(i,i+k-1,j,j+k-1);
                if(!v.empty()){
                    size = k;
                    is_find = 1;
                    f_x = i;
                    r_x = i+k-1;
                    f_y = j;
                    r_y = j+k-1;
                    break;
                }
            }
            if(is_find) break;
        }
        if(is_find) break;
    }

    // cout << '\n';
    // cout << size << ' ' << f_x << ' ' << f_y << ' ' << r_x << ' ' << r_y << '\n';
    int temp[11][11] = {0,};
    int end_check = 1;
    int checked[m+1]={0,};

    int x = f_x;
    for(int i =r_y; i>= f_y; i--){
        int y = f_y;
        for(int j= f_x; j<= r_x; j++){

            // 0 and 출구 and 사람 있니
            if(maze[x][y] == 0){
                temp[j][i] = maze[x][y];
                //출구 변경
                if(x == e_x && y == e_y && end_check){
                    e_x = j;
                    e_y = i;
                    end_check = 0;
                }

                for(int k = 0; k<v.size();k++){
                    if(runner[v[k]].first == x && runner[v[k]].second == y && checked[v[k]] == 0){
                        checked[v[k]] = 1;
                        runner[v[k]].first = j;
                        runner[v[k]].second = i;
                    }
                }

            }
            else{
                temp[j][i] = maze[x][y] -1; 
            }
            y++;
        }
        x++;
    }
    
    // 변경 적용
    for(int i =f_x; i<= r_x; i++){
        for(int j= f_y; j<= r_y; j++){
            maze[i][j] = temp[i][j];
        }
    }
    return; 
}

void move(int index){

    if(runner_state[index] == 1) return;

    int now_dis = distance(runner[index].first,runner[index].second);
    int min_dis = 987654321;
    int v = -1 ;
    for(int i = 0; i < 4; i++){

        int x = runner[index].first + dx[i];
        int y = runner[index].second + dy[i];

        if(x < 1  || x > n || y < 1 || y > n) continue;
        if(maze[x][y] !=0) continue;
        if(x == e_x && y == e_y){
            runner_state[index] = 1;
            runner_score[index] ++ ;
            runner[index].first = 0;
            runner[index].second = 0;
            return;
        }

        int dis = distance(x,y);

        if(dis >= min_dis || dis >= now_dis) continue;

        else{
            v = i;
            min_dis = dis;
        }
    }
    if(v != -1){
        runner[index].first += dx[v];
        runner[index].second += dy[v];
        runner_score[index]++;
    }
    return ;
}
int main(){
    
    cin >> n >> m >>z;
    for(int i=1; i<=n;i++){
        for(int j=1; j<=n; j++){
            cin >> maze[i][j];
        }
    }

    for(int i=1; i<=m; i++){
        int x, y;
        cin >> x>> y;
        runner[i].first =x;
        runner[i].second =y;
        runner_state[i] = 0;
        runner_score[i] = 0;
    }

    cin >> e_x >> e_y;

    for(int i = 1; i<=z; i++){

        for(int j= 1; j<=m; j++){
            move(j);
        }

        // cout << '\n';
        // for(int j= 1; j<=m; j++){
        //     cout <<j << ':' <<runner[j].first << ' ' <<runner[j].second <<'\n';
        // }
        if(is_over()) break;
        maze_rotate();

        // cout << '\n';
        // cout<<'e' << e_x << ' ' << e_y << '\n';
        // for(int j= 1; j<=m; j++){
        //     cout<<j << ':' << runner[j].first << ' ' <<runner[j].second <<'\n';
        // }
        // cout << '\n';
        // for(int k = 1; k<=n;k++){
        //     for(int s = 1; s<=n; s++){
        //         cout << maze[k][s] << ' ';
        //     }
        //     cout << '\n';
        // }
    }

    for(int i = 1; i<=m;i++){
        result += runner_score[i] ;
    }

    cout << result << '\n';
    cout <<e_x << ' ' << e_y;
}