// -------------------------------------------------------------------------------------
//         Author: Sourabh S Joshi (cbrghostrider); Copyright - All rights reserved.
//                       For email, run on linux (perl v5.8.5):
//   perl -e 'print pack "H*","736f75726162682e732e6a6f73686940676d61696c2e636f6d0a"'
// -------------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>

using std::unordered_set;
using std::unordered_map;
using std::pair;
using std::make_pair;
using std::vector;

class TwoSum {
public:
    TwoSum(int n, long long lo, long long hi);
    int countTargets();
private:
    void readInput();
    void findTarget(long long target);
    const int N;
    const long long LO;
    const long long HI;
    unordered_map<long long, int> numCounts;
    unordered_set<long long> targets;
};

TwoSum::TwoSum(int n, long long lo, long long hi) 
: N(n), LO(lo), HI(hi), numCounts(), targets() {
    readInput();
}

void TwoSum::readInput() {
    long long val;
    for (int i=0; i<N; i++) {
        std::cin >> val;
        auto fit = numCounts.find(val);
        if (fit == numCounts.end()) {
            numCounts.insert(make_pair(val, 1));
        } else {
            numCounts[val]++;
        }
    }
}

void TwoSum::findTarget(long long target) {
    for (const pair<long long, int>& nc : numCounts) {
        long long findMe = target - nc.first;
        if (findMe == nc.first && nc.second == 1) continue;
        auto fit = numCounts.find(findMe);
        if (fit == numCounts.end()) continue;
        targets.insert(target);
        return;
    }
}

int TwoSum::countTargets() {
    for (long long i=LO; i<=HI; i++) {
        findTarget(i);
    }
    return targets.size();
}

int main() {
    TwoSum ts(1000000, -10000, 10000);
    int ans = ts.countTargets();
    std::cout << ans << std::endl;
}


