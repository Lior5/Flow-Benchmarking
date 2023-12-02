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
    auto vt = vector<tuple<int,int,int>>();
    TidalFlow din(1003);
    int source = din.source;
    int sink = din.sink;
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
                if(inBoinds(i+1,j,k))din.add(get_idx(i,j,k),get_idx(i+1,j,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j+1,k))din.add(get_idx(i,j,k),get_idx(i,j+1,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j,k+1))din.add(get_idx(i,j,k),get_idx(i,j,k+1),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i-1,j,k))din.add(get_idx(i,j,k),get_idx(i-1,j,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j-1,k))din.add(get_idx(i,j,k),get_idx(i,j-1,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j,k-1))din.add(get_idx(i,j,k),get_idx(i,j,k-1),1); else 
                    din.add(get_idx(i,j,k),sink,1);
            }   
        }
    }
    // cout << "hello\n";
    for(auto tp:vt){
        auto[a,b,c] = tp;
        din.add(source,get_idx(a,b,c),1000);
    }
    int ans = din.maxFlow();
    cout << ans << "\n";

}