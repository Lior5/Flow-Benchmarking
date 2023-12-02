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
    while(cin >> n){
        if(n == 0)break;
        vector<vector<bool>> times(n,vector<bool>(48));
        vector<int> shifts(n);
        auto fillRange = [&](vector<bool> &v, vector<bool> &curr) -> void {
            for(int i = 0;i<48;i++){
                bool weGood = true;
                for(int j = i*30;j<(i+1)*30;j++){
                    if(!curr[j]){
                        weGood = false;
                        break;
                    }
                }
                v[i] = weGood;
            }
        };
        for(int i = 0;i<n;i++){
            int blocks, m;
            cin >> blocks >> m;
            shifts[i] = m;
            
            vector<bool> curr(1441);
            for(int j = 0;j<blocks;j++){
                string start, end;
                cin >> start >> end;
                int time1 = stoi(start.substr(0,2))*60+stoi(start.substr(3));
                int time2 = stoi(end.substr(0,2))*60+stoi(end.substr(3));
                if(time1 == time2){
                    time1 = 0;
                    time2 = 1440;
                }
                if(time2<time1){
                    for(int k = time1;k<=1440;k++){
                        curr[k] = true;
                    }
                    for(int k = 0;k<time2;k++){
                        curr[k] = true;
                    }
                }else{
                    for(int k = time1;k<time2;k++){
                        curr[k] = true;
                    }
                }
            }
            fillRange(times[i],curr);
        }
        // for(int i = 0;i<48;i++){
        //     if(i<10){
        //         cout << " ";
        //     }
        //     cout << i << " ";
        // }
        // cout << "\n";
        // for(int i = 0;i<n;i++){
        //     for(int j = 0;j<48;j++){
        //         cout << times[i][j] << "  ";
        //     }
        //     cout << "\n";
        // }
        int lo = 0, hi = 100;
        int ans = 0;
        for(int mid = 0;mid<=60;mid++){
            TidalFlow flow(n+48);
            for(int i = 0;i<n;i++){
                flow.add(flow.source,i,shifts[i]/30);
                for(int j = 0;j<48;j++){
                    if(times[i][j])flow.add(i,j+n,1);
                }
            }
            for(int i = 0;i<48;i++){
                flow.add(i+n,flow.sink,mid);
            }
            int currF = flow.maxFlow();
            if(currF == mid*48){
                ans = max(ans,mid);
                lo = mid+1;
            }else{
                hi = mid-1;
            }
        }
        cout << ans << "\n";

    }
}