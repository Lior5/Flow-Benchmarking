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
    int n, m;
    cin >> n >>m;
    vector<vector<int>> board(n,vector<int>(m));
    for(int i = 0;i<n;i++){
        for(int j = 0;j<m;j++){
            cin >> board[i][j];
        }
    }
    for(int outer = 0;outer<n;outer++){
        PushRelabel flow(n+m);
        for(int i = 0;i<n;i++){
            int cap = 1;
            if(i == outer)cap = 100000;
            flow.add(flow.source,i,cap);
            for(int j =0;j<m;j++){
                if(board[i][j])flow.add(i,j+n,1);
                flow.add(j+n,flow.sink,1);
            }
        }
        int ans = flow.maxFlow();
        if(ans<m){
            int num = -3;
            cout << "bleh\n";
            for(int i = 0;i<m;i++){
                cout << flow.adj[i+n][flow.sink] << "\n";
                if(flow.adj[i+n][flow.sink] == 0){
                    num =i+1;
                    break;
                }
            }
            cout << "0\n";
            cout << num << " ";
            int counter = 0;
            for(int i = 0;i<m;i++){
                if(i+1==num){
                    continue;
                }
                if(counter++ == n-1)break;
                cout << (i+1) << " ";
            }
            return 0;
        }
        
        cout << "1\n";
    }
}