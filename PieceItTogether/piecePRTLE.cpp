#include <bits/stdc++.h>
#include <pthread.h>
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
        PushRelabel flow(bound);
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