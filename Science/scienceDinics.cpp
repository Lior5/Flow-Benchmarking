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
        // cout << "Nodes: "<<n << "\n";
        // cout << "Edges: "<<edges << "\n";
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
    int size = n*2;
    vector<vector<bool>> valid(n,vector<bool>(n));
	for(int i = 0;i<n;i++){
		string s;
		cin >> s;
		for(int j = 0;j<n;j++){
			valid[i][j] = (s[j] == 'Y'); 	
		}
	}
	auto buildgraph = [&](int k) -> Dinics {
		Dinics flow(size);
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
    	Dinics flow = buildgraph(k);
		if(flow.maxFlow() >= k*n) orderings = k;
		else break;
	}
	vector<vector<int>> ans(orderings);
    for(int k = orderings-1;k>=0;k--){
		Dinics currFlow = buildgraph(k+1);
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
		Dinics nextFlow = buildgraph(k);
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