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
using u32    = uint_least32_t;
using engine = std::mt19937;

struct compareNodes{
    bool operator()(const MCTSNode*lhs,const MCTSNode*rhs) const {
        return lhs->nodeScore>rhs->nodeScore;
    }
};

set<MCTSNode*,compareNodes> bestNodes;


void uniformAdvance(){
    cout<<"Uniform Advance"<<endl;
    MCTSNode root;
    MCTSNode*currentNode=&root;
    int allowedIters=50;
    while (!currentNode->finished){
        auto comparator=[](const pair<int,MCTSNode*>& lhs,const pair<int,MCTSNode*>& rhs){
            return lhs.f<rhs.f;
        };
        for (auto i:currentNode->listMoves()){
            auto*newNode=new MCTSNode();
            newNode->state=currentNode->state;
            newNode->nodeReq=currentNode->nodeReq;
            newNode->finished=currentNode->finished;
            newNode->edgeLeft=currentNode->edgeLeft;
            newNode->edgeReq=currentNode->edgeReq;
            newNode->excessWeight=currentNode->excessWeight;
            newNode->makeMove(i);
            newNode->setParent(currentNode);
            newNode->dep=currentNode->dep+1;
        }
        int itersCount=allowedIters;
        randomRoute(currentNode,true);
        while (itersCount>1){
            itersCount--;
            iters++;
            int ccost=randomRoute(currentNode);
        }
        Node*move=bestNodeYet[currentNode->state.state.size()];
        for (auto &i:currentNode->children){
            if (i->lastMove()==move){
                currentNode=i;
                goto cont;
            }
        }
        cout<<"ERR"<<endl;
        break;
        cont:;
        currentNode->parent=nullptr;
        currentDepth++;
        currentNode->state.print();
        cout<<"Current Cost: "<<currentNode->excessWeight+baseCost<<endl;
        cout<<"Edges Left: "<<currentNode->edgeLeft<<endl;
        cout<<"Iterations: "<<iters<<endl;
        cout<<"Best Route Yet: "<<bestAns<<endl;
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
    srand(time(nullptr));
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
    uniformAdvance();
}