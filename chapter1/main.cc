#include <iostream>
#include <vector>
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

int main(){
#if 0
    POJ1321();
#endif
    // POJ1321();
    POJ2251();

    return 0;
}