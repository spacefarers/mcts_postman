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

#define ll long long
#define f first
#define s second

using namespace std;
using namespace chrono;

int n,mReq,mNoReq,baseCost=0;
vector<int> nodeTotCost;
double edgePromotionFactor=5,nodePromotionFactor=5;

class Edge{
public:
    int to,from,cost,id;

    Edge(){to=0,from=0,cost=0,id=-1;}

    Edge(int from,int to,int cost,int id):from(from),to(to),cost(cost),id(id){}
};

class Pattern{
public:
    Edge*from,*middle,*to;

    Pattern() = default;

    Pattern(Edge*from,Edge*middle,Edge*to):from(from),middle(middle),to(to){}

};

vector<vector<Edge*>> paths;
vector<Pattern*> allPatterns;
vector<bool> reqEdge;

vector<Edge*> currentPattern;
set<string> bestAns;
int bestCost=1e9;

class NodePatterns{
public:
    vector<Pattern*> nodePatterns;

    void intersectSet(NodePatterns inputPattern){
        vector<Pattern*> newPatterns;
        sort(inputPattern.nodePatterns.begin(),inputPattern.nodePatterns.end());
        sort(nodePatterns.begin(),nodePatterns.end());
        set_intersection(nodePatterns.begin(),nodePatterns.end(),inputPattern.nodePatterns.begin(),
                         inputPattern.nodePatterns.end(),
                         inserter(newPatterns,newPatterns.begin()));
        swap(nodePatterns,newPatterns);
    }

    void unionSet(NodePatterns inputPattern){
        vector<Pattern*> newPatterns;
        sort(inputPattern.nodePatterns.begin(),inputPattern.nodePatterns.end());
        sort(nodePatterns.begin(),nodePatterns.end());
        set_union(nodePatterns.begin(),nodePatterns.end(),inputPattern.nodePatterns.begin(),
                  inputPattern.nodePatterns.end(),
                  inserter(newPatterns,newPatterns.begin()));
        swap(nodePatterns,newPatterns);
    }

    NodePatterns(){
        nodePatterns=allPatterns;
    }
};


map<Pattern*,pair<NodePatterns,NodePatterns>> propagator;

void buildPropagator(){
    // create the propagator from patterns
    int c=0;
    for (auto*i:allPatterns){
        NodePatterns shiftOne,shiftTwo;
        shiftOne.nodePatterns.clear();
        shiftTwo.nodePatterns.clear();
        for (auto*j:allPatterns){
            if (i->middle==j->from&&i->to==j->middle){
                shiftOne.nodePatterns.push_back(j);
                c++;
                if (c%1000==0) cout<<c<<endl;
            }
//            if (i->to==j->from) shiftTwo.nodePatterns.push_back(j);
        }
        propagator[i].f=shiftOne;
        propagator[i].s=shiftTwo;
    }
}


string visualizeDebug(const vector<NodePatterns>&x){
    string ans;
    if (x.front().nodePatterns.size()==1){
        ans+=to_string(x.front().nodePatterns.front()->from->id);
        ans+=to_string(x.front().nodePatterns.front()->middle->id);
    }
    for (auto&i:x){
        if (i.nodePatterns.size()!=1) break;
        ans+=to_string(i.nodePatterns.front()->to->id);
    }
    return ans;
}

void logging();
void ending();

class MCTSNode{
public:

    vector<NodePatterns> state;
    int lowestEntropy=(int)allPatterns.size(),lowestEntropyIndex=0,excessCost=0,usedEdges=0,subMinCost=1e9,subTrials=0,fullNodes=0;
    bool isFullyExpanded=false,gameFinished=false;
    vector<MCTSNode*> children;
    MCTSNode*parent{};
    vector<int> edgeCosts,nodeCosts,edgeVisits;
	string patternString;
    MCTSNode() = default;

    explicit MCTSNode(vector<NodePatterns> state):state(state){
        edgeCosts.resize(mReq+mNoReq);
        edgeVisits.resize(mReq+mNoReq);
        nodeCosts=nodeTotCost;
        children.clear();
	    int i;
        for (i=0;state[i].nodePatterns.size()==1;i++){
            makeMove(i,state[i].nodePatterns.front());
        }
        lowestEntropyIndex=i;
//	    propagate(0);
    }

    void setParent(MCTSNode*par){
        par->children.push_back(this);
        parent=par;
    }

    void handleEdge(Edge*e){
        if (edgeCosts[e->id]==0&&reqEdge[e->id]){
            usedEdges++;
            if (--nodeCosts[e->from]==0) fullNodes++;
            if (--nodeCosts[e->to]==0) fullNodes++;
        } else {
	        excessCost+=e->cost,edgeVisits[e->id]++;
        }
	    edgeCosts[e->id]+=e->cost;
    }

    void makeMove(int pos,Pattern*p){
        state[pos].nodePatterns.clear();
        state[pos].nodePatterns.push_back(p);
        auto selectedPattern=*state[pos].nodePatterns.begin();
        if (pos==0){
            handleEdge(selectedPattern->from);
            handleEdge(selectedPattern->middle);
        }
        handleEdge(selectedPattern->to);
	    patternString=visualizeDebug(state);
    }

    void propagate(int pos){
        lowestEntropy=1e9;
        vector<pair<int,int>> propQueue;
        propQueue.emplace_back(pos,state[pos].nodePatterns.size());
        while (!propQueue.empty()){
            auto u=propQueue.back();
            propQueue.pop_back();
            int x=u.f;
            if (state[x].nodePatterns.size()!=u.s) continue;
            auto cPattern=state[x];
            for (int i=0;i<=2;i++){
                NodePatterns limitPattern;
                if (i!=0) limitPattern.nodePatterns.clear();
                for (auto&p:cPattern.nodePatterns){
                    if (i==1) limitPattern.unionSet(propagator[p].f);
                    else if (i==2) limitPattern.unionSet(propagator[p].s);
                }
                int prevSize=(int)state[x+i].nodePatterns.size();
                state[x+i].intersectSet(limitPattern);
                int afterSize=(int)state[x+i].nodePatterns.size();
                if (afterSize==1){
                	if (i!=0) makeMove(x+i,state[x+i].nodePatterns.front());
                } else if (afterSize<lowestEntropy){
                    lowestEntropy=afterSize;
                    lowestEntropyIndex=x+i;
                }
                if (prevSize!=afterSize){
                    propQueue.emplace_back(x+i,afterSize);
                }
            }
        }
        gameFinished=usedEdges==mReq;
        if (gameFinished) isFullyExpanded=true;
    }

    vector<MCTSNode*> listMoves(){
    	vector<MCTSNode*> ans;
	    for (auto i:state[lowestEntropyIndex].nodePatterns){
	    	auto *newNode=new MCTSNode(state);
	    	newNode->makeMove(newNode->lowestEntropyIndex,i);
	    	newNode->propagate(newNode->lowestEntropyIndex);
	    	if (*max_element(newNode->edgeVisits.begin(),newNode->edgeVisits.end())<=2)
	    	    ans.push_back(newNode);
	    }
	    return ans;
    }

    void expand(){
	    for (auto &i:listMoves()){
            i->setParent(this);
        }
    }

    void simulate(){
        auto theoryNode=new MCTSNode(state);
        while (!theoryNode->gameFinished){
	        vector<MCTSNode*> moves=theoryNode->listMoves();
	        if (moves.empty()){
	        	theoryNode->excessCost=1e9;
	        	break;
	        }
	        auto score=[](MCTSNode *p){
	        	// computes the score of a node state
	        	return p->usedEdges+p->fullNodes+round(10/max(double(p->excessCost),0.1));
	        };
	        sort(moves.begin(),moves.end(),[score](MCTSNode *lhs,MCTSNode *rhs){
				return score(lhs)>score(rhs);
	        });
	        theoryNode=moves.front();
        }
        if (theoryNode->excessCost<bestCost){
            bestCost=theoryNode->excessCost;
            bestAns.clear();
        }
        if (theoryNode->excessCost==bestCost){
	        bestAns.insert(visualizeDebug(theoryNode->state));
        }
        // back propagation
        MCTSNode *backTrackNode=this;
        do {
	        backTrackNode->subMinCost=min(backTrackNode->subMinCost,theoryNode->excessCost);
	        backTrackNode->subTrials++;
	        backTrackNode=backTrackNode->parent;
        } while (backTrackNode!=nullptr);
    }
}root;

void ending(){
	int c=0;
	for (auto j:root.children){
		if (j->patternString=="103"){
			cout<<c<<endl;
		}
		c++;
	}
	exit(0);
}

void createPatterns(int at){
    if (currentPattern.size()==3){
        if (currentPattern[0]->id==currentPattern[1]->id&&currentPattern[0]->id==currentPattern[2]->id) return;
        allPatterns.push_back(new Pattern(currentPattern[0],currentPattern[1],currentPattern[2]));
        return;
    }
    for (auto&i:paths[at]){
        currentPattern.push_back(i);
        createPatterns(i->to);
        currentPattern.pop_back();
    }
}


double getScore(const MCTSNode*node){
	if (node->isFullyExpanded) return -1e7;
	double exploration=sqrt(2);
	int costScore=node->subMinCost;
	if (costScore==1e9) return 1e7;
    return exploration*sqrt(log(node->parent->subTrials)/node->subTrials)+1/double(costScore);
}

bool nodeComparator(const MCTSNode*lhs,const MCTSNode*rhs){
//	if (getScore(lhs)!=1e9){
//		cout<<round(getScore(lhs))<<" vs. "<<round(getScore(rhs))<<endl;
//		cout<<round(getScore(lhs))<<" vs. "<<round(getScore(rhs))<<endl;
//		cout<<round(getScore(lhs))<<" vs. "<<round(getScore(rhs))<<endl;
//	}
    return round(getScore(lhs)*100)>round(getScore(rhs)*100);
}

vector<int> getAllScore(const vector<MCTSNode*> &v){
	vector<int> ans;
	ans.reserve(v.size());
	for (auto &i:v){
		ans.push_back((int)round(100*getScore(i)));
	}
	return ans;
}

MCTSNode*MCTSSelection(){
    MCTSNode*deepestRoot=&root;
    while (!deepestRoot->children.empty()&&!deepestRoot->isFullyExpanded){
	    for (auto &i:deepestRoot->children){
	    	if (i->listMoves().empty()) i->isFullyExpanded=true;
		    if (!i->isFullyExpanded) goto cont;
	    }
	    deepestRoot->isFullyExpanded=true;
	    MCTSSelection();
	    cont:;
        sort(deepestRoot->children.begin(),deepestRoot->children.end(),nodeComparator);
        deepestRoot=deepestRoot->children.front();
    }
    return deepestRoot;
}

int treeSize(MCTSNode*node){
    int ans=0;
    for (auto i:node->children){
        ans+=treeSize(i);
    }
    return ans+1;
}

void logging(){
    cout<<"Tree Size: "<<treeSize(&root)<<endl;
    cout<<"Current Best Answer: "<<bestCost+baseCost<<endl;
    int c=0;
	for (auto &i:bestAns){
		cout<<c++<<". "<<i<<endl;
	}
}

void MCTSSearch(){
    auto startTime=high_resolution_clock::now();
    vector<NodePatterns> grid(100);
    root=MCTSNode(grid);
    auto checkTime=[=](){
        auto currentTime=high_resolution_clock::now();
        auto duration=duration_cast<seconds>(currentTime-startTime);
        return duration.count()<20;
    };
    int c=0;
    while (checkTime()||root.isFullyExpanded){
        MCTSNode*selectedNode=MCTSSelection();
//        cout<<"Selected Node: "+visualizeDebug(selectedNode->state)<<endl;
		selectedNode->expand();
        selectedNode->simulate();
        if (c++%1==0) logging();
    }
    cout<<"All Nodes Visited"<<endl;
    ending();
}

void setIO(const string&name){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen((name+".in").c_str(),"r",stdin);
//    freopen((name+".out").c_str(),"w",stdout);
}

int main(){
    setIO("input2");
    cin>>n>>mReq>>mNoReq;
    nodeTotCost.resize(n+1);
	reqEdge.resize(mReq+mNoReq);
    paths.resize(n+1);
    for (int i=0;i<mReq+mNoReq;i++){
        int a,b,c; // inputs 1-indexed
        cin>>a>>b>>c;
        paths[a].push_back(new Edge(a,b,c,i));
        if (i<mReq){
        	nodeTotCost[a]++,nodeTotCost[b]++;
	        baseCost+=c;
        }
        paths[b].push_back(new Edge(b,a,c,i));
        reqEdge[i]=i<mReq;
//        nodeTotCost[a]++,nodeTotCost[b]++;
    }
    for (int i=1;i<=n;i++){
        createPatterns(i);
    }
    cout<<allPatterns.size()<<endl;
//	for (auto &i:allPatterns){
//		cout<<i->from->id<<i->middle->id<<i->to->id<<endl;
//	}
    buildPropagator();
    MCTSSearch();
}