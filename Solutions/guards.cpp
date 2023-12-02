#include <bits/stdc++.h>
using namespace std;
#include "../Algorithms/Dinics.cpp"
#include "../Algorithms/FordFulkerson.cpp"
#include "../Algorithms/PushRelabel.cpp"
#include "../Algorithms/TidalFlow.cpp"

int main(){
    int n;
    while(cin >> n){
        if(n == 0)break;
        vector<vector<bool>> times(n,vector<bool>(48));
        vector<int> shifts(n);
        auto fillRange = [&](vector<bool> &v, vector<bool> &curr) -> void {
            for(int i = 0;i<48;i++){
                bool weGood = true;
                for(int j = i*30;j<(i+1)*30;j++){
                    if(!curr[j]){
                        weGood = false;
                        break;
                    }
                }
                v[i] = weGood;
            }
        };
        for(int i = 0;i<n;i++){
            int blocks, m;
            cin >> blocks >> m;
            shifts[i] = m;
            
            vector<bool> curr(1441);
            for(int j = 0;j<blocks;j++){
                string start, end;
                cin >> start >> end;
                int time1 = stoi(start.substr(0,2))*60+stoi(start.substr(3));
                int time2 = stoi(end.substr(0,2))*60+stoi(end.substr(3));
                if(time1 == time2){
                    time1 = 0;
                    time2 = 1440;
                }
                if(time2<time1){
                    for(int k = time1;k<=1440;k++){
                        curr[k] = true;
                    }
                    for(int k = 0;k<time2;k++){
                        curr[k] = true;
                    }
                }else{
                    for(int k = time1;k<time2;k++){
                        curr[k] = true;
                    }
                }
            }
            fillRange(times[i],curr);
        }
        // for(int i = 0;i<48;i++){
        //     if(i<10){
        //         cout << " ";
        //     }
        //     cout << i << " ";
        // }
        // cout << "\n";
        // for(int i = 0;i<n;i++){
        //     for(int j = 0;j<48;j++){
        //         cout << times[i][j] << "  ";
        //     }
        //     cout << "\n";
        // }
        int lo = 0, hi = 100;
        int ans = 0;
        for(int mid = 0;mid<=60;mid++){
            #if defined(USE_DINICS)
            Dinics flow(n+48);
            #elif defined(USE_FORD_FULKERSON)
            FordFulkerson flow(n+48);
            #elif defined(USE_PUSH_RELABEL)
            PushRelabel flow(n+48);
            #elif defined(USE_TIDAL_FLOW)
            TidalFlow flow(n+48);
            #else
            #warning No algorithm used, please select between Dinics, Ford Fulkerson, Push Relabel, and Tidal Flow
            Dinics flow(n+48);
            #endif
            for(int i = 0;i<n;i++){
                flow.add(flow.source,i,shifts[i]/30);
                for(int j = 0;j<48;j++){
                    if(times[i][j])flow.add(i,j+n,1);
                }
            }
            for(int i = 0;i<48;i++){
                flow.add(i+n,flow.sink,mid);
            }
            int currF = flow.maxFlow();
            if(currF == mid*48){
                ans = max(ans,mid);
                lo = mid+1;
            }else{
                hi = mid-1;
            }
        }
        cout << ans << "\n";

    }
}