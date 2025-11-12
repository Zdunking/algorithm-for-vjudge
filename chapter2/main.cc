#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <numeric>
#include <cstring>
#include <algorithm>
using namespace std;

auto recursive_lambda = [](auto &&lam) {
     return [lam_impl = std::forward<decltype(lam)>(lam)](auto &&...args) -> decltype(auto) {
         return lam_impl(lam_impl, std::forward<decltype(args)>(args)...);
     };
 };

void HDU1043(){
	// 目标状态
	const string target = "12345678x";

	// 计算逆序数判断是否可解
	auto countInversions = [&](const string &s) {
	    int inv_count = 0;
	    int n = s.size();
	    for (int i = 0; i < n; ++i) {
	        if (s[i] == 'x') continue;
	        for (int j = i + 1; j < n; ++j) {
	            if (s[j] == 'x') continue;
	            if (s[i] > s[j]) {
	                inv_count++;
	            }
	        }
	    }
	    return inv_count;
	};

	// BFS求解并返回路径
	auto solvePuzzle = [&](const string &initial)->string {
	    if (initial == target) return "";
	
	    // 检查是否可解
	    if (countInversions(initial) % 2 != 0) {
	        return "unsolvable";
	    }
	
	    // 方向数组：上、下、左、右 (对应移动方向的反方向，因为是x在移动)
	    int dx[] = {-1, 1, 0, 0};
	    int dy[] = {0, 0, -1, 1};
	    char dir[] = {'u', 'd', 'l', 'r'};
	
	    // BFS队列
	    queue<string> q;
	    // 记录访问过的状态及对应的前驱状态和移动方向
	    unordered_map<string, pair<string, char>> parent;
	
	    q.push(initial);
	    parent[initial] = make_pair("", ' ');  // 初始状态无前驱
	
	    while (!q.empty()) {
	        string current = q.front();
	        q.pop();
		
	        // 找到x的位置
	        int pos = current.find('x');
	        int x = pos / 3;  // 行
	        int y = pos % 3;  // 列
		
	        // 尝试四个方向移动
	        for (int i = 0; i < 4; ++i) {
	            int nx = x + dx[i];
	            int ny = y + dy[i];
			
	            // 检查新位置是否合法
	            if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
	                int npos = nx * 3 + ny;  // 新位置的索引
				
	                // 生成新状态
	                string next = current;
	                swap(next[pos], next[npos]);
				
	                // 如果找到目标状态，回溯路径
	                if (next == target) {
	                    string path;
	                    path += dir[i];
	                    string cur = current;
	                    while (parent[cur].first != "") {
	                        path += parent[cur].second;
	                        cur = parent[cur].first;
	                    }
	                    reverse(path.begin(), path.end());
	                    return path;
	                }
				
	                // 如果是未访问的状态，加入队列
	                if (parent.find(next) == parent.end()) {
	                    parent[next] = make_pair(current, dir[i]);
	                    q.push(next);
	                }
	            }
	        }
	    }
	
	    // 理论上不会到达这里，因为可解性已经判断过
	    return "unsolvable";
	};


	string line, initial;
    // 读取输入并处理（过滤空格）
    while (getline(cin, line)) {
        initial.clear();
        for (char c : line) {
            if (c != ' ') {
                initial += c;
            }
        }
        cout << solvePuzzle(initial) << endl;
    }

}


void HDU3567(){
	// 操作按字典序从小到大排列：d < l < r < u（根据ASCII码）
	// 这样BFS时优先尝试字典序小的操作，确保找到的最短路径字典序最小
	vector<pair<char, int>> dirs = {
	    {'d', 3},   // 下移：X与下方棋子交换（索引+3）
	    {'l', -1},  // 左移：X与左侧棋子交换（索引-1）
	    {'r', 1},   // 右移：X与右侧棋子交换（索引+1）
	    {'u', -3}   // 上移：X与上方棋子交换（索引-3）
	};

// 检查操作是否合法
	auto is_valid = [](int pos, char op) {
	    if (op == 'l') {
	        return pos % 3 != 0;  // 不在第一列（列索引0）才能左移
	    } else if (op == 'r') {
	        return pos % 3 != 2;  // 不在第三列（列索引2）才能右移
	    } else if (op == 'u') {
	        return pos / 3 != 0;  // 不在第一行（行索引0）才能上移
	    } else if (op == 'd') {
	        return pos / 3 != 2;  // 不在第三行（行索引2）才能下移
	    }
	    return false;
	};

	auto solve = [&](string start, string target)->pair<int, string> {
	    if (start == target) {
	        return {0, ""};
	    }

	    queue<string> q;
	    unordered_map<string, string> visited;  // 记录状态对应的路径

	    q.push(start);
	    visited[start] = "";

	    while (!q.empty()) {
	        string current = q.front();
	        q.pop();
	        string path = visited[current];

	        // 找到当前状态中X的位置
	        int pos = current.find('X');

	        // 按字典序尝试所有可能的操作
	        for (auto &dir : dirs) {
	            char op = dir.first;
	            int delta = dir.second;

	            if (!is_valid(pos, op)) {
	                continue;  // 跳过不合法操作
	            }

	            // 生成新状态（交换X和目标位置的字符）
	            string next_state = current;
	            swap(next_state[pos], next_state[pos + delta]);

	            // 找到目标状态，返回结果
	            if (next_state == target) {
	                return {(int)path.size() + 1, path + op};
	            }

	            // 新状态未访问过，加入队列并记录路径
	            if (visited.find(next_state) == visited.end()) {
	                visited[next_state] = path + op;
	                q.push(next_state);
	            }
	        }
	    }

	    // 题目保证有解，此处不会执行
	    return {0, ""};
	};


	int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        string A, B;
        cin >> A >> B;
        auto result = solve(A, B);
        cout << "Case " << t << ": " << result.first << endl;
        cout << result.second << endl;
    }
}

void HDU2181(){
	int link[21][21];
	int visit[21];
	int a[21];
	int cnt=1;

	auto DFS = recursive_lambda([&](auto const &self, int n,int country,int m)->void {
	    if(country==19&&link[n][m]==1)
	    {
			cout << cnt++ << ": ";
	        for(int q=0;q<20;q++)
				cout << " " << a[q];
	        cout << " " << m << endl;
	    }
	    for(int i=1;i<=20;i++)
	    {
	        if(visit[i]==0&&link[n][i]==1&&n!=i)
	        {
	            visit[i]=1;
	            a[country+1]=i;
	            self(self,i,country+1,m);
	            visit[i]=0;
	        }
	    }
	});


	memset(link,0,sizeof(link));
	//构造多面体
    for(int i=1;i<=20;i++) {
        int a,b,c;
        cin >> a >> b >> c;
        link[i][a]=link[i][b]=link[i][c]=1;
    }

    int m=1;
    while(m!=0)
    {
        memset(visit,0,sizeof(visit));
        cin >>m;
        if(m==0) break;
        visit[m]=1;
        a[0]=m;
        DFS(m,0,m);
    }
} 

void HDU3533(){
	int n,m,k,life;
	int dt[5][2] = {0,1,1,0,0,-1,-1,0,0,0};//四个方向与停止不动的走法
	vector<vector<int>> map(105, vector<int>(105));
	vector<vector<vector<bool>>> vis(105, vector<vector<bool>>(105, vector<bool>(105)));
	
	struct period
	{
	    char c;
	    int t,v;
	};
	vector<vector<period>> s(105,vector<period>(105));

	struct node
	{
	    int x,y,step;
	};

	auto check = [&](int x,int y) {
    	if(x<0 || x>n || y<0 || y>m)
    	    return 1;
    	return 0;
	};

	auto bfs = [&]()
	{
	    node now,net;
	    queue<node> q;
	    int i,j,flag,dis,timee;
	    now.x = now.y = now.step = 0;
	    q.push(now);
	    vis[0][0][0] = true;
	    while(!q.empty())
	    {
	        now = q.front();
	        q.pop();
	        if(now.step>life)
	            break;
	        if(now.x == n && now.y == m)
	        {
	            cout<<now.step<<endl;
	            return ;
	        }
	        for(i = 0; i<5; i++)
	        {
	            net = now;
	            net.x+=dt[i][0];
	            net.y+=dt[i][1];
	            net.step++;
	            if(check(net.x,net.y))
	                continue;
	            if(!s[net.x][net.y].t && !vis[net.x][net.y][net.step] && net.step<=life)//在符合条件的情况下，枚举四个方向
	            {
	                flag = 1;
	                for(j = net.x-1; j>=0; j--)//当位于这点，我们往北向寻找是否有朝南方向射击的炮台
	                {
	                    if(s[j][net.y].t && s[j][net.y].c == 'S')//找到第一个炮台，且这个炮台是朝南射击的
	                    {
	                        dis = net.x-j;//看炮台与人的距离
	                        if(dis%s[j][net.y].v)
	                            break;//因为不需要看子弹中途的点，子弹每一秒跑v，距离是dis，dis不能整除v的话，那么子弹是不可能停在这个点的
	                        timee = net.step-dis/s[j][net.y].v;//人走的时间减去第一个子弹飞行到这个位置所需的时间
	                        if(timee<0)
	                            break;//为负数就是第一个子弹都没有经过这个点，那么人绝对安全
	                        if(timee%s[j][net.y].t==0)//看间隔，能整除，那么就是后续有子弹刚好到这个点，人死定了
	                        {
	                            flag = 0;
	                            break;
	                        }
	                    }
	                    if(s[j][net.y].t)//找到炮台但不是朝南射击，那么这个炮台会当下后面所有子弹，所以北方向安全我们不需要再找
	                        break;
	                }
	                if(!flag)//这个方向都死定了，后面也就不需要看了
	                    continue;
	                //其他方向也是一样的道理，就不注释了
	                for(j = net.x+1; j<=n; j++)
	                {
	                    if(s[j][net.y].t && s[j][net.y].c == 'N')
	                    {
	                        dis = j-net.x;
	                        if(dis%s[j][net.y].v)
	                            break;
	                        timee = net.step-dis/s[j][net.y].v;
	                        if(timee<0)
	                            break;
	                        if(timee%s[j][net.y].t==0)
	                        {
	                            flag = 0;
	                            break;
	                        }
	                    }
	                    if(s[j][net.y].t)
	                        break;
	                }
	                if(!flag)
	                    continue;
	                for(j = net.y-1; j>=0; j--)
	                {
	                    if(s[net.x][j].t && s[net.x][j].c == 'E')
	                    {
	                        dis = net.y-j;
	                        if(dis%s[net.x][j].v)
	                            break;
	                        timee = net.step-dis/s[net.x][j].v;
	                        if(timee<0)
	                            break;
	                        if(timee%s[net.x][j].t==0)
	                        {
	                            flag = 0;
	                            break;
	                        }
	                    }
	                    if(s[net.x][j].t)
	                        break;
	                }
	                if(!flag)
	                    continue;
	                for(j = net.y+1; j<=m; j++)
	                {
	                    if(s[net.x][j].t && s[net.x][j].c == 'W')
	                    {
	                        dis = j-net.y;
	                        if(dis%s[net.x][j].v)
	                            break;
	                        timee = net.step-dis/s[net.x][j].v;
	                        if(timee<0)
	                            break;
	                        if(timee%s[net.x][j].t==0)
	                        {
	                            flag = 0;
	                            break;
	                        }
	                    }
	                    if(s[net.x][j].t)
	                        break;
	                }
	                if(!flag)
	                    continue;
	                vis[net.x][net.y][net.step] = true;
	                q.push(net);
	            }
	        }
	    }
	    cout<<"Bad luck!"<<endl;
	};

	
	while(cin>>n>>m>>k>>life)
    {
        for(int i=0; i<k; i++)
        {
            char c;
            int t,v,x,y;
            cin>>c>>t>>v>>x>>y;
            s[x][y].c=c;
            s[x][y].t=t;
            s[x][y].v=v;
        }
        bfs();
    }
}

void HDU1560(){
	int pos[10];//pos[i]  第i个序列正在使用第几个位置
	int T,n;
	int deep;//自己构造的DNA序列最小长度
	char c[10] = "ACGT";
	struct node
	{
	    char ch[10];      //DNA的组成
	    int len;     //DNA长度
	};
	node a[10];//a[i]  第i个DNA序列
	auto init = [&]() {//预估长度
	    int ans=0;
	    for(int i=0;i<n;i++)//总长度-正在使用的位置=剩下还没用的位置 即预计长度
	        ans=max(ans,a[i].len-pos[i]);
	    return ans;
	};
	auto dfs = recursive_lambda([&](auto const &self, int step) {
	    if(step+init()>deep)//预计长度+当前长度>构造DNA序列的最小长度
	        return 0;
	    if(init()==0)//预计长度为0，即已完成
	        return 1;
	    int pre[10];//先将pos保存起来，一会回溯要用
	    for(int i=0;i<4;i++)
	    {
	        int f=0;
	        for(int j=0;j<n;j++)//保存pos
	            pre[j]=pos[j];
	        for(int j=0;j<n;j++)//当前这位符合，则该串的位置往后移一位
	        {
	            if(a[j].ch[pos[j]]==c[i])
	            {
	                f=1;
	                pos[j]++;
	            }
	        }
	        if(f)
	        {
	            if(self(self, step+1))//有符合的，则往下搜索
	                return 1;
	            for(int j=0;j<n;j++)//回溯
	                pos[j]=pre[j];
	        }
	    }
	    return 0;
	});
	
	cin>>T;
    while(T--)
    {
        deep=0;//自己构造的DNA序列最小长度
        cin>>n;
        for(int i=0;i<n;i++)//存值
        {
            cin>>a[i].ch;
            a[i].len=strlen(a[i].ch);
            deep=max(deep,a[i].len);
            pos[i]=0;
        }
        while(1)
        {
            if(dfs(0))
                break;
            deep++;
        }
        cout<<deep<<endl;
    }
}


void HDU3085(){
	const int N = 805;
	int t, n, m, zx1, zy1, zx2, zy2, mx, my, gx, gy, step; 
	vector<vector<int>> vis(N, vector<int>(N,0));
	vector<string> G(N);
	int dx[4] = {0, 1, 0, -1};
	int dy[4] = {1, 0, -1, 0};
	struct node {
		int x, y;  
		node(int xx, int yy) {
			x = xx; y = yy;  ;
		}
	};
	auto ok = [&](int x, int y) {
		if (x < 0 || y < 0 || x >= n || y >=  m || G[x][y] == 'X') return false;
		//看能不能被鬼覆盖
		if (abs(x - zx1) + abs(y - zy1) <= 2 * step) return false;
		if (abs(x - zx2) + abs(y - zy2) <= 2 * step) return false;
		
		return true;
	};
	auto bfs = [&](){
		//标记状态 0表示未访问 1表示男孩 2表示女孩访问 
		vis[mx][my] = 1; 
		vis[gx][gy] = 2;
		step = 0;
		//初始化时间 
		queue<node> qm, qg;
		qm.push(node(mx, my));
		qg.push(node(gx, gy));
		while (!qm.empty() && !qg.empty()) {
			step++;
			//男孩走3步
			for (int k = 0; k < 3; k++) {
				for (int i = 0, len = qm.size(); i < len; i++) {
					node t = qm.front();
					qm.pop();
					//判断一下刚出来的点是否能动
					if (!ok(t.x, t.y)) continue; 
					for (int j = 0; j < 4; j++) {
						int fx = t.x + dx[j];
						int fy = t.y + dy[j];				
						if (!ok(fx, fy) ||  vis[fx][fy] == vis[t.x][t.y]) continue;
						//看该点符合要求否 避免重复走 
						if (vis[fx][fy] + vis[t.x][t.y] == 3) {
							//找到答案
							return step;	 
						}
						vis[fx][fy] = vis[t.x][t.y];
						qm.push(node(fx, fy));	
					}
				}
			} 
			//女孩走一步 
			for (int i = 0, len = qg.size(); i < len; i++) {
				node t = qg.front();
				qg.pop();
				if (!ok(t.x, t.y)) continue; 
				for (int j = 0; j < 4; j++) {
					int fx = t.x + dx[j];
					int fy = t.y + dy[j];
					if (!ok(fx, fy) ||  vis[fx][fy] == vis[t.x][t.y]) continue;
					if (vis[fx][fy] + vis[t.x][t.y] == 3) {
						//找到答案
						return step;	 
					} 		
					vis[fx][fy] = vis[t.x][t.y];
					qg.push(node(fx, fy));		
				}
			}
		}
		return -1;
	};

	scanf("%d", &t);
	while (t--) {
		vector<vector<int>> vis_temp(N, vector<int>(N,0));
		vector<string> G_temp(N);
		vis.swap(vis_temp);
		G.swap(G_temp);
		scanf("%d%d", &n, &m);
		for (int i = 0; i < n; i++) {
			cin >> G[i];
		} 
		//找出男女和鬼的位置 
		int cnt = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (G[i][j] == 'Z') {
					if (!cnt){
							zx1 = i; zy1 = j;
						cnt++;	
					} else {
						zx2 = i; zy2= j;
					} 
				}
				if (G[i][j] == 'M') mx = i, my = j;
				if (G[i][j] == 'G') gx = i, gy = j;
			}
		}
		printf("%d\n", bfs());
	}
}

int main(){
#if 0
    
#endif
    // HDU1043();
	// HDU3567();
	// HDU2181();
	// HDU3533();
	// HDU1560();
	HDU3085();
}