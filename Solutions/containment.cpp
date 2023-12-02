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
    Dinics din(1003);
    #elif defined(USE_FORD_FULKERSON)
    FordFulkerson din(1003);
    #elif defined(USE_PUSH_RELABEL)
    PushRelabel din(1003);
    #elif defined(USE_TIDAL_FLOW)
    TidalFlow din(1003);
    #else
    #warning No algorithm used, please select between Dinics, Ford Fulkerson, Push Relabel, and Tidal Flow
    Dinics din(1003);
    #endif
    auto vt = vector<tuple<int,int,int>>();
    int source = din.source;
    int sink = din.sink;
    for(int i = 0;i<n;i++){
        int a, b, c;
        cin >> a >> b >> c;
        vt.emplace_back(a,b,c);
    }
    unordered_map<int,int> mp;
    auto inBoinds = [](int a, int b, int c){
        return (min({a,b,c})>=0 && max({a,b,c})<=9);
    };
    auto get_idx = [](int a, int b, int c){
        return a*100+b*10+c;
    };
    int cnt = 0;
    for(int i = 0;i<10;i++){
        for(int j = 0;j<10;j++){
            for(int k = 0;k<10;k++){
                if(inBoinds(i+1,j,k))din.add(get_idx(i,j,k),get_idx(i+1,j,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j+1,k))din.add(get_idx(i,j,k),get_idx(i,j+1,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j,k+1))din.add(get_idx(i,j,k),get_idx(i,j,k+1),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i-1,j,k))din.add(get_idx(i,j,k),get_idx(i-1,j,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j-1,k))din.add(get_idx(i,j,k),get_idx(i,j-1,k),1); else 
                    din.add(get_idx(i,j,k),sink,1);
                if(inBoinds(i,j,k-1))din.add(get_idx(i,j,k),get_idx(i,j,k-1),1); else 
                    din.add(get_idx(i,j,k),sink,1);
            }   
        }
    }
    // cout << "hello\n";
    for(auto tp:vt){
        auto[a,b,c] = tp;
        din.add(source,get_idx(a,b,c),1000);
    }
    int ans = din.maxFlow();
    cout << ans << "\n";

}