#include <iostream>
#include <vector>
#include <queue>
#include <map>
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


void POJ1426(){
    auto bfs = [](int n){
        queue<int64_t> q;//数值类型为long long类型
        q.push(1);
        while (!q.empty())
        {
            int64_t ls = q.front();
            if(ls%n!=0){ //不是n的倍数将两个子数入队
                q.push(ls*10);
                q.push(ls*10+1);
            } else {
                cout<<ls<<endl;
                return;
            }
            q.pop();
        }
    };

    int64_t n;
    while (cin>>n) {
        if(n==0)break;
        bfs(n);
    }
}

void POJ3126(){
    //说白了，栈空间不一定能过去
    int f[10001];
    int v[10001];
    auto init = [&](){
        //把所有不是素数的置为1
        memset(f,0,sizeof f);
	    for(int i=2;i<=10000;i++) {
	    	if(f[i]==0) {
	    		for(int j = i+i;j<=10000;j+=i) {
	    			f[j] = 1;
	    		}
	    	}
	    }
    };

    //input
    int a,b;
    auto bfs = [&]() {
    	memset(v,0,sizeof v);
        //a入队列
    	queue<int> q;
    	q.push(a);
    	v[a] = 1;
    	int tmp;
    	while(!q.empty())
    	{
    		tmp = q.front();
            q.pop();
    		if(tmp==b)return v[tmp];
    		int now;
    		int t = v[tmp]+1;
    		for(int i=0;i<=9;i++)//循环改变的数字 0 ~ 9
    		{
    			now = tmp - tmp%10+i;//尝试个位0-9
    			if(now>=1000&&f[now]==0&&v[now] == 0){v[now] = t;q.push(now);}
    			now = tmp/100*100+i*10+tmp%10;//尝试十位0-9
    			if(now>=1000&&f[now]==0&&v[now] == 0){v[now] = t;q.push(now);}
    			now = tmp/1000*1000+i*100+tmp%100;//改变百位
    			if(now>=1000&&f[now]==0&&v[now] == 0){v[now] = t;q.push(now);}
    			now = i*1000+tmp%1000;//改变千位
    			if(now>=1000&&f[now]==0&&v[now] == 0){v[now] = t;q.push(now);}
    		}
    	}
    };

    init(); 
    int count;
    cin>>count;
    while(count--) {
    	cin>>a>>b;
    	int ans = bfs();
    	cout<<ans-1<<endl;
    }

}


void POJ3087(){
    string s1,s2,s12;
    int C;
    int N,flag=1;
    struct Node{
        string cur;
        int step;
    };

    auto shuffle = [&](string &s1,string &s2){
        string now;
        for(int i=0;i<C;i++){
            now+=s2[i];
            now+=s1[i];
        }
        return now;
    };

    auto BFS = [&](string s1,string s2,string s12){
        map<string,bool>vis;     //来标记出现过的情况
        queue<Node> q;
        Node now,next;
        now.step=1;
        now.cur=shuffle(s1,s2);
        vis[now.cur]=true;
        q.push(now);
        while(!q.empty()){
            now=q.front();
            q.pop();
            if(now.cur==s12){
                std::cout << now.step << std::endl;
                return;
            }
            s1.clear();s2.clear();   //开始没清空
            for(int i=0;i<C;i++)
                s1+=now.cur[i];
            for(int i=C;i<2*C;i++)
                s2+=now.cur[i];
            next.cur=shuffle(s1,s2);
            if(!vis[next.cur]){
                next.step=now.step+1;
                vis[next.cur]=true;
                q.push(next);
            }
        }
        std::cout << "-1\n";
    };
    
    
    std::cin >> N;
    while(N--){
        cin>>C;
        cin>>s1>>s2>>s12;
        std::cout << flag;
        flag++;
        BFS(s1,s2,s12);
    }
}

void POJ3414(){
    const int N = 110;
    const string op[6] = {"FILL(1)", "FILL(2)", "DROP(1)", "DROP(2)", "POUR(1,2)", "POUR(2,1)"};
    int a, b, c, visit[N][N];// 标记状态(a,b)是否已访问过
    struct state {
        int a, b;
        string op;
        state(int x, int y, string z) : a(x), b(y), op(z) {}
    };

    auto bfs = [&](){
        queue<state> q;
        memset(visit, 0, sizeof visit);
        visit[0][0] = 1;
        q.push(state(0, 0, ""));// 初始状态入队：两个水壶都是空的，无操作
        while(!q.empty()) {
            state temp = q.front();
            q.pop();
            // cout << temp.a << " " << temp.b << endl;
            if(temp.a == c || temp.b == c) {
                int n = temp.op.size(); 
                cout << n << endl;
                for(int i = 0; i < n; i++)
                    cout << op[temp.op[i] - '0'] << endl;
                return ;
            }
            //装满水壶 1（FILL (1)）
            if(temp.a != a && !visit[a][temp.b]++)  q.push(state(a, temp.b, temp.op + '0'));
            //装满水壶 2（FILL (2)）
            if(temp.b != b && !visit[temp.a][b]++)  q.push(state(temp.a, b, temp.op + '1'));
            //清空水壶 1（DROP (1)）
            if(temp.a != 0 && !visit[0][temp.b]++)  q.push(state(0, temp.b, temp.op + '2'));
            //清空水壶 2（DROP (2)）
            if(temp.b != 0 && !visit[temp.a][0]++)  q.push(state(temp.a, 0, temp.op + '3'));
            //从水壶 1 倒入水壶 2（POUR (1,2)）
            if(temp.a != 0) {
                int add = min(temp.a, b - temp.b);
                if(!visit[temp.a - add][temp.b + add]++)
                    q.push(state(temp.a - add, temp.b + add, temp.op + '4'));
            }
            //从水壶 2 倒入水壶 1（POUR (2,1)）
            if(temp.b != 0) {
                int add = min(a - temp.a, temp.b);
                // cout << temp.a + add << " " << temp.b - add << endl;
                if(!visit[temp.a + add][temp.b -add]++)
                    q.push(state(temp.a + add, temp.b - add, temp.op + '5'));
            }
        }
        cout << "impossible" << endl;
    };

    while(cin >> a >> b >> c) {
        bfs();
    }

}

/*
无来源，暂搁置
FZU-2150    Fire Game
UVA-11624   Fire!
POJ-3984    迷宫问题 POJ系统没搜到😮
*/

void HDU1241(){
    #define N 110

    char Map[N][N];
    int n, m, dir[8][2]={{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

    auto dfs = recursive_lambda([&](auto const& self,int x, int y) ->void{
        if(Map[x][y]=='*')
            return ;
        if(x<0 || x>=n || y<0 || y>=m)
            return ;

        Map[x][y] = '*';

        for(int i=0; i<8; i++)
            self(self, x+dir[i][0], y+dir[i][1]);
    });

    while(cin >> n>>m)
    {
        if(m ==0) break;
        int i, j, sum=0;
        for(i=0; i<n; i++)
            cin >> Map[i];
        for(i=0; i<n; i++){
            for(j=0; j<m; j++)
            {
                if(Map[i][j]=='@')
                {
                    dfs(i, j);
                    sum ++;
                }
            }
        }
        std::cout << sum << std::endl;
    }

}

int main(){
#if 0
    POJ1321();
    POJ2251();
    POJ3278();
    POJ3279();
    POJ1426();
    POJ3126();
    POJ3087();
    POJ3414();
#endif
    HDU1241();
    return 0;
}