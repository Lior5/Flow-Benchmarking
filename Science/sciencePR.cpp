#include <bits/stdc++.h>
using namespace std;

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
    int size = n*2;
    vector<vector<bool>> valid(n,vector<bool>(n));
	for(int i = 0;i<n;i++){
		string s;
		cin >> s;
		for(int j = 0;j<n;j++){
			valid[i][j] = (s[j] == 'Y'); 	
		}
	}
	auto buildgraph = [&](int k) -> PushRelabel {
		PushRelabel flow(size);
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
    	PushRelabel flow = buildgraph(k);
		if(flow.maxFlow() >= k*n) orderings = k;
		else break;
	}
	vector<vector<int>> ans(orderings);
    for(int k = orderings-1;k>=0;k--){
		PushRelabel currFlow = buildgraph(k+1);
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
		PushRelabel nextFlow = buildgraph(k);
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