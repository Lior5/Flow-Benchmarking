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
    auto vt = vector<tuple<int,int,int>>();
    Dinic din(1003);
    int source = 1001, sink = 1002;
    for(int i = 0;i<n;i++){
        int a, b, c;
        cin >> a >> b >> c;
        vt.emplace_back(a,b,c);
    }
    unordered_map<int,int> mp;
    auto inBoinds = [](int a, int b, int c){
        return (min({a,b,c})>=0 && max({a,b,c})<=9);
    };
    auto get_idx = [](int a, int b, int c){
        return a*100+b*10+c;
    };
    int cnt = 0;
    for(int i = 0;i<10;i++){
        for(int j = 0;j<10;j++){
            for(int k = 0;k<10;k++){
                if(inBoinds(i+1,j,k))din.addEdge(get_idx(i,j,k),get_idx(i+1,j,k),1); else 
                    din.addEdge(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j+1,k))din.addEdge(get_idx(i,j,k),get_idx(i,j+1,k),1); else 
                    din.addEdge(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j,k+1))din.addEdge(get_idx(i,j,k),get_idx(i,j,k+1),1); else 
                    din.addEdge(get_idx(i,j,k),sink,1);
                if(inBoinds(i-1,j,k))din.addEdge(get_idx(i,j,k),get_idx(i-1,j,k),1); else 
                    din.addEdge(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j-1,k))din.addEdge(get_idx(i,j,k),get_idx(i,j-1,k),1); else 
                    din.addEdge(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j,k-1))din.addEdge(get_idx(i,j,k),get_idx(i,j,k-1),1); else 
                    din.addEdge(get_idx(i,j,k),sink,1);
            }   
        }
    }
    // cout << "hello\n";
    for(auto tp:vt){
        auto[a,b,c] = tp;
        din.addEdge(source,get_idx(a,b,c),1000);
    }
    ll ans = din.calc(source,sink);
    cout << ans << "\n";

}