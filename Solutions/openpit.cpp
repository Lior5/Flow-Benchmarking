#include <bits/stdc++.h>
using namespace std;
#include "../Algorithms/Dinics.cpp"
#include "../Algorithms/FordFulkerson.cpp"
#include "../Algorithms/PushRelabel.cpp"
#include "../Algorithms/TidalFlow.cpp"

int main(){
    int n;
    cin >> n;
    vector<int> value(n);
    vector<int> cost(n);
    vector<vector<int>> depend(n);
    int sumV = 0, sumG = 0;
    for(int i = 0;i<n;i++){
        cin >> value[i] >> cost[i];
        sumV+=value[i];
        int howMany;
        cin >> howMany;
        for(int j = 0;j<howMany;j++){
            int curr;
            cin >> curr;
            depend[i].push_back(curr-1);
        }
    }
    #if defined(USE_DINICS)
    Dinics flow(n+2);
    #elif defined(USE_FORD_FULKERSON)
    FordFulkerson flow(n+2);
    #elif defined(USE_PUSH_RELABEL)
    PushRelabel flow(n+2);
    #elif defined(USE_TIDAL_FLOW)
    TidalFlow flow(n+2);
    #else
    #warning No algorithm used, please select between Dinics, Ford Fulkerson, Push Relabel, and Tidal Flow
    Dinics flow(n+2);
    #endif
    int source = flow.source;
    int sink = flow.sink;
    for(int i = 0;i<n;i++){
        flow.add(source,i,value[i]);
        flow.add(i,sink,cost[i]);
        for(int need:depend[i]){
            flow.add(need,i,100000);
        }
    }
    cout << sumV - flow.maxFlow() << "\n";
}