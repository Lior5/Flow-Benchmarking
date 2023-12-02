#include <bits/stdc++.h>
using namespace std;
#include "../Algorithms/Dinics.cpp"
#include "../Algorithms/FordFulkerson.cpp"
#include "../Algorithms/PushRelabel.cpp"
#include "../Algorithms/TidalFlow.cpp"

int main(){
    int n;
    cin >> n;
    #if defined(USE_DINICS)
    Dinics flow(n);
    #elif defined(USE_FORD_FULKERSON)
    FordFulkerson flow(n);
    #elif defined(USE_PUSH_RELABEL)
    PushRelabel flow(n);
    #elif defined(USE_TIDAL_FLOW)
    TidalFlow flow(n);
    #else
    #warning No algorithm used, please select between Dinics, Ford Fulkerson, Push Relabel, and Tidal Flow
    Dinics flow(n);
    #endif
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