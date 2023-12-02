#include <bits/stdc++.h>
#include <pthread.h>
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
    void add(int from, int to, int capacity){
        adj[from][to] = capacity;
        adj[to][from] = 0;
    }
    int maxFlow(){
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
        FordFulkerson flow(bound);
        int source = flow.source;
        int sink = flow.sink;
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