#include <iostream>
#include <set>

using namespace std;
#define ll long long
#define f first
#define s second

bool matrix[505][505];

int main(){
    set<pair<int,pair<int,int>>> ss,ss2;
    int a;
    cin>>a;
    for (int i=0;i<a;i++){
        for (int j=0;j<a;j++){
            cin>>matrix[i][j];
        }
    }
    for (int i=0;i<a;i++){
        for (int j=0;j<a;j++){
            int x;
            cin>>x;
            if (x!=-1){
                int r=i,t=j;
                if (r>t) swap(r,t);
                if (matrix[i][j])
                    ss.insert({x,{r+1,t+1}});
                else
                    ss2.insert({x,{r+1,t+1}});
            }
        }
    }
    cout<<a<<" "<<ss.size()<<" "<<ss2.size()<<endl;
    for (auto i:ss){
        cout<<i.s.f<<" "<<i.s.s<<" "<<i.f<<endl;
    }
    for (auto i:ss2){
        cout<<i.s.f<<" "<<i.s.s<<" "<<i.f<<endl;
    }
}