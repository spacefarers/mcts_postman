// created on 2021/8/6 by spacefarers

#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <random>
#include <iterator>
#include <set>
#include <map>
#include <chrono>
#include <thread>

#define ll long long
#define f first
#define s second

using namespace std;
using namespace chrono;

template<typename Iter,typename RandomGenerator>
Iter select_randomly(Iter start,Iter end,RandomGenerator&g){
    std::uniform_int_distribution<> dis(0,std::distance(start,end)-1);
    std::advance(start,dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start,Iter end){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start,end,gen);
}

class ConnectFourBoard{
public:
    int board[7][6]{},track[7]{};
    int moveSide=1;
    int gameFinished=-10;
    int lastMove=-1;

    ConnectFourBoard() = default;

    ConnectFourBoard(ConnectFourBoard const&newBoard){
        for (int i=0;i<7;i++){
            for (int j=0;j<6;j++){
                board[i][j]=newBoard.board[i][j];
            }
            track[i]=newBoard.track[i];
        }
        gameFinished=newBoard.gameFinished;
        moveSide=newBoard.moveSide;
    }

    void makeMove(int pos){
        board[pos][track[pos]++]=moveSide;
        lastMove=pos;
        int u=0,d=0,l=0,r=0,ul=0,ur=0,dl=0,dr=0;
        int y=track[pos];
        while (y<6&&board[pos][y++]==moveSide) u++;
        y=track[pos]-2;
        while (y>=0&&board[pos][y--]==moveSide) d++;
        int x=pos+1;
        while (x<7&&board[x++][track[pos]-1]==moveSide) r++;
        x=pos-1;
        while (x>=0&&board[x--][track[pos]-1]==moveSide) l++;
        x=pos+1,y=track[pos];
        while (x<7&&y<6&&board[x++][y++]==moveSide) ur++;
        x=pos-1,y=track[pos];
        while (x>=0&&y<6&&board[x--][y++]==moveSide) ul++;
        x=pos+1,y=track[pos]-2;
        while (x<7&&y>=0&&board[x++][y--]==moveSide) dr++;
        x=pos-1,y=track[pos]-2;
        while (x>=0&&y>=0&&board[x--][y--]==moveSide) dl++;
        if (u+d>=3||r+l>=3||ur+dl>=3||ul+dr>=3) gameFinished=moveSide;
        for (int i:track){
            if (i!=6) goto cont;
        }
        gameFinished=0;
        cont:;
        moveSide=moveSide==-1?1:-1;
    }

    vector<int> listMoves(){
        vector<int> ans;
        for (int i=0;i<7;i++){
            if (track[i]<6) ans.push_back(i);
        }
        return ans;
    }

    void print(){
        for (int i=5;i>=0;i--){
            for (int j=0;j<7;j++){
                string output;
                switch (board[j][i]){
                    case 1:
                        output='X';
                        break;
                    case -1:
                        output='O';
                        break;
                    case 0:
                        output='.';
                        break;
                }
                cout<<output<<" ";
            }
            cout<<endl;
        }
        cout<<"-----------------"<<endl;
    }
};

typedef ConnectFourBoard boardState;

class convMCTSNode{
public:
    convMCTSNode() = default;

    boardState state;
    convMCTSNode*parent{};
    vector<convMCTSNode*> children;
    double wins=0,trials=0;
    bool isFullyExpanded=false;

    void setParent(convMCTSNode*node){
        node->children.push_back(this);
        parent=node;
    }

    convMCTSNode*makeMove(int move){
        for (auto i:children){
            if (i->state.lastMove==move){
                return i;
            }
        }
        return new convMCTSNode;
    }

    bool fullyExpanded(){
        return children.size()==state.listMoves().size();
    }
};

class ConvMCTS{
public:
    double explorationFactor=sqrt(2);
    bool firstMove;

    explicit ConvMCTS(bool firstMove):firstMove(firstMove){}

    convMCTSNode*bestChild(convMCTSNode*node){
        auto nodeScore=[this](convMCTSNode*node){
            if (node->trials==0) return 1e9;
            double score=double(node->wins)/node->trials+
                         explorationFactor*sqrt(log(node->parent->trials)/node->trials);
            return score;
        };
        auto sortNodes=[this,nodeScore](convMCTSNode*lhs,convMCTSNode*rhs){
//            if ((lhs->state.moveSide==-1)^firstMove) return nodeScore(lhs)<nodeScore(rhs);
            return nodeScore(lhs)>nodeScore(rhs);
        };
        sort(node->children.begin(),node->children.end(),sortNodes);
        return node->children.front();
    }

    convMCTSNode*selection(convMCTSNode*treeRoot){
        auto currentDeepestNode=treeRoot;
        while (currentDeepestNode->state.gameFinished==-10){
            if (currentDeepestNode->fullyExpanded()){
                currentDeepestNode=bestChild(currentDeepestNode);
            } else {
                return expansion(currentDeepestNode);
            }
        }
        return currentDeepestNode;
    }

    convMCTSNode*expansion(convMCTSNode*node){
        set<int> moves;
        for (auto j:node->children){
            moves.insert(j->state.lastMove);
        }
        for (auto i:node->state.listMoves()){
            if (moves.find(i)!=moves.end()) continue;
            auto newNode=new convMCTSNode();
            newNode->state=boardState(node->state);
            newNode->state.makeMove(i);
            newNode->setParent(node);
            break;
        }
        return node->children.back();
    }


    void backPropagation(convMCTSNode*node,int reward) const{
        while (true){
            node->wins+=reward;
            reward*=-1;
            node->trials++;
            if (node->parent==nullptr) break;
            node=node->parent;
        }
    }

    void simulation(convMCTSNode*node) const{
        convMCTSNode theoryBoard;
        theoryBoard.state=node->state;
        while (theoryBoard.state.gameFinished==-10){
            vector<int> moves=theoryBoard.state.listMoves();
            if (moves.empty()) break;
            int randomSelectedMove=*select_randomly(moves.begin(),moves.end());
            theoryBoard.state.makeMove(randomSelectedMove);
        }
        int score=theoryBoard.state.gameFinished;
        if (firstMove) score=-score;
        backPropagation(node,score);
    }
};

class wfcMCTS{
public:
    double explorationFactor=sqrt(2);
    bool firstMove;

    explicit wfcMCTS(bool firstMove):firstMove(firstMove){}

    convMCTSNode*bestChild(convMCTSNode*node){
        auto nodeScore=[this](convMCTSNode*node){
            if (node->trials==0) return 1e9;
            double score=double(node->wins)/node->trials+
                         explorationFactor*sqrt(log(node->parent->trials)/node->trials);
            return score;
        };
        auto sortNodes=[this,nodeScore](convMCTSNode*lhs,convMCTSNode*rhs){
//            if ((lhs->state.moveSide==-1)^firstMove) return nodeScore(lhs)<nodeScore(rhs);
            return nodeScore(lhs)>nodeScore(rhs);
        };
        sort(node->children.begin(),node->children.end(),sortNodes);
        return node->children.front();
    }

    convMCTSNode*selection(convMCTSNode*treeRoot){
        auto currentDeepestNode=treeRoot;
        while (currentDeepestNode->state.gameFinished==-10){
            if (currentDeepestNode->fullyExpanded()){
                currentDeepestNode=bestChild(currentDeepestNode);
            } else {
                return expansion(currentDeepestNode);
            }
        }
        return currentDeepestNode;
    }

    convMCTSNode*expansion(convMCTSNode*node){
        set<int> moves;
        for (auto j:node->children){
            moves.insert(j->state.lastMove);
        }
        for (auto i:node->state.listMoves()){
            if (moves.find(i)!=moves.end()) continue;
            auto newNode=new convMCTSNode();
            newNode->state=boardState(node->state);
            newNode->state.makeMove(i);
            newNode->setParent(node);
            break;
        }
        return node->children.back();
    }



    void propagation(convMCTSNode*node,double reward,convMCTSNode*from,int vertDist,int horizDist) const{
        if (node->parent!=nullptr&&node->parent!=from){
            node->parent->trials++;
            node->parent->wins-=reward;
            propagation(node->parent,-reward,node,vertDist+1,horizDist);
        }
        return;
        double factor=pow(0.5,horizDist);
        double adjReward=factor*reward;
        double adjTrials=factor;
        if (abs(adjReward)<=0.01&&abs(adjTrials)<=0.01) return;
        for (auto i:node->children){
            if (i!=from){
                i->wins-=adjReward;
                i->trials+=adjTrials;
                propagation(i,-reward,node,vertDist-1,horizDist+1);
            }
        }
    }

    void simulation(convMCTSNode*node) const{
        convMCTSNode theoryBoard;
        theoryBoard.state=node->state;
        while (theoryBoard.state.gameFinished==-10){
            vector<int> moves=theoryBoard.state.listMoves();
            if (moves.empty()) break;
            int randomSelectedMove=*select_randomly(moves.begin(),moves.end());
            theoryBoard.state.makeMove(randomSelectedMove);
        }
        int score=theoryBoard.state.gameFinished;
        if (firstMove) score=-score;
        node->trials++;
        node->wins+=score;
        propagation(node,score,node,0,0);
    }
};

void debug(convMCTSNode*node){
    map<int,pair<double,double>> mp;
    for (auto i:node->children){
        mp[i->state.lastMove]={double(i->trials)/node->trials,double(i->wins)/i->trials};
    }
    for (int i=0;i<7;i++){
        cout<<"Move: "<<i<<" Trial: "<<mp[i].f*100<<"%; Win: "<<mp[i].s*100<<"%"<<endl;
    }
}

void clearMemory(convMCTSNode*node){
    for (auto i:node->children){
        clearMemory(i);
    }
    delete node;
}


int test=1200;
int numThreads=4;
int totTest=test;
int player1win=0,player2win=0,draw=0;
auto beginTime=high_resolution_clock::now();

void runTest(int tests){
    int allowedIters=2000;
    while (tests--){
        ConvMCTS sims(true);
        ConvMCTS sims2(false);
        convMCTSNode currentPosition;
        auto *player1Pos=new convMCTSNode(),*player2Pos=new convMCTSNode;
        bool firstTurn=true;
        int allowedTime=1;

        auto startTime=high_resolution_clock::now();
        auto checkTime=[allowedTime](time_point<high_resolution_clock> startTime){
            auto currentTime=high_resolution_clock::now();
            auto duration=duration_cast<seconds>(currentTime-startTime);
            return duration.count()<allowedTime;
        };
        auto limitTimeRun=[&startTime,checkTime,&sims,&sims2,&player1Pos,&player2Pos](){
            startTime=high_resolution_clock::now();
            while (checkTime(startTime)){
                auto selectedNode=sims.selection(player1Pos);
                sims.simulation(selectedNode);
            }
            startTime=high_resolution_clock::now();
            while (checkTime(startTime)){
                auto selectedNode2=sims2.selection(player2Pos);
                sims2.simulation(selectedNode2);
            }
        };
        auto limitIterRun=[allowedIters,&sims,&sims2,&player1Pos,&player2Pos](){
            auto startTime=high_resolution_clock::now();
            int iter=0;
            while (iter++<allowedIters){
                auto selectedNode=sims.selection(player1Pos);
                sims.simulation(selectedNode);
            }
            auto currentTime=high_resolution_clock::now();
            auto duration=duration_cast<milliseconds>(currentTime-startTime);
//            cout<<"WFC Time: "<<duration.count()<<endl;
            startTime=high_resolution_clock::now();
            iter=0;
            while (iter++<allowedIters){
                auto selectedNode2=sims2.selection(player2Pos);
                sims2.simulation(selectedNode2);
            }
            currentTime=high_resolution_clock::now();
            duration=duration_cast<milliseconds>(currentTime-startTime);
//            cout<<"Vanilla Time: "<<duration.count()<<endl;
        };
        while (currentPosition.state.gameFinished==-10){
            if (firstTurn){
                limitIterRun();
                //                debug(player1Pos);
                player1Pos=sims.bestChild(player1Pos);
                int move=player1Pos->state.lastMove;
                currentPosition.state.makeMove(move);
                player2Pos=player2Pos->makeMove(move);
            } else {
                limitIterRun();
                //                debug(player2Pos);
                player2Pos=sims2.bestChild(player2Pos);
                int move=player2Pos->state.lastMove;
                currentPosition.state.makeMove(move);
                player1Pos=player1Pos->makeMove(move);
            }
            if (player1Pos->parent!=nullptr) player1Pos->parent->parent=nullptr;
            if (player2Pos->parent!=nullptr) player2Pos->parent->parent=nullptr;
//            currentPosition.state.print();
            firstTurn=!firstTurn;
        }
        if (currentPosition.state.gameFinished==1){
//            cout<<"Player 1 Won!"<<endl;
            player1win++;
        } else if (currentPosition.state.gameFinished==-1){
//            cout<<"Player 2 Won!"<<endl;
            player2win++;
        } else {
//            cout<<"Draw!"<<endl;
            draw++;
        }
        //        clearMemory(player1Pos);
        //        clearMemory(player2Pos);
        auto currentTime=high_resolution_clock::now();
        auto duration=duration_cast<seconds>(currentTime-beginTime);
        test--;
        cout<<"Est. "<<(double(duration.count())/(totTest-test))*test/60<<" mins left"<<endl;
        cout<<"Player 1 wins: "<<player1win<<" "<<double(player1win)/(totTest-test)*100<<"%"<<endl;
        cout<<"Player 2 wins: "<<player2win<<" "<<double(player2win)/(totTest-test)*100<<"%"<<endl;
        cout<<"Draws: "<<draw<<" "<<double(draw)/(totTest-test)*100<<"%"<<endl;
        cout<<"Tests Left: "<<test<<endl;
        cout<<"WFC(1,0) vs Vanilla"<<endl;
    }
}

int main(){
    vector<thread> threads;
    int avg=test/numThreads,left=test%numThreads;
    threads.reserve(numThreads);
    for (int i=0;i<numThreads;i++){
        threads.emplace_back(runTest,avg+(left-->0?1:0));
    }
    for (auto &i:threads){
        i.join();
    }
}