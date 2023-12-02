#include <bits/stdc++.h>
using namespace std;
#include "../Algorithms/Dinics.cpp"
#include "../Algorithms/FordFulkerson.cpp"
#include "../Algorithms/PushRelabel.cpp"
#include "../Algorithms/TidalFlow.cpp"


int main(){
    int n, m;
    cout << "waiting input\n";
    cin >> n >>m;
    cout << "took in input\n";
    vector<vector<int>> board(n,vector<int>(m));
    for(int i = 0;i<n;i++){
        for(int j = 0;j<m;j++){
            cin >> board[i][j];
        }
    }
    cout << "hi\n";
    for(int outer = 0;outer<n;outer++){
        #if defined(USE_DINICS)
        Dinics flow(n+m+2);
        #elif defined(USE_FORD_FULKERSON)
        FordFulkerson flow(n+m+2);
        #elif defined(USE_PUSH_RELABEL)
        PushRelabel flow(n+m+2);
        // #warning Push Relabel is acting very wonky on this problem for some reason, terminating.
        // return 0;
        #elif defined(USE_TIDAL_FLOW)
        TidalFlow flow(n+m+2);
        #else
        #warning No algorithm used, please select between Dinics, Ford Fulkerson, Push Relabel, and Tidal Flow
        Dinics flow(n+m+2);
        #endif
        int source = flow.source;
        int sink = flow.sink;
        cout << "hi\n";
        for(int i = 0;i<m;i++){
            flow.add(i+n,sink,1);
        }
        for(int i = 0;i<n;i++){
            int cap = 1;
            if(i == outer)cap = 100000;
            flow.add(source,i,cap);
            for(int j =0;j<m;j++){
                if(board[i][j])flow.add(i,j+n,1);
            }
        }
        int ans = flow.maxFlow();
        // TODO IMPLEMENT LEFT OF MIN CUT ON MY FLOW ALGOS
        // not really necessary for benchmarking the flow algo's runtime however
        // if(ans<m){
        //     cout << "0\n";
        //     vector<bool> used(m);
        //     int counter = 0;
        //     for(int i = 0;i<m;i++){
        //         if(!flow.leftOfMinCut(i+n)){
        //             used[i] = true;
        //             counter++;
        //         }
        //     }
        //     for(int i = 0;i<m;i++){
        //         if(counter < n && flow.leftOfMinCut(i+n)){
        //             used[i] = true;
        //             counter++;
        //         }
        //     }
        //     for(int i = 0;i<m;i++){
        //         if(used[i]){
        //             cout << (i+1) << " ";
        //         }
        //     }
        //     return 0;
        // }
    }
    cout << "finished1\n";
    return 0;
}