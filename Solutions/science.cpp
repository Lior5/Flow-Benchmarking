#include <bits/stdc++.h>
using namespace std;
#define ALGO Dinics
#if defined(USE_DINICS)
#define ALGO Dinics
#elif defined(USE_FORD_FULKERSON)
#define ALGO FordFulkerson
#elif defined(USE_PUSH_RELABEL)
#define ALGO PushRelabel
#elif defined(USE_TIDAL_FLOW)
#define ALGO TidalFlow
#endif
#include "../Algorithms/Dinics.cpp"
#include "../Algorithms/FordFulkerson.cpp"
#include "../Algorithms/PushRelabel.cpp"
#include "../Algorithms/TidalFlow.cpp"

int main(){
    #if defined(USE_TIDAL_FLOW)
    #warning Tidal flow is not supported for this problem yet
    return 0;
    #else
    int n;
    while(true){
        cin >> n;
        if(n==0)break;
        int size = n*2;
        vector<vector<bool>> valid(n,vector<bool>(n));
        for(int i = 0;i<n;i++){
            string s;
            cin >> s;
            for(int j = 0;j<n;j++){
                valid[i][j] = (s[j] == 'Y'); 	
            }
        }
        auto buildgraph = [&](int k) -> ALGO {
            ALGO flow(size);
            for(int i = 0;i<n;i++){
                flow.add(flow.source,i,k);
                for(int j = 0;j<n;j++){
                    if(valid[i][j])
                        flow.add(i,j+n,1);
                }
                flow.add(i+n,flow.sink,k);
            }
            return flow;
        };
        int orderings = 0;
        for(int k = 1;k<=n;k++){
            ALGO flow = buildgraph(k);
            if(flow.maxFlow() >= k*n) orderings = k;
            else break;
        }
        vector<vector<int>> ans(orderings);
        for(int k = orderings-1;k>=0;k--){
            ALGO currFlow = buildgraph(k+1);
            // have to set valid to only the grid cells taken by currFlow
            int currmax = currFlow.maxFlow();
            for(int i = 0;i<n;i++){
                for(int j = 0;j<n;j++){
                    if(currFlow.indices[i].count(j+n) && valid[i][j]){
                        if(currFlow.adj[i][currFlow.indices[i][j+n]].second > 0){ 
                            valid[i][j] = false;
                        }
                    }
                }
            }
            ALGO nextFlow = buildgraph(k);
            int nextmax = nextFlow.maxFlow();
            // cout << k << "\n";
            // cout << currmax << " " << nextmax << "\n";
            for(int j = 0;j<n;j++){
                int asd =0;
                for(int i = 0;i<n;i++){
                    if(currFlow.indices[i].count(j+n) && nextFlow.indices[i].count(j+n)){
                        if(currFlow.adj[i][currFlow.indices[i][j+n]].second != 
                            nextFlow.adj[i][nextFlow.indices[i][j+n]].second){
                                valid[i][j] = false;
                                // cout << j << " ";
                                ans[k].push_back(i+1);
                                asd++;
                        }
                    }
                }
                // cout << "ASD " << asd << "\n";
            }
        }
        cout << orderings << "\n";
        for(auto row: ans){
            for(int in:row){
                cout << in << " ";
            }
            cout << "\n";
        }
    }
    #endif
}