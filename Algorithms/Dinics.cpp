#include <bits/stdc++.h>
using namespace std;

struct Dinics{
    int n;
    vector<unordered_map<int,int>> indices;
    vector<vector<pair<int,int>>> adj;
    int source, sink;
    vector<int> levels;
    vector<int> edge_ptrs;
    int big = (int)(1e9);
    int edges;
    Dinics(int size){
        adj.clear();
        source = size;
        sink = size+1;
        n = size+2;
        edges = 0;
        indices = vector<unordered_map<int,int>>(n);
        adj = vector<vector<pair<int,int>>>(n);
        levels = vector<int>(n,-1);
        edge_ptrs = vector<int>(n,0);
    }
    void add(int from, int to, int capacity){
        if(indices[from].count(to)){
            adj[from][indices[from][to]].second += capacity;
        }else{
            edges++;
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
        cout << "Nodes: "<<n << "\n";
        cout << "Edges: "<<edges << "\n";
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
