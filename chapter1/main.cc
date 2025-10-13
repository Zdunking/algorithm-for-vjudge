#include <iostream>
#include <vector>
#include <queue>
using namespace std;

//using for lambda trap in recursive
auto recursive_lambda = [](auto &&lam) {
     return [lam_impl = std::forward<decltype(lam)>(lam)](auto &&...args) -> decltype(auto) {
         return lam_impl(lam_impl, std::forward<decltype(args)>(args)...);
     };
 };

void POJ1321(){
    char mp[8][8];
    int record[8];//列是否访问过的记录
    int n,k;
    int sum=0;
    std::vector<int> ans;
    auto backtrack = recursive_lambda([&](auto const &self,int x,int cnt)->void{
        if(cnt==k) {
            sum++;
        }
        for(int i=x; i<n; i++) {
            for(int j=0; j<n; j++) {
                if(mp[i][j]=='#'&&record[j]==0) {//判断这一列没有访问
                    record[j]=1;
                    self(self,i+1,cnt+1);
                    record[j]=0;
                }
            }
        }
    });
    while(cin >> n >>k) {
        if(n==-1&&k==-1)
            break;
        else {
            sum=0;
            memset(record,0,sizeof(record));
            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++)
                    cin >> mp[i][j];
            backtrack(0,0);
        }
        ans.push_back(sum);
    }

    for(auto i : ans){
        std::cout << i << ' ';
    }
}


void POJ2251(){
    const int maxn = 30000;
    // 定义三维坐标点结构
    struct Point {
        int x, y, z;// 分别代表层、行、列
    }q[maxn]; // BFS队列

    int len[maxn]; // 存储到每个位置的距离

    // 6个方向的偏移量（三维空间中的6个相邻方向）
    int xx[6] = {-1 ,1  ,0  ,0  ,0  ,0};  // x方向（层）的变化
    int yy[6] = {0  ,0  ,-1 ,1  ,0  ,0};  // y方向（行）的变化
    int zz[6] = {0  ,0  ,0  ,0  ,-1 ,1};  // z方向（列）的变化
    bool vis[35][35][35]; // 标记是否访问过该位置
    char map[35][35][35];
    int l,r,c,sx,sy,sz;// l:层数, r:行数, c:列数; sx,sy,sz:起点坐标  由用户输入

    auto bfs = [&]()->int{
        int rear, front, dx, dy, dz;
        memset(vis, 0, sizeof(vis));
        memset(len, 0, sizeof(len));
        // 起点入队列
        q[0].x=sx, q[0].y=sy, q[0].z=sz;
        //队列头尾指针 TODO 考虑使用标准库
        front = rear = 0;

        // 当队列不为空时继续搜索
        while(front <= rear)
        {
             // 探索当前位置的6个方向
            for(int i=0; i<6; i++)
            {
                // 到达新位置，计算新位置坐标
                dx = q[front].x + xx[i];
                dy = q[front].y + yy[i];
                dz = q[front].z + zz[i];
                /*
                判断该空间可以访问的条件需满足一下三项
                1.这块空间没有访问过
                2.空间没有岩石或者直接是出口
                3.并且没有越界
                */
                if(!vis[dx][dy][dz] && (map[dx][dy][dz]=='.'||map[dx][dy][dz]=='E')
                   && dx>=0 && dx<l && dy>=0 && dy<r && dz>=0 && dz<c)
                   {
                     vis[dx][dy][dz] = 1;
                     rear++;
                     q[rear].x = dx;
                     q[rear].y = dy;
                     q[rear].z = dz;
                     len[rear] = len[front]+1;

                     //由于是广度优先，那么当搜到路径的时候，一定是最短的
                     if(map[dx][dy][dz] == 'E')
                        return len[rear];
                   }
            }
            front++;
        }
        return 0;
    };

    while(cin>>l>>r>>c) {
        if(l==0 && r==0 && c==0)
           break;
        for(int i=0; i<l; i++) {
            for(int j=0; j<r; j++) {
                for(int k=0; k<c; k++) {
                    cin>>map[i][j][k];
                    if(map[i][j][k] == 'S')
                       sx=i, sy=j, sz=k;
                }
            }
        }
        int ret = bfs();
        if(ret)
           printf("Escaped in %d minute(s).\n",ret);
        else
           puts("Trapped!");
    }
}

void POJ3278(){
    typedef pair<int, int> PII;
    const int N = 1e5 + 10;
    int visit[N], n, k;
    auto bfs = [&]() {
        queue<PII> q;
        q.push(make_pair(n, 0));
        visit[n] = 1;
        while(!q.empty()) {
            PII temp = q.front();
            q.pop();
            if(temp.first == k) {
                printf("%d\n", temp.second);
                break;
            }
            if(temp.first - 1 >= 0 && !visit[temp.first - 1]) {//坐标减一
                visit[temp.first - 1] = 1;
                q.push(make_pair(temp.first - 1, temp.second + 1));
            }
            if(temp.first  + 1 <= N && !visit[temp.first + 1]) {//坐标加一
                visit[temp.first + 1] = 1;
                q.push(make_pair(temp.first + 1, temp.second + 1));
            }
            if(temp.first * 2 <= N && !visit[temp.first * 2]) {//坐标乘二
                visit[temp.first * 2] = 1;
                q.push(make_pair(temp.first * 2, temp.second + 1));
            }
        }
    };

    while(cin >> n >> k) {
        memset(visit, 0, sizeof visit);
        bfs();
    }
}

void POJ3279(){//注意细节啊
    const int MAX_N = 15;
    const int MAX_M = 15;

    int n, m;
    int grid[MAX_N][MAX_M];  // 原始网格
    int flip[MAX_N][MAX_M];  // 当前翻转方案
    int best_flip[MAX_N][MAX_M];  // 最优翻转方案
    int min_flips = INT_MAX;  // 最少翻转次数

    auto get_color = [&](int x, int y, int current_flip[MAX_N][MAX_M]){
        int color = grid[x][y];
        // 加上自身和上下左右的翻转次数
        color += current_flip[x][y];
        if (x > 0) color += current_flip[x-1][y];
        if (x < n-1) color += current_flip[x+1][y];
        if (y > 0) color += current_flip[x][y-1];
        if (y < m-1) color += current_flip[x][y+1];
        return color % 2;  // 奇数次翻转改变状态，偶数次不变
    };

    auto fliptitle = [&](){
        //通过第一行的所有可能翻转状态（2^m种）去枚举，因为第一行没有依据，只能通过下一行去修正
        //变量state是一个二进制数，用于表示第一行的翻转方案。例如，当m=3时，state=5（二进制101）表示第一行第 0 列和第 2 列需要翻转。
        for (int state = 0; state < (1 << m); state++) {
            memset(flip, 0, sizeof(flip));
            int total = 0;

            // 根据state设置第一行的翻转方案
            for (int j = 0; j < m; j++) {
                if (state & (1 << j)) {
                    flip[0][j] = 1;
                    total++;
                }
            }

            // 处理第1行到第n-2行，确定每行的翻转方案
            for (int i = 0; i < n-1; i++) {
                for (int j = 0; j < m; j++) {
                    // 如果当前位置的颜色是1，需要翻转下一行对应位置来修正
                    if (get_color(i, j, flip) == 1) {
                        flip[i+1][j] = 1;
                        total++;
                    }
                }
            }

            // 检查最后一行是否全为0
            bool valid = true;
            for (int j = 0; j < m; j++) {
                if (get_color(n-1, j, flip) == 1) {
                    valid = false;
                    break;
                }
            }

            // 如果有效且翻转次数更少，更新最优解
            if (valid && total < min_flips) {
                min_flips = total;
                memcpy(best_flip, flip, sizeof(flip));
            }
        }

        // 输出结果
        if (min_flips == INT_MAX) {
            cout << "IMPOSSIBLE" << endl;
        } else {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (j > 0) cout << " ";
                    cout << best_flip[i][j];
                }
                cout << endl;
            }
        }
    };

    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }
    fliptitle();
}

int main(){
#if 0
    POJ1321();
    POJ2251();
    POJ3278();
#endif
    // POJ1321();
    // POJ2251();
    // POJ3278();
    POJ3279();
    return 0;
}