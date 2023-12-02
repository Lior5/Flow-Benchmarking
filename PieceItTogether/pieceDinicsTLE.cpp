#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;
struct Dinics{
    int n;
    vector<vector<int>> adj;
    int source, sink;
    vector<int> levels;
    vector<int> edge_ptrs;
    int big = (int)(1e9);
    Dinics(int size){
        adj.clear();
        source = size;
        sink = size+1;
        n = size+2;
        adj = vector<vector<int>>(n,vector<int>(n));
        levels = vector<int>(n,-1);
        edge_ptrs = vector<int>(n,0);
    }
    void add(int from, int to, int capacity){
        adj[from][to] = capacity;
    }
    void build_graph(){
        queue<int> q;
        q.push(source);
        levels = vector<int>(n,-1);
        edge_ptrs = vector<int>(n,0);
        levels[source] = 0;
        while(q.size()>0){
            int curr = q.front();
            q.pop();
            for(int i = 0 ;i<n;i++){
                if(levels[i]!=-1)continue;
                if(adj[curr][i]>0){
                    levels[i] = levels[curr]+1;
                    q.push(i);
                }
            }
        }
    }
    int maxFlow(){
        int ans = 0;
        while(true){
            build_graph();
            int aug = dfs(source,big);
            if(aug==0)break;
            ans+=aug;
        }
        return ans;
    }
    int dfs(const int idx,int minFlow){
        if(idx == sink)return minFlow;
        int uniqueId = rand();
        while(edge_ptrs[idx]<n){
            int i = edge_ptrs[idx]++;
            int augmin = 0;
            if(levels[i] > levels[idx] && adj[idx][i]>0){
                augmin = dfs(i,min(minFlow,adj[idx][i]));
            }
            if(augmin>0){
                adj[idx][i]-=augmin;
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
        Dinics flow(bound);
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