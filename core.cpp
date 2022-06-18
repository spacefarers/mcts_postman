//
// Created by michael on 6/17/2022.
//


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

#define ll long long
#define f first
#define s second

using namespace std;
using u32 = uint_least32_t;
using engine = std::mt19937;
u32 seed;

int baseCost = 0;
int n, mReq, mNoReq;
vector<vector<pair<int, int>>> dist;
double bestAns = 1e9;


int iters = 0;

vector<pair<int, int>> finalOutput;

void setIO(const string &name) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen((name).c_str(), "r", stdin);
//    freopen((name+".out").c_str(),"w",stdout);
}


class Node {
public:
    int cost, id;
    bool req{};

    map<set<int>, pair<int, vector<Node *>>> lowestCosts;

    Node(int cost, int id) : cost(cost), id(id) {}
};


vector<Node *> nodes;


map<Node *, vector<Node *>> paths;


vector<Node *> bestNodeYet;

class Edge {
public:
    Node *to{}, *from{};
    int costForw, costBack, id, costTravel = 0;
    bool req;

    Edge() { costForw = 1e9, costBack = 1e9, id = -1, req = false; }

    Edge(Node *from, Node *to, int costForw, int costBack, int id, bool req) : from(from), to(to), costForw(costForw),
                                                                               costBack(costBack), id(id), req(req) {}
};

vector<Edge *> edges;
map<pair<Node *, Node *>, Edge *> lookup;
map<Node *, int> nodeVals;
int currentDepth=0;

int random(int min, int max) {
    random_device os_seed;
    seed = os_seed();
    engine generator(seed);
    uniform_int_distribution<u32> distribute(min, max);
    return (int) distribute(generator);
}

class State {
public:
    vector<Node *> state;

    void addNode(Node *node) {
        state.push_back(node);
    }

    Node *back() { return state.back(); }

    Edge *lastEdge() {
        return lookup[{state[state.size() - 2], state.back()}];
    }

    void print() {
        for (auto i: state) {
            cout << i->id << " ";
        }
        cout << endl;
    }
};

double sigmoid(int x) {
    return 1 / double(1 + exp(-x));
}

class MCTSNode{
public:
    State state;
    bool finished=false;
    int dep;
    double nodeScore=0;
    MCTSNode(){
        edgeLeft=mReq;
        nodeReq=nodeVals;
        dep=0;
    };
    int edgeLeft,nonReqCons=0;
    map<Node*,int> nodeReq;
    map<int,int> edgeReq;
    vector<MCTSNode*> children;
    double trials=0,weight=0;
    int excessWeight=0;
    MCTSNode*parent{};
    int lastMoveRec=-1;

    vector<Node*> listMoves(){
        if (state.state.empty()){
            return nodes;
        }
        vector<Node*> ans;
        for (auto i:paths[state.state.back()]){
//            if (edgeReq[lookup[{state.state.back(),i}]]<3)
            ans.push_back(i);
        }
        return ans;
    }

    void makeMove(Node*newNode){
        state.addNode(newNode);
        Edge*madeEdge=state.lastEdge();
        if (state.state.size()>1){
            if (edgeReq[madeEdge->id]++==0&&madeEdge->req){
                nodeReq[madeEdge->from]--;
                nodeReq[madeEdge->to]--;
                if (--edgeLeft==0){
                    finished=true;
                }
                nonReqCons=0;
                excessWeight-=min(madeEdge->costForw,madeEdge->costBack);
            } else {
                nonReqCons++;
                madeEdge->costTravel++;
            }
            auto cNode=state.state[state.state.size()-2];
            excessWeight+=dist[cNode->id][newNode->id].f;
        }
    }

    bool fullyExpanded(){
        return children.size()==listMoves().size();
    }


    MCTSNode *bestChild() {
        int minEdge = 1e9, maxRep = 0;
        for (auto &i: children) {
            minEdge = min(minEdge, i->edgeLeft);
            maxRep = max(maxRep, i->edgeReq[i->lastMove()->id]);
        }
        auto score = [minEdge, maxRep](MCTSNode *node) {
            double explorationFactor = sqrt(2) / 2;
            if (node->trials == 0) return 1e9;
            double exploitation = -double(node->weight) / (node->trials * (bestAns - baseCost));
            double exploration = explorationFactor * sqrt(log(node->parent->trials) / node->trials);
            double score = -exploitation + exploration;
            score -= (node->edgeLeft - minEdge);
            score -= double(node->edgeReq[node->lastMove()->id]) / maxRep;
            return score;
        };
        auto compareNodes = [score](MCTSNode *lhs, MCTSNode *rhs) {
            return score(lhs) > score(rhs);
        };
        sort(children.begin(), children.end(), compareNodes);
        return children.front();
    }


    Node*lastMove(){
        lastMoveRec=state.back()->id;
        return state.back();
    }

    void setParent(MCTSNode*node){
        node->children.push_back(this);
        parent=node;
    }
    double score(){
        double exploration=10*sqrt(2);
        if (this->trials==0) this->trials=0.1;
        if (this->parent==nullptr) return -1e9;
        double score=double(bestAns*this->trials-this->weight)/this->trials+
                     exploration*sqrt(log(this->parent->trials)/this->trials)-pow(this->dep-currentDepth,2);
        this->nodeScore=score;
        return score;
    }
    bool operator>(MCTSNode*node){
        return this->score()>node->score();
    }
    bool operator==(const MCTSNode*node) const{
        return this==node;
    }
};

int randomRoute(MCTSNode *node, bool chooseFirst = false) {
//    int minAll=1e9;
//    for (int _=0;_<5;_++){
    auto *theoryNode = new MCTSNode(*node);
    while (!theoryNode->finished && theoryNode->excessWeight < 1000000) {
        if (theoryNode->nonReqCons > 8) {
            int bestNode = -1, bestCost = 1e9;
            int cNode = theoryNode->state.back()->id;
            for (int i = 0; i < n; i++) {
                if (theoryNode->nodeReq[nodes[i]] > 0 && dist[cNode][i].f < bestCost) {
                    bestCost = dist[cNode][i].f;
                    bestNode = i;
                }
            }
            assert(bestNode != -1);
            if (bestNode != theoryNode->lastMove()->id) {
                while (cNode != bestNode) {
                    cNode = dist[cNode][bestNode].s;
                    theoryNode->makeMove(nodes[cNode]);
                }
            }
            theoryNode->nonReqCons = 0;
        }
        auto scoreMove = [&](Node *node) {
            if (theoryNode->state.state.empty()) return 1.;
            int alpha = 1;
            Edge *edge = lookup[{theoryNode->lastMove(), node}];
            int cost = ((node == edge->to) ? edge->costForw : edge->costBack) + edge->costTravel * 2;
            double score = pow(100 / double(cost), alpha);
            if (theoryNode->nodeReq[edge->to] > 0) score = 100;
            if (edge->req && theoryNode->edgeReq[edge->id] == 0) score = 1000;
            return score;
        };
        vector<pair<double, Node *>> moves;
        int totalScore = 0;
        for (auto i: theoryNode->listMoves()) {
            moves.emplace_back(scoreMove(i), i);
            totalScore += moves.back().f;
        }
        sort(moves.begin(), moves.end(), greater<>());
        double randomItem = random(1, totalScore);
        Node *randMove = moves.front().s;
        for (auto i: moves) {
            if (randomItem - i.f < 0) {
                randMove = i.s;
                break;
            }
            randomItem -= i.f;
        }
        if (chooseFirst) theoryNode->makeMove(moves.front().s);
        else theoryNode->makeMove(randMove);
    }
    int weightTot = theoryNode->excessWeight;
    vector<Node *> lastNodes;
    set<int> edgesUnfulfilled;
    int leftVals = weightTot, rightVals = 0;
    bool extraPrec = false;
    while (extraPrec && theoryNode->state.state.size() != node->state.state.size() &&
           theoryNode->state.state.size() > 1) {
        lastNodes.push_back(theoryNode->state.state.back());
        theoryNode->state.state.pop_back();
        Edge *currentEdge = lookup[{theoryNode->state.state.back(), lastNodes.back()}];
        int edgeCost = dist[theoryNode->state.state.back()->id][lastNodes.back()->id].f;
        if (currentEdge->req && theoryNode->edgeReq[currentEdge->id] == 1)
            edgeCost -= min(currentEdge->costBack, currentEdge->costForw);
        leftVals -= edgeCost;
        Node *focusNode = lastNodes.back();
        int prevLow = focusNode->lowestCosts[edgesUnfulfilled].f;
        if (prevLow == 0) prevLow = 1e9;
        if (rightVals < prevLow) {
            focusNode->lowestCosts[edgesUnfulfilled] = {rightVals, lastNodes};
        }
        rightVals = focusNode->lowestCosts[edgesUnfulfilled].f;
        lastNodes = focusNode->lowestCosts[edgesUnfulfilled].s;
        rightVals += edgeCost;
        if (currentEdge->req && theoryNode->edgeReq[currentEdge->id]-- == 1) {
            edgesUnfulfilled.insert(currentEdge->id);
        }
    }
    weightTot = leftVals + rightVals;
    while (!theoryNode->state.state.empty()) {
        lastNodes.push_back(theoryNode->state.state.back());
        theoryNode->state.state.pop_back();
    }
    reverse(lastNodes.begin(), lastNodes.end());
    if (baseCost + weightTot < bestAns) {
        bestAns = baseCost + weightTot;
        bestNodeYet = lastNodes;
        finalOutput.emplace_back(iters, bestAns);
    }
    return weightTot;
}