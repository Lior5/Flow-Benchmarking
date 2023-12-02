#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

struct FordFulkerson{
    
    int n;
    vector<vector<int>> adj;
    int source, sink;
    int big = (int)(1e9);
    FordFulkerson(int size) {
        adj.clear();
        n = size + 2;
        adj = vector<vector<int>>(n,vector<int>(n));
        source = size;
        sink = size+1;
    }
    void add(int from, int to, int capacity){
        adj[from][to] += capacity;
    }
    int maxFlow(){
        int ans = 0;
        while(true){
            vector<bool> visited(n);
            vector<tuple<int,int,int>> updates;
            int augpath = dfs(source,visited,big,updates);
            for(auto [a,b,c]:updates){
                adj[a][b]+=c;
            }
            ans+=augpath;
            if(augpath == 0)break;
        }
        return ans;
    }
    void printcaps(){
        for(int i = 0;i<n;i++){
            for(int j = 0;j<n;j++){
                cout << adj[i][j] << " ";
            }
            cout << "\n";
        }
    }
    int dfs(int idx, vector<bool> &v, int minFlow, vector<tuple<int,int,int>> &updates){
        if(idx == sink)return minFlow;
        if(v[idx])return 0;
        v[idx] = true;
        for(int i = 0;i<adj[idx].size();i++){
            int augmin = 0;
            if(!v[i] && adj[idx][i] > 0){
                augmin = dfs(i,v, min(minFlow,adj[idx][i]),updates);
            }
            if(augmin > 0){
                updates.emplace_back(idx,i,-augmin);
                updates.emplace_back(i,idx,augmin);
                // guaranteed i exist
                return augmin;
            }
        }

        return 0;
    }
};