#include <bits/stdc++.h>
#include <pthread.h>
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
    int N, fptr, bptr, s, t, oo = (int)(1e9);

    TidalFlow(int NN) : adjj(NN+2), adj(NN+2), q(NN+2), dist(NN+2), pool(NN+2) {
        N = (t = (s = NN) + 1) + 1;
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
        pool[t] = dist[s] = fptr = bptr = 0;
        pool[q[bptr++] = s] = oo;
        while (bptr > fptr && q[fptr] != t)
            for (Edge* e : adj[q[fptr++]]) {
                if (dist[e->i] < dist[e->j])
                    pool[e->j] += e->carry = min(e->cap - e->flow, pool[e->i]);
                if (dist[e->i] + 1 < dist[e->j] && e->cap > e->flow)
                    dist[q[bptr++] = e->j] = dist[e->i] + 1;
            }
        if (pool[t] == 0) return 0;
        fill(pool.begin(), pool.end(), 0);
        fptr = bptr = 0;
        pool[q[bptr++] = t] = oo;
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
        int res = pool[s];
        fill(pool.begin(), pool.end(), 0);
        pool[s] = res;
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

void* threadFunction(void* arg) {
    cin.tie(0)->sync_with_stdio(0);
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
        int bound = countB*3+countW;
        TidalFlow flow(bound);
        int source = flow.s;
        int sink = flow.t;
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
        int ans = flow.maxFlow();
        if(ans!=countW || countB*2!=countW){
            cout << "NO\n";
        }else{
            cout << "YES\n";
        }
    }
    return 0;
}

int main() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 8*1024*1024); // Set stack size to 8 MB.

    pthread_t thread;
    pthread_create(&thread, &attr, threadFunction, nullptr);

    pthread_attr_destroy(&attr); // Don't forget to destroy the attribute object!

    // Your code here.

    pthread_join(thread, nullptr); // Wait for the thread to finish.
    return 0;
}