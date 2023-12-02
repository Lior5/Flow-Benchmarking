#include <bits/stdc++.h>
using namespace std;
#include "../Algorithms/Dinics.cpp"
#include "../Algorithms/FordFulkerson.cpp"
#include "../Algorithms/PushRelabel.cpp"
#include "../Algorithms/TidalFlow.cpp"
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int t;
    cin >> t;
    while(t-->0){
        int n, m;
        cin >> n >> m;
        string trash;
        getline(cin,trash);
        unordered_set<string> names;
        vector<tuple<int,int,int,string>> edges;
        for(int i = 0;i<m;i++){
            string s;
            getline(cin,s);
            vector<string> tokens = split(s, ',');
            int a = stoi(tokens[0]);
            int b = stoi(tokens[1]);
            int cap = stoi(tokens[2]);
            string street = tokens[3];
            edges.emplace_back(a,b,cap,street);
            names.insert(street);
        }
        names.insert("-1");
        int original = 0;
        int best = 0;
        string best_exp = "";
        for(string expand: names){
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
            flow.source = 0;
            flow.sink = n-1;
            for(int i = 0;i<m;i++){
                auto[a,b,cap,street] = edges[i];
                if(street == expand){
                    cap = 1000000;
                }
                flow.add(a,b,cap);
            }
            int val = flow.maxFlow();
            if(expand == "-1"){
                original = val;
            }else{
                if(val>best){
                    best = val;
                    best_exp = expand;
                }
            }
        }
        string ans = best_exp + " " + to_string(best-original);
        cout << ans << "\n";
        // cout << best_exp << "  \r";
        // cout << (best-original) << "\n";
    }
    return 0;
}