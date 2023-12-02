#include <bits/stdc++.h>
using namespace std;
#include "../Algorithms/Dinics.cpp"
#include "../Algorithms/FordFulkerson.cpp"
#include "../Algorithms/PushRelabel.cpp"
#include "../Algorithms/TidalFlow.cpp"

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
    //m*2+2
    #if defined(USE_DINICS)
    Dinics flow(m*2+2);
    #elif defined(USE_FORD_FULKERSON)
    FordFulkerson flow(m*2+2);
    #elif defined(USE_PUSH_RELABEL)
    PushRelabel flow(m*2+2);
    #elif defined(USE_TIDAL_FLOW)
    TidalFlow flow(m*2+2);
    #else
    #warning No algorithm used, please select between Dinics, Ford Fulkerson, Push Relabel, and Tidal Flow
    Dinics flow(m*2+2);
    #endif
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