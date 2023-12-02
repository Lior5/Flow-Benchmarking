#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define sz(x) (int)(x).size()
typedef vector<int> vi;
typedef long long ll;
struct PushRelabel{
    int big = (int)(1e9);
    int n;
    vector<unordered_map<int,int>> indices;
    vector<vector<pair<int,int>>> adj;
    int source, sink;
    queue<int> excess_vertices;
    vector<int> height, excess;
    PushRelabel(int size){
        n = size+2;
        source = size;
        sink = size+1;
        indices = vector<unordered_map<int,int>>(n);
        adj = vector<vector<pair<int,int>>>(n);
        height = vector<int>(n);
        excess = vector<int>(n);
    }
    void add(int from, int to, int capacity){
        if(indices[from].count(to)){
            adj[from][indices[from][to]].second += capacity;
        }else{
            indices[from][to] = adj[from].size();
            adj[from].emplace_back(to,capacity);
        }
    }
    void push(int from, int to){
        int capacity = adj[from][indices[from][to]].second;
        int d = min(excess[from], capacity);
        d = max(d,0);
        add(from,to,-d);
        add(to,from,d);
        excess[from] -=d;
        excess[to] += d;
        return;
    }
    bool discharge(int idx) {
        bool good = false;
        for(auto[to,capacity]:adj[idx]){
            if(capacity <= 0)continue;
            if(height[idx] > height[to]) {
                push(idx, to);
                good = true;
            }
        }
        return good;
    }
    void relabel(int idx){
        int mn = big;
        for(auto [to,capacity]:adj[idx]){
            if(capacity==0)continue;
            mn = min(mn,height[to]);
        }
        if(mn!=big){
            height[idx] = mn+1;
        }
    }
    int maxFlow(){
        int sum = 0;
        for(auto [to,capacity]:adj[source]){
            sum+=capacity;
        }
        excess[source] = sum;
        height[source] = n;
        for(auto [to,capacity]:adj[source]){
            if(capacity>0) push(source,to);
        }
        while(true){
            bool found = false;
            for(int i = 0;i<n-2;i++){
                if(excess[i]>0){
                    if(!discharge(i)){
                        relabel(i);
                    }
                    found = true;
                }
            }
            if(!found)break;
        }
        return excess[sink];
    }
};

int main(){
    int n;
    cin >> n;
    auto vt = vector<tuple<int,int,int>>();
    PushRelabel din(1003);
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