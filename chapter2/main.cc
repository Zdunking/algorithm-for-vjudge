#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
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


int main(){
#if 0
    
#endif
    // HDU1043();
	// HDU3567();
	HDU2181();
}