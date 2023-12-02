#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define sz(x) (int)(x).size()
typedef vector<int> vi;
typedef long long ll;

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
        adj[from][to] += capacity;
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

int main(){
    int n, m;
    cin >> n >> m;
    vector<int> inspections(n);
    vector<vector<int>> adj(n,vector<int>(n));
    vector<vector<int>> direct(n,vector<int>(n));
    for(int i = 0;i<n;i++){
        cin >> inspections[i];
    }
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
			int x;
			cin >> x;
            adj[i][j] = direct[i][j]=x+inspections[j];
        }
    }
	for(int i = 0;i<n;i++)adj[i][i] = 0;
	for(int k = 0;k<n;k++){
		for(int i = 0;i<n;i++){
			for(int j = 0;j<n;j++){
				adj[i][j] = min(adj[i][j],adj[i][k]+adj[k][j]);
			}
		}
	}
    vector<vector<int>> flights(3,vector<int>(m));
    for(int i = 0;i<m;i++){
        for(int j = 0;j<3;j++){
            cin >> flights[j][i];
        }
        flights[0][i]--;
        flights[1][i]--;
    }
    Dinics flow(m*2+2);
    int source = flow.source;
    int sink = flow.sink;
    for(int i = 0;i<m;i++){
        flow.add(source,i,1);
        int time = flights[2][i]+direct[flights[0][i]][flights[1][i]];
        int location = flights[1][i]; 
        for(int j = 0;j<m;j++){
			if(i==j)continue;
            int by = flights[2][j];
			int newTime = time+adj[location][flights[0][j]];
            if(newTime <= by){
                flow.add(i,j+m,1);
            }
        }
        flow.add(i+m,sink,1);
    }
    cout << m - flow.maxFlow() << "\n";

    
}