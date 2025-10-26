#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
using namespace std;



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
int main(){
#if 0
    
#endif
    HDU1043();
}