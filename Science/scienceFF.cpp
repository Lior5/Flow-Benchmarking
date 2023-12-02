#include <bits/stdc++.h>
using namespace std;

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


int main(){
    int n;
    cin >> n;
    int size = n*2;
    vector<vector<bool>> valid(n,vector<bool>(n));
	for(int i = 0;i<n;i++){
		string s;
		cin >> s;
		for(int j = 0;j<n;j++){
			valid[i][j] = (s[j] == 'Y'); 	
		}
	}
	auto buildgraph = [&](int k) -> FordFulkerson {
		FordFulkerson flow(size);
		for(int i = 0;i<n;i++){
			flow.add(flow.source,i,k);
			for(int j = 0;j<n;j++){
				if(valid[i][j])
					flow.add(i,j+n,1);
			}
			flow.add(i+n,flow.sink,k);
		}
		return flow;
	};
	int orderings = 0;
	for(int k = 1;k<=n;k++){
    	FordFulkerson flow = buildgraph(k);
		if(flow.maxFlow() >= k*n) orderings = k;
		else break;
	}
	vector<vector<int>> ans(orderings);
    for(int k = orderings-1;k>=0;k--){
		FordFulkerson currFlow = buildgraph(k+1);
		// have to set valid to only the grid cells taken by currFlow
		int currmax = currFlow.maxFlow();
		for(int i = 0;i<n;i++){
			for(int j = 0;j<n;j++){
				if(currFlow.indices[i].count(j+n) && valid[i][j]){
					if(currFlow.adj[i][currFlow.indices[i][j+n]].second > 0){ 
						valid[i][j] = false;
					}
				}
			}
		}
		FordFulkerson nextFlow = buildgraph(k);
		int nextmax = nextFlow.maxFlow();
		// cout << k << "\n";
		// cout << currmax << " " << nextmax << "\n";
		for(int j = 0;j<n;j++){
			int asd =0;
			for(int i = 0;i<n;i++){
				if(currFlow.indices[i].count(j+n) && nextFlow.indices[i].count(j+n)){
					if(currFlow.adj[i][currFlow.indices[i][j+n]].second != 
						nextFlow.adj[i][nextFlow.indices[i][j+n]].second){
							valid[i][j] = false;
							// cout << j << " ";
							ans[k].push_back(i+1);
							asd++;
					}
				}
			}
			// cout << "ASD " << asd << "\n";
		}
    }
	cout << orderings << "\n";
	for(auto row: ans){
		for(int in:row){
			cout << in << " ";
		}
		cout << "\n";
	}
}