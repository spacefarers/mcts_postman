// created on 2021/7/27 by michaelyang
// problem: route inspection problem with wfc and mcts

#include <iostream>
#include <random>
#include <utility>
#include <vector>
#include <cstring>
#include <algorithm>
#include <map>
#include <set>
#include <chrono>
#include <cmath>
#include <thread>
#include <cassert>
#include <cstdlib>
#include "core.cpp"

#define ll long long
#define f first
#define s second

using namespace std;
using namespace chrono;


void randomSearch(){
    bestAns=1e9;
    int allowedIters=8000;
    MCTSNode root;
    randomRoute(&root, true);
    while (iters++<allowedIters){
        int ccost=randomRoute(&root);
//        cout<<ccost<<endl;
        if (iters%100==0) cout<<double(iters)/allowedIters*100<<"% "<<bestAns<<endl;
    }
    finalOutput.emplace_back(iters,bestAns);
    cout<<endl<<endl<<endl;
    int lasty=-1,lastx=-1;
    vector<pair<int,int>> ans;
    for (auto &i:finalOutput){
        int x=i.f,y=i.s;
        if (lasty!=-1&&lastx!=x-1){
            ans.emplace_back(x-1,lasty);
        }
        ans.emplace_back(x,y);
        lasty=y;
        lastx=x;
    }
    for (auto i:ans){
        cout<<i.f<<"    "<<i.s<<endl;
    }
}


int main(){
    setIO("data/hd4110");
//    setIO("input.in");
    cin>>n>>mReq>>mNoReq;
    nodes.resize(n);
    dist.resize(n);
    for (auto&i:dist){
        for (int j=0;j<n;j++){
            i.emplace_back(1e9,-1);
        }
    }
    for (int i=0;i<n;i++){
        nodes[i]=new Node(0,i);
        dist[i][i]={0,-1};
    }
    for (int i=0;i<mReq+mNoReq;i++){
        int a,b,c,d; // inputs 1-indexed
        cin>>a>>b>>c>>d;
        dist[a-1][b-1]={c,b-1};
        dist[b-1][a-1]={d,a-1};
        Node*Na=nodes[a-1],*Nb=nodes[b-1];
        Edge*newEdge=new Edge(Na,Nb,c,d,i,i<mReq);
        edges.push_back(newEdge);
        paths[Na].push_back(Nb);
        paths[Nb].push_back(Na);
        lookup[{Na,Nb}]=newEdge;
        lookup[{Nb,Na}]=newEdge;
        if (i<mReq){
            baseCost+=min(c,d);
            nodeVals[Na]++;
            nodeVals[Nb]++;
        }
    }
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            for (int k=0;k<n;k++){
                if (dist[j][i].f+dist[i][k].f<dist[j][k].f){
                    dist[j][k].f=dist[j][i].f+dist[i][k].f;
                    dist[j][k].s=dist[j][i].s;
                }
            }
        }
    }
    randomSearch();
}