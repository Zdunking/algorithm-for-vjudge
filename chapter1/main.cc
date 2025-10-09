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
int main(){
#if 0

#endif
    POJ1321();

    return 0;
}