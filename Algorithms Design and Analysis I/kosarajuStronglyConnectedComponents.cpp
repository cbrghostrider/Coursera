// -------------------------------------------------------------------------------------
//         Author: Sourabh S Joshi (cbrghostrider); Copyright - All rights reserved.
//                       For email, run on linux (perl v5.8.5):
//   perl -e 'print pack "H*","736f75726162682e732e6a6f73686940676d61696c2e636f6d0a"'
// -------------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <utility>

using std::pair;
using std::make_pair;
using std::vector;
using std::unordered_set;
using std::map;

typedef unordered_set<int> Bucket;
typedef vector<Bucket> AdjList;

class Kosaraju {
public:
    Kosaraju(int nverts);
    void runKosaraju();
private:
    enum PassType {PASS1, PASS2};

    void readInput();
    void clearMarks();
    void dfs(int start);
    void dfsLoop();

    const int NVERTS;
    AdjList graph;
    AdjList revgraph;
    PassType pass;
    vector<bool> marks;
    int time = 0;
    int source = 0;
    vector<pair<int, int>> times;  //(node name, finish time)
    vector<int> leaders; 
};

Kosaraju::Kosaraju(int nverts)
: NVERTS(nverts), graph(NVERTS, Bucket()), revgraph(NVERTS, Bucket()), 
  pass(PASS1), marks(NVERTS, false), time(0), source(0), 
  times(NVERTS, make_pair(0, 0)), leaders(NVERTS, 0) {
    readInput();
}

void Kosaraju::readInput() {
    int from, to;
    while (std::cin >> from >> to) {
        from -= 1; to -= 1; //make 0-indexed
        graph[from].insert(to);
        revgraph[to].insert(from);
    }
    for (int i=0; i<NVERTS; i++) {
        times[i] = make_pair(i, 0);
    }
}

void Kosaraju::clearMarks() {
    for (int i=0; i<NVERTS; i++) {
        marks[i] = false;
    }
}

void Kosaraju::dfsLoop() {
    for (int ti=0; ti<times.size(); ti++) {
        int node = times[ti].first;
        if (marks[node] == false) {
            if (pass == PASS2) {
                source = node;
            }
            dfs(node);
        }
    }
}

void Kosaraju::dfs(int node) {
    marks[node] = true;

    if (pass == PASS2) {
        leaders[node] = source;
    }

    const Bucket& mybucket = (pass == PASS1) ? revgraph[node] : graph[node];
    for (const int& neigh : mybucket) {
        if (marks[neigh] == false) {
            dfs(neigh);
        }
    }

    if (pass == PASS1) {
        time++;
        times[node].second = time;
    }
}

void Kosaraju::runKosaraju() {
    //run pass 1 of Kosaraju (reverse DFS)
    pass = PASS1;
    dfsLoop();

    //clear bookkeeping and prep for pass 2
    clearMarks();
    std::sort(times.begin(), times.end(), [] (const pair<int, int>& lhs, const pair<int, int>& rhs) -> bool {
        return (lhs.second > rhs.second);
    });

    //run pass 2 of kosaraju
    pass = PASS2;
    dfsLoop();

    //compute the answers needed
    map<int, int> countLeaders;
    vector<int> counts;
    for (const int& leader : leaders) {
        auto it = countLeaders.find(leader);
        if (it == countLeaders.end()) {
            countLeaders.insert(make_pair(leader, 1));
        } else {
            countLeaders[leader]++;
        }
    }
    for (const pair<int, int>& lc : countLeaders) {
        counts.push_back(lc.second);
    }
    std::sort(counts.begin(), counts.end(), std::greater<int>());

    std::cout << "Number of connected components: " << countLeaders.size() << std::endl;
    for (const int& cc : counts) {
        std::cout << cc << ",";
    }
    std::cout << std::endl;
}

//class assignment has 875714 vertices... change to parse this from file as enhancement later
const int nvertices = 875714; 

int main() {
    Kosaraju kosaraju(nvertices);
    kosaraju.runKosaraju();
}


