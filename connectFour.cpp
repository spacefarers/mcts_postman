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

#define ll long long
#define f first
#define s second

using namespace std;

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
    int wins=0,trials=0;
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
}rootPlayer1,rootPlayer2;

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

void debug(convMCTSNode*node){
    map<int,pair<double,double>> mp;
    for (auto i:node->children){
        mp[i->state.lastMove]={double(i->trials)/node->trials,double(i->wins)/i->trials};
    }
    for (int i=0;i<7;i++){
        cout<<"Move: "<<i<<" Trial: "<<mp[i].f*100<<"%; Win: "<<mp[i].s*100<<"%"<<endl;
    }
}

int main(){
    ConvMCTS sims(true),sims2(false);
    convMCTSNode currentPosition;
    convMCTSNode*player1Pos=&rootPlayer1,*player2Pos=&rootPlayer2;
    bool firstTurn=true;
    int allowedIters=1000;
    int iterations;
    while (currentPosition.state.gameFinished==-10){
        if (firstTurn){
            iterations=0;
            while (iterations++<allowedIters){
                auto selectedNode=sims.selection(player1Pos);
                sims.simulation(selectedNode);
                auto selectedNode2=sims2.selection(player2Pos);
                sims2.simulation(selectedNode2);
            }
            debug(player1Pos);
            player1Pos=sims.bestChild(player1Pos);
            int move=player1Pos->state.lastMove;
            currentPosition.state.makeMove(move);
            player2Pos=player2Pos->makeMove(move);
        } else {
            iterations=0;
            while (iterations++<allowedIters){
                auto selectedNode=sims.selection(player1Pos);
                sims.simulation(selectedNode);
                auto selectedNode2=sims2.selection(player2Pos);
                sims2.simulation(selectedNode2);
            }
            debug(player2Pos);
            player2Pos=sims2.bestChild(player2Pos);
            int move=player2Pos->state.lastMove;
            currentPosition.state.makeMove(move);
            player1Pos=player1Pos->makeMove(move);
        }
        currentPosition.state.print();
        firstTurn=!firstTurn;
    }
    cout<<"Finished"<<endl;
}