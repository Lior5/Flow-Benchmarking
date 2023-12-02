#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define sz(x) (int)(x).size()
typedef vector<int> vi;
typedef long long ll;
struct Dinics{
    int n;
    vector<unordered_map<int,int>> indices;
    vector<vector<pair<int,int>>> adj;
    int source, sink;
    vector<int> levels;
    vector<int> edge_ptrs;
    int big = (int)(1e9);
    Dinics(int size){
        adj.clear();
        source = size;
        sink = size+1;
        n = size+2;
        indices = vector<unordered_map<int,int>>(n);
        adj = vector<vector<pair<int,int>>>(n);
        levels = vector<int>(n,-1);
        edge_ptrs = vector<int>(n,0);
    }
    void add(int from, int to, int capacity){
        if(indices[from].count(to)){
            adj[from][indices[from][to]].second += capacity;
        }else{
            indices[from][to] = adj[from].size();
            adj[from].emplace_back(to,capacity);
        }
    }
    void build_graph(){
        queue<int> q;
        q.push(source);
        levels = vector<int>(n,-1);
        edge_ptrs = vector<int>(n,0);
        levels[source] = 0;
        while(q.size()>0){
            int curr = q.front();
            q.pop();
            for(auto [to,capacity] : adj[curr]){
                if(levels[to]!=-1)continue;
                if(capacity > 0){
                    levels[to] = levels[curr]+1;
                    q.push(to);
                }
            }
        }
    }
    int maxFlow(){
        int ans = 0;
        while(true){
            build_graph();
            int aug = dfs(source,big);
            if(aug==0)break;
            ans+=aug;
        }
        return ans;
    }
    int dfs(const int idx,int minFlow){
        if(idx == sink)return minFlow;
        while(edge_ptrs[idx]<adj[idx].size()){
            int i = edge_ptrs[idx]++;
            auto[to, capacity] = adj[idx][i];
            int augmin = 0;
            if(levels[to] > levels[idx] && capacity>0){
                augmin = dfs(to,min(minFlow,capacity));
            }
            if(augmin>0){
                add(idx,to,-augmin);
                add(to,idx,augmin);
                return augmin;
            }
        }
        return 0;
    }
};
int main(){
    int n;
    cin >> n;
    auto vt = vector<tuple<int,int,int>>();
    Dinics din(1003);
    int source = din.source;
    int sink = din.sink;
    for(int i = 0;i<n;i++){
        int a, b, c;
        cin >> a >> b >> c;
        vt.emplace_back(a,b,c);
    }
    unordered_map<int,int> mp;
    auto inBoinds = [](int a, int b, int c){
        return (min({a,b,c})>=0 && max({a,b,c})<=9);
    };
    auto get_idx = [](int a, int b, int c){
        return a*100+b*10+c;
    };
    int cnt = 0;
    for(int i = 0;i<10;i++){
        for(int j = 0;j<10;j++){
            for(int k = 0;k<10;k++){
                if(inBoinds(i+1,j,k))din.add(get_idx(i,j,k),get_idx(i+1,j,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j+1,k))din.add(get_idx(i,j,k),get_idx(i,j+1,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j,k+1))din.add(get_idx(i,j,k),get_idx(i,j,k+1),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i-1,j,k))din.add(get_idx(i,j,k),get_idx(i-1,j,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j-1,k))din.add(get_idx(i,j,k),get_idx(i,j-1,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j,k-1))din.add(get_idx(i,j,k),get_idx(i,j,k-1),1); else 
                    din.add(get_idx(i,j,k),sink,1);
            }   
        }
    }
    // cout << "hello\n";
    for(auto tp:vt){
        auto[a,b,c] = tp;
        din.add(source,get_idx(a,b,c),1000);
    }
    ll ans = din.maxFlow();
    cout << ans << "\n";

}