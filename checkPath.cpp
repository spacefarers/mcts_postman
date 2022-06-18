// created on 9/7/21 by spacefarers
// problem: Checking Path if valid

#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

#define ll long long
#define f first
#define s second

using namespace std;

void setIO(const string&name){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen((name).c_str(),"r",stdin);
//    freopen((name+".out").c_str(),"w",stdout);
}

pair<int,int> dist[1005][1005];
bool req[1005];

int main(){
    memset(dist,0x3f,sizeof dist);
//    int n;
//    cin>>n;
    vector<int> chain;
    while (true){
        int x;
        cin>>x;
        if (x==-1) break;
        x++;
        chain.push_back(x);
    }
//    int n=chain.size();
    setIO("data/M5104");
    int n,mReq,mNoReq;
    cin>>n>>mReq>>mNoReq;
    for (int i=0;i<mReq+mNoReq;i++){
        if (i<mReq) req[i]=true;
        int a,b,c,d;
        cin>>a>>b>>c>>d;
        dist[a][b]={c,i};
        dist[b][a]={d,i};
    }
    for (int i=1;i<=n;i++){
        for (int j=1;j<=n;j++){
            for (int k=1;k<=n;k++){
                if (dist[j][i].f+dist[i][k].f<dist[j][k].f){
                    dist[j][k].f=dist[j][i].f+dist[i][k].f;
                }
            }
        }
    }
    int totCost=0;
    for (int i=0;i<chain.size()-1;i++){
        int from=chain[i],to=chain[i+1];
        if (dist[from][to].f>1e8){
            goto fail;
        }
        totCost+=dist[from][to].f;
        req[dist[from][to].s]=false;
    }
    for (int i=0;i<mReq;i++){
        if (req[i]) goto fail;
    }
    cout<<"Success: "<<totCost<<endl;
    exit(0);
    fail:;
    cout<<"Path Failed"<<endl;
}
