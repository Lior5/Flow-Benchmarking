#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;
#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define sz(x) (int)(x).size()
typedef vector<int> vi;
typedef long long ll;

struct Edge {
    int i, j, cap, flow, carry;
    Edge* rev;
    Edge(int ii, int jj, int cc, int ff) : i(ii), j(jj), cap(cc), flow(ff), carry(0) {}
};

struct Dinic {
	struct Edge {
		int to, rev;
		ll c, oc;
		ll flow() { return max(oc - c, 0LL); } // if you need flows
	};
	vi lvl, ptr, q;
	vector<vector<Edge>> adj;
	Dinic(int n): lvl(n), ptr(n), q(n), adj(n) {}
	void add(int a, int b, ll c, ll rcap = 0) {
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

void* threadFunction(void* arg) {
    int t;
    cin >> t;
    while(t-->0){
        int n, m;
        cin >> n >> m;
        vector<string> board(n);
        for(int i = 0;i<n;i++){
            cin >> board[i];
        }
        vector<vector<int>> diag1(n,vector<int>(m,-1));
        vector<vector<int>> diag2(n,vector<int>(m,-1));
        int countW = 0;
        int countB = 0;

        vector<vector<int>> whiteIdxs(n,vector<int>(m));
        for(int i = 0;i<n;i++){
            for(int j = 0;j<m;j++){
                if(board[i][j] == 'W')whiteIdxs[i][j] = countW++;
                if(board[i][j] == 'B'){
                    countB++;
                }
            }
        }
        int bound = countB*3+countW+2;
        int source = bound-2;
        int sink = bound-1;
        Dinic flow(bound);
        int currB = 0, currH = 0, currV = 0;
        int hori = countB;
        int vert = countB*2;
        int white = countB*3;
        for(int i = 0;i<n;i++){
            for(int j = 0;j<m;j++){
                if(board[i][j] == 'B'){
                    int idx = currB++;
                    int origin = 0;
                    flow.add(source,idx,2);
                    flow.add(idx,idx+hori,1);
                    flow.add(idx,idx+vert,1);
                    if(i>0 && board[i-1][j] == 'W')
                        flow.add(idx+vert,whiteIdxs[i-1][j]+white,1);
                    if(i<n-1 && board[i+1][j] == 'W')
                        flow.add(idx+vert,whiteIdxs[i+1][j]+white,1);
                    if(j>0 && board[i][j-1] == 'W')
                        flow.add(idx+hori,whiteIdxs[i][j-1]+white,1);
                    if(j<m-1 && board[i][j+1] == 'W')
                        flow.add(idx+hori,whiteIdxs[i][j+1]+white,1);
                }
            }
        }
        for(int i = 0;i<bound;i++){
            if(i>=white && i!=source){
                flow.add(i,sink,1);
            }
        }
        int ans = flow.calc(source,sink);
        if(ans!=countW || countB*2!=countW){
            cout << "NO\n";
        }else{
            cout << "YES\n";
        }
    }
    return 0;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 512*1024*1024); // Set stack size to 8 MB.

    pthread_t thread;
    pthread_create(&thread, &attr, threadFunction, nullptr);


    pthread_attr_destroy(&attr); // Don't forget to destroy the attribute object!

    // Your code here.

    pthread_join(thread, nullptr); // Wait for the thread to finish.
    return 0;
}