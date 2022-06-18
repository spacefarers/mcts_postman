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

MCTSNode*expansion(MCTSNode*node){
    set<Node*> covered;
    for (auto i:node->children){
        covered.insert(i->lastMove());
    }
    for (auto i:node->listMoves()){
        if (covered.find(i)==covered.end()){
            auto*newNode=new MCTSNode();
            newNode->state=node->state;
            newNode->nodeReq=node->nodeReq;
            newNode->finished=node->finished;
            newNode->edgeLeft=node->edgeLeft;
            newNode->edgeReq=node->edgeReq;
            newNode->excessWeight=node->excessWeight;
            newNode->makeMove(i);
            newNode->setParent(node);
            break;
        }
    }
    return node->children.back();
}

MCTSNode*selection(MCTSNode*root){
    MCTSNode*currentDeepest=root;
    while (!currentDeepest->finished){
        if (currentDeepest->fullyExpanded()){
            currentDeepest=currentDeepest->bestChild();
        } else {
            return expansion(currentDeepest);
        }
    }
    return currentDeepest;
}


int tests=1,numThreads=1;

void simulation(MCTSNode*node){
    double randomCost=randomRoute(node, true);
    if (randomCost==0) randomCost=0.001;
    node->weight+=randomCost;
    node->trials++;
    while (node->parent!=nullptr){
        node=node->parent;
        node->weight+=randomCost;
        node->trials++;
    }
}


void  MCTSSearch(){
    cout<<"MCTS"<<endl;
    MCTSNode root;
    MCTSNode*currentNode=&root;
    while (!currentNode->finished){
        int allowed=50;
        int prevIter=iters;
        while (iters-prevIter<allowed){
            MCTSNode*selectedNode;
            selectedNode=selection(currentNode);
            simulation(selectedNode);
            iters++;
        }
        while (currentNode->children.empty()||currentNode->children.back()!=expansion(currentNode)) expansion(currentNode);
        Node*move=bestNodeYet[currentNode->state.state.size()];
        for (auto &i:currentNode->children){
            if (i->lastMove()==move){
                currentNode=i;
                goto cont;
            }
        }
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
    setIO("data/m7212");
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
    MCTSSearch();
}