#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define sz(x) (int)(x).size()
typedef vector<int> vi;
typedef long long ll;

struct Dinic {
	struct Edge {
		int to, rev;
		ll c, oc;
		ll flow() { return max(oc - c, 0LL); } // if you need flows
	};
	vi lvl, ptr, q;
	vector<vector<Edge>> adj;
	Dinic(int n): lvl(n), ptr(n), q(n), adj(n) {}
	void addEdge(int a, int b, ll c, ll rcap = 0) {
		adj[a].push_back({b, sz(adj[b]), c, c});
		adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
	}
	ll dfs(int v, int t, ll f) {
		if (v == t || !f) return f;
		for (int& i = ptr[v]; i < sz(adj[v]); i++) {
			Edge& e = adj[v][i];
			if (lvl[e.to] == lvl[v] + 1)
				if (ll p = dfs(e.to, t, min(f, e.c))) {
					e.c -= p, adj[e.to][e.rev].c += p;
					return p;
				}
		}
		return 0;
	}
	ll calc(int s, int t) {
		ll flow = 0;
		q[0] = s;
		rep(L, 0,
			31) do { // 'int L=30' maybe faster for random data
			lvl = ptr = vi(sz(q));
			int qi = 0, qe = lvl[s] = 1;
			while (qi < qe && !lvl[t]) {
				int v = q[qi++];
				for (Edge e : adj[v])
					if (!lvl[e.to] && e.c >> (30 - L))
						q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
			}
			while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
		}
		while (lvl[t])
			;
		return flow;
	}
	bool leftOfMinCut(int a) { return lvl[a] != 0; }
};
int main(){
    int n, m;
    cin >> n >> m;
    vector<int> inspections(n);
    vector<vector<int>> adj(n,vector<int>(n));
    vector<vector<int>> direct(n,vector<int>(n));
    for(int i = 0;i<n;i++){
        cin >> inspections[i];
    }
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
			int x;
			cin >> x;
            adj[i][j] = direct[i][j]=x+inspections[j];
        }
    }
	for(int i = 0;i<n;i++)adj[i][i] = 0;
	for(int k = 0;k<n;k++){
		for(int i = 0;i<n;i++){
			for(int j = 0;j<n;j++){
				adj[i][j] = min(adj[i][j],adj[i][k]+adj[k][j]);
			}
		}
	}
    vector<vector<int>> flights(3,vector<int>(m));
    for(int i = 0;i<m;i++){
        for(int j = 0;j<3;j++){
            cin >> flights[j][i];
        }
        flights[0][i]--;
        flights[1][i]--;
    }
    Dinic flow(m*2+2);
    int source = m*2;
    int sink = m*2+1;
    for(int i = 0;i<m;i++){
        flow.addEdge(source,i,1);
        int time = flights[2][i]+direct[flights[0][i]][flights[1][i]];
        int location = flights[1][i]; 
        for(int j = 0;j<m;j++){
			if(i==j)continue;
            int by = flights[2][j];
			int newTime = time+adj[location][flights[0][j]];
            if(newTime <= by){
                flow.addEdge(i,j+m,1);
            }
        }
        flow.addEdge(i+m,sink,1);
    }
    cout << m - flow.calc(source,sink) << "\n";

    
}