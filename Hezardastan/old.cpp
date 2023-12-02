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
    cin >> n >>m;
    vector<vector<int>> board(n,vector<int>(m));
    for(int i = 0;i<n;i++){
        for(int j = 0;j<m;j++){
            cin >> board[i][j];
        }
    }
    for(int outer = 0;outer<n;outer++){
        Dinic flow(n+m+2);
        int source = n+m;
        int sink = source+1;
        for(int i = 0;i<m;i++){
            flow.addEdge(i+n,sink,1);
        }
        for(int i = 0;i<n;i++){
            int cap = 1;
            if(i == outer)cap = 100000;
            flow.addEdge(source,i,cap);
            for(int j =0;j<m;j++){
                if(board[i][j])flow.addEdge(i,j+n,1);
            }
        }
        int ans = flow.calc(source,sink);
        cout << ans << "\n";
        if(ans<m){
            int num = -3;
            cout << "bleh\n";
            for(int i = 0;i<m;i++){
                cout << flow.adj[i+n][sink].oc << "\n";
            }
            cout << "bleh2\n";
            for(int i = 0;i<m;i++){
                cout << flow.adj[i+n][sink].flow() << "\n";
                if(flow.adj[i+n][sink].flow() == 0){
                    num =i+1;
                    break;
                }
            }
            cout << "0\n";
            cout << num << " ";
            int counter = 0;
            for(int i = 0;i<m;i++){
                if(i+1==num){
                    continue;
                }
                if(counter++ == n-1)break;
                cout << (i+1) << " ";
            }
            return 0;
        }
        
        cout << "1\n";
    }
}