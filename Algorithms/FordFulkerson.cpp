#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

struct FordFulkerson{
    int n;
    vector<unordered_map<int,int>> indices;
    vector<vector<pair<int,int>>> adj;
    int source, sink;
    int big = (int)(1e9);
    FordFulkerson(int size) {
        adj.clear();
        n = size + 2;
        indices = vector<unordered_map<int,int>>(n);
        adj = vector<vector<pair<int,int>>>(n);
        source = size;
        sink = size+1;
    }
    void add(int from, int to, int capacity){
        if(indices[from].count(to)){
            adj[from][indices[from][to]].second += capacity;
        }else{
            indices[from][to] = adj[from].size();
            adj[from].emplace_back(to,capacity);
        }
    }
    int maxFlow(){
        int ans = 0;
        while(true){
            vector<bool> visited(n);
            vector<tuple<int,int,int>> updates;
            int augpath = dfs(source,visited,big,updates);
            for(auto [a,b,c]:updates){
                add(a,b,c);
            }
            ans+=augpath;
            if(augpath == 0)break;
        }
        return ans;
    }
    void printcaps(){
        for(int i = 0;i<n;i++){
            for(auto[to,capacity]:adj[i]){
                cout << capacity << " ";
            }
            cout << "\n";
        }
    }
    int dfs(int idx, vector<bool> &v, int minFlow, vector<tuple<int,int,int>> &updates){
        if(idx == sink)return minFlow;
        if(v[idx])return 0;
        v[idx] = true;
        for(auto [to,capacity] : adj[idx]){
            int augmin = 0;
            if(!v[to] && capacity > 0){
                augmin = dfs(to,v,min(minFlow,capacity),updates);
            }
            if(augmin > 0){
                updates.emplace_back(idx,to,-augmin);
                updates.emplace_back(to,idx,augmin);
                return augmin;
            }
        }
        return 0;
    }
};