#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int i, j, cap, flow, carry;
    Edge* rev;
    Edge(int ii, int jj, int cc, int ff) : i(ii), j(jj), cap(cc), flow(ff), carry(0) {}
};

struct TidalFlow {
    deque<Edge*> stk;
    vector<vector<Edge*>> adjj, adj;
    vector<int> q, dist, pool;
    int N, fptr, bptr, source, sink, oo = (int)(1e9);

    TidalFlow(int NN) : adjj(NN+2), adj(NN+2), q(NN+2), dist(NN+2), pool(NN+2) {
        N = (sink = (source = NN) + 1) + 1;
    }

    void add(int i, int j, int cap) {
        Edge* fwd = new Edge(i, j, cap, 0);
        Edge* rev = new Edge(j, i, 0, 0); // for bidirectional, use -cap
        fwd->rev = rev;
        rev->rev = fwd;
        adjj[i].push_back(fwd);
        adjj[j].push_back(rev);
    }

    int augment() {
        fill(dist.begin(), dist.end(), INT_MAX);
        pool[sink] = dist[source] = fptr = bptr = 0;
        pool[q[bptr++] = source] = oo;
        while (bptr > fptr && q[fptr] != sink)
            for (Edge* e : adj[q[fptr++]]) {
                if (dist[e->i] < dist[e->j])
                    pool[e->j] += e->carry = min(e->cap - e->flow, pool[e->i]);
                if (dist[e->i] + 1 < dist[e->j] && e->cap > e->flow)
                    dist[q[bptr++] = e->j] = dist[e->i] + 1;
            }
        if (pool[sink] == 0) return 0;
        fill(pool.begin(), pool.end(), 0);
        fptr = bptr = 0;
        pool[q[bptr++] = sink] = oo;
        while (bptr > fptr)
            for (Edge* e : adj[q[fptr++]]) {    
                if (pool[e->i] == 0) break;
                int f = e->rev->carry = min(pool[e->i], e->rev->carry);
                if (dist[e->i] > dist[e->j] && f != 0) {
                    if (pool[e->j] == 0)
                        q[bptr++] = e->j;
                    pool[e->i] -= f;
                    pool[e->j] += f;
                    stk.push_back(e->rev);
                }
            }
        int res = pool[source];
        fill(pool.begin(), pool.end(), 0);
        pool[source] = res;
        while (!stk.empty()) {
            Edge* e = stk.back(); stk.pop_back();
            int f = min(e->carry, pool[e->i]);
            pool[e->i] -= f; pool[e->j] += f;
            e->flow += f; e->rev->flow -= f;
        }
        return res;
    }
    int maxFlow() {
        int res = 0, f = 1;
        adj = adjj; // copy vectors
        while (f != 0) res += (f = augment());
        return res;
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
	auto buildgraph = [&](int k) -> TidalFlow {
		TidalFlow flow(size);
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
    	TidalFlow flow = buildgraph(k);
		if(flow.maxFlow() >= k*n) orderings = k;
		else break;
	}
	vector<vector<int>> ans(orderings);
    for(int k = orderings-1;k>=0;k--){
		TidalFlow currFlow = buildgraph(k+1);
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
		TidalFlow nextFlow = buildgraph(k);
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