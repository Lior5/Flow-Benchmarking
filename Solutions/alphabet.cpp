#include <bits/stdc++.h>
using namespace std;
#include "../Algorithms/Dinics.cpp"
#include "../Algorithms/FordFulkerson.cpp"
#include "../Algorithms/PushRelabel.cpp"
#include "../Algorithms/TidalFlow.cpp"
int main(){
    int n;
    cin >> n;
    while(n-->0){
        #if defined(USE_DINICS)
        Dinics flow(520);
        #elif defined(USE_FORD_FULKERSON)
        FordFulkerson flow(520);
        #elif defined(USE_PUSH_RELABEL)
        PushRelabel flow(520);
        #elif defined(USE_TIDAL_FLOW)
        TidalFlow flow(520);
        #else
        #warning No algorithm used, please select between Dinics, Ford Fulkerson, Push Relabel, and Tidal Flow
        Dinics flow(520);
        #endif
        string a, b;
        cin >> a >> b;
        for(int i = 0;i<a.length();i++){
            flow.add(flow.source,i,1);
            for(int j = 0;j<b.length();j++){
                if(abs(a[i]-b[j])>=3){
                    flow.add(i,j+a.length(),1);
                }
            }
        }
        for(int i = 0;i<b.length();i++){
            flow.add(i+a.length(),flow.sink,1);
        }
        int ans = flow.maxFlow();
        cout << ans << "\n";
    }
}