#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
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
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int t;
    cin >> t;
    while(t-->0){
        int n, m;
        cin >> n >> m;
        string trash;
        getline(cin,trash);
        unordered_set<string> names;
        vector<tuple<int,int,int,string>> edges;
        for(int i = 0;i<m;i++){
            string s;
            getline(cin,s);
            vector<string> tokens = split(s, ',');
            int a = stoi(tokens[0]);
            int b = stoi(tokens[1]);
            int cap = stoi(tokens[2]);
            string street = tokens[3];
            edges.emplace_back(a,b,cap,street);
            names.insert(street);
        }
        names.insert("-1");
        int original = 0;
        int best = 0;
        string best_exp = "";
        for(string expand: names){
            TidalFlow din(n);
            din.s = 0;
            din.t = n-1;
            for(int i = 0;i<m;i++){
                auto[a,b,cap,street] = edges[i];
                if(street == expand){
                    cap = 1000000;
                }
                din.add(a,b,cap);
            }
            int flow = din.maxFlow();
            if(expand == "-1"){
                original = flow;
            }else{
                if(flow>best){
                    best = flow;
                    best_exp = expand;
                }
            }
        }
        string ans = best_exp + " " + to_string(best-original);
        cout << ans << "\n";
        // cout << best_exp << "  \r";
        // cout << (best-original) << "\n";
    }
    return 0;
}