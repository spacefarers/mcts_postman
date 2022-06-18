// created on 9/12/21 by spacefarers
// problem: makeGraph

#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

#define ll long long
#define f first
#define s second

using namespace std;

int main(){
    int lasty=-1,lastx=-1;
    vector<pair<int,int>> ans;
    while (true){
        int x,y;
        cin>>x;
         if (x==-1) break;
         cin>>y;
         if (lasty!=-1&&lastx!=x-1){
             ans.emplace_back(x-1,lasty);
         }
         ans.emplace_back(x,y);
         lasty=y;
         lastx=x;
     }
    for (auto i:ans){
        cout<<i.f<<" "<<i.s<<endl;
    }
}
