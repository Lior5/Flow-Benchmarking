#include <bits/stdc++.h>
using namespace std;

struct FordFulkerson{
    
    int n;
    vector<vector<int>> adj;
    int source, sink;
    int big = (int)(1e9);
    FordFulkerson(int size) {
        adj.clear();
        n = size + 2;
        adj = vector<vector<int>>(n,vector<int>(n));
        source = size;
        sink = size+1;
    }
    void add(int from, int to, int capacity, int flow){
        adj[from][to] = capacity;
        adj[to][from] = 0;
    }
    int maxFlow(int source, int dest){
        int ans = 0;
        while(true){
            vector<bool> visited(n);
            int augpath = dfs(source,visited,big);
            ans+=augpath;
            if(augpath == 0)break;
        }
        return ans;
    }
    void printcaps(){
        for(int i = 0;i<n;i++){
            for(int j = 0;j<n;j++){
                cout << adj[i][j] << " ";
            }
            cout << "\n";
        }
    }
    int dfs(int idx, vector<bool> &v, int minFlow){
        if(idx == sink)return minFlow;
        if(v[idx])return 0;
        v[idx] = true;
        for(int i = 0;i<adj[idx].size();i++){
            int augmin = 0;
            if(!v[i] && adj[idx][i] > 0){
                augmin = dfs(i,v, min(minFlow,adj[idx][i]));
            }
            if(augmin > 0){
                adj[idx][i]-=augmin;
                // guaranteed i exist
                adj[i][idx]+=augmin;
                return augmin;
            }
        }
        return 0;
    }
};
int main(){
    int t;
    cin >> t;
    while(t-->0){
        int n;
        cin >> n;
        FordFulkerson flow(n);
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
                flow.add(flow.source,i,1,0);
            }
        }
        for(int i= 0;i<n;i++){
            if(segs[i][1] != segs[i][3]){
                flow.add(i,flow.sink,1,0);
            }
        }
        for(int i= 0;i<n;i++){
            for(int j = 0;j<n;j++){
                if(i==j)continue;
                if(segs[i][1]==segs[i][3] && segs[j][1] != segs[j][3]){
                    //first is hori second is vert
                    if(segs[i][0] <= segs[j][0] && segs[i][2] >= segs[j][0]){
                        if(segs[j][1] <= segs[i][1] && segs[j][3] >= segs[i][1]){
                            flow.add(i,j,1,0);

                        }
                    }
                }
            }
        }
        int ans = n-flow.maxFlow(flow.source,flow.sink);
        cout << ans << "\n";
    }
}