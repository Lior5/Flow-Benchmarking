#include <bits/stdc++.h>
using namespace std;

struct Dinics{
    int n;
    vector<vector<int>> adj;
    int source, sink;
    vector<int> levels;
    vector<int> edge_ptrs;
    int big = (int)(1e9);
    Dinics(int size){
        adj.clear();
        source = size;
        sink = size+1;
        n = size+2;
        adj = vector<vector<int>>(n,vector<int>(n));
        levels = vector<int>(n,-1);
        edge_ptrs = vector<int>(n,0);
    }
    void add(int from, int to, int capacity){
        adj[from][to] += capacity;
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
            for(int i = 0 ;i<n;i++){
                if(levels[i]!=-1)continue;
                if(adj[curr][i]>0){
                    levels[i] = levels[curr]+1;
                    q.push(i);
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
        while(edge_ptrs[idx]<n){
            int i = edge_ptrs[idx]++;
            int augmin = 0;
            if(levels[i] > levels[idx] && adj[idx][i]>0){
                augmin = dfs(i,min(minFlow,adj[idx][i]));
            }
            if(augmin>0){
                adj[idx][i]-=augmin;
                adj[i][idx]+=augmin;
                return augmin;
            }
        }
        return 0;
    }
};
