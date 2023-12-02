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
    int n;
    cin >> n;
    vector<int> value(n);
    vector<int> cost(n);
    vector<vector<int>> depend(n);
    int sumV = 0, sumG = 0;
    for(int i = 0;i<n;i++){
        cin >> value[i] >> cost[i];
        sumV+=value[i];
        int howMany;
        cin >> howMany;
        for(int j = 0;j<howMany;j++){
            int curr;
            cin >> curr;
            depend[i].push_back(curr-1);
        }
    }
    Dinic flow(n+2);
    int source = n;
    int sink = n+1;
    for(int i = 0;i<n;i++){
        flow.addEdge(source,i,value[i]);
        flow.addEdge(i,sink,cost[i]);
        for(int need:depend[i]){
            flow.addEdge(need,i,100000);
        }
    }
    cout << sumV - flow.calc(source,sink) << "\n";
}