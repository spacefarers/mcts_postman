//
// Created by micha on 6/7/2022.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <map>

#define ll long long
#define f first
#define s second

using namespace std;

using u32    = uint_least32_t;
using engine = std::mt19937;

int main() {
    vector<pair<int, int>> nums;
    nums.emplace_back(5, 1);
    nums.emplace_back(5, 2);
    nums.emplace_back(5, 3);
    nums.emplace_back(5, 4);
    int totalScore = 0;
    for (auto &i: nums) {
        totalScore += i.f;
    }
    sort(nums.begin(), nums.end(), greater<>());
    random_device os_seed;
    const u32 seed = os_seed();

    engine generator( seed );
    uniform_int_distribution< u32 > distribute( 1, totalScore);
    map<int,int> mp;
    int tot=200000;
    for (int j = 0; j < tot; ++j) {
        int randomItem = distribute(generator);
        int randMove = nums.front().s;
//    cout<<randomItem<<endl;
        for (auto i: nums) {
            if (randomItem - i.f < 0) {
                randMove = i.s;
                break;
            }
            randomItem -= i.f;
        }
        mp[randMove]++;
    }
    for (auto &i:mp){
        cout<<i.f<<" "<<double(i.s)/tot<<endl;
    }
}