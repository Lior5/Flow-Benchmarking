#include <bits/stdc++.h>
using namespace std;

struct PushRelabel{
    int big = (int)(1e9);
    int n;
    vector<vector<int>> adj;
    int source, sink;
    queue<int> excess_vertices;
    vector<int> height, excess;
    PushRelabel(int size){
        n = size+2;
        source = size;
        sink = size+1;
        adj = vector<vector<int>>(n,vector<int>(n));
        height = vector<int>(n);
        excess = vector<int>(n);
    }
    void add(int from, int to, int cap){
        adj[from][to] += cap;
    }
    void push(int from, int to){
        int d = min(excess[from], adj[from][to]);
        d = max(d,0);
        adj[from][to] -=d;
        adj[to][from]+=d;
        excess[from] -=d;
        excess[to] += d;
        return;
    }
    bool discharge(int idx) {
        bool good = false;
        for(int to = 0;to<n;to++){
            if(adj[idx][to] <= 0)continue;
            if(height[idx] > height[to]) {
                push(idx, to);
                good = true;
            }
        }
        return good;
    }
    void relabel(int idx){
        int mn = big;
        for(int i = 0;i<n;i++){
            if(adj[idx][i]==0)continue;
            mn = min(mn,height[i]);
        }
        if(mn!=big){
            height[idx] = mn+1;
        }
    }
    int maxFlow(){
        int sum = 0;
        for(int i = 0;i<n;i++){
            sum += adj[source][i];
        }
        excess[source] = sum;
        height[source] = n;
        for(int i = 0;i<n;i++){
            if(adj[source][i]>0)
                push(source,i);
        }
        while(true){
            bool found = false;
            for(int i = 0;i<n-2;i++){
                if(excess[i]>0){
                    if(!discharge(i)){
                        relabel(i);
                    }
                    found = true;
                }
            }
            if(!found)break;
        }
        return excess[sink];
    }
};
int main(){
    int t;
    cin >> t;
    while(t-->0){
        int n;
        cin >> n;
        PushRelabel flow(n);
        vector<vector<int>> segs(n,vector<int>(4));
        for(int i = 0;i<n;i++){
            for(int j = 0;j<4;j++){
                cin >> segs[i][j];
            }
            if(segs[i][2]<segs[i][0]){
                swap(segs[i][2],segs[i][0]);
            }
            if(segs[i][3] < segs[i][1]){
                swap(segs[i][3],segs[i][1]);
            }
        }
        for(int i= 0;i<n;i++){
            if(segs[i][1] == segs[i][3]){
                flow.add(flow.source,i,1);
            }
        }
        for(int i= 0;i<n;i++){
            if(segs[i][1] != segs[i][3]){
                flow.add(i,flow.sink,1);
            }
        }
        for(int i= 0;i<n;i++){
            for(int j = 0;j<n;j++){
                if(i==j)continue;
                if(segs[i][1]==segs[i][3] && segs[j][1] != segs[j][3]){
                    //first is hori second is vert
                    if(segs[i][0] <= segs[j][0] && segs[i][2] >= segs[j][0]){
                        if(segs[j][1] <= segs[i][1] && segs[j][3] >= segs[i][1]){
                            flow.add(i,j,1);

                        }
                    }
                }
            }
        }
        int ans = n-flow.maxFlow();
        cout << ans << "\n";
    }
}