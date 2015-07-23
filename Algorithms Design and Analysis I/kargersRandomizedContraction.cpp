// -------------------------------------------------------------------------------------
//         Author: Sourabh S Joshi (cbrghostrider); Copyright - All rights reserved.
//                       For email, run on linux (perl v5.8.5):
//   perl -e 'print pack "H*","736f75726162682e732e6a6f73686940676d61696c2e636f6d0a"'
// -------------------------------------------------------------------------------------
//
// Randomized Contraction Algorithm (a.k.a. Karger's Algorithm) 
// to find the min-cut of a given graph
//
#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <unordered_set>
#include <string>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <limits>

using std::pair;
using std::make_pair;
using std::vector;
using std::unordered_map;
using std::unordered_multiset;
using std::string;
using std::istringstream;

typedef unsigned int Node;
typedef pair<Node, Node> Edge;
typedef unordered_multiset<Node> Bucket;

class MinCut{
public:
    MinCut();
    int runKargers();
private:
    void readInput();
    int numIterations();
    int mincutVal();
    Edge pickEdge(const unordered_map<Node, Bucket>& myAdjList);
    int numNodes;
    unordered_map<Node, Bucket> adjList;
};

MinCut::MinCut()
: numNodes(0), adjList() {
    readInput();
}

void MinCut::readInput() {
    string line;
    while (std::getline(std::cin, line)) {
        istringstream iss(line);

        Node nd; iss >> nd; 
        adjList.insert(make_pair(nd, Bucket()));

        Node neighbor;
        while (iss >> neighbor) {
            adjList[nd].insert(neighbor);
        }
    }
}

int MinCut::numIterations() {
    //(n^2 log n) number of iterations for (1/n) probability of finding the mincut
    return (numNodes * numNodes * (std::log(numNodes))); 
}

Edge MinCut::pickEdge(const unordered_map<Node, Bucket>& myAdjList) {
    //pick node
    assert(myAdjList.size() > 0);
    int alIndex = std::rand() % (myAdjList.size());
    auto alIt = myAdjList.begin();
    for (int i=0; i<alIndex; i++) {alIt++;}

    //pick outgoing edge from this node
    int nedges = (alIt->second).size();
    assert(nedges > 0);
    int eIndex = std::rand() % nedges;
    auto ndIt = (alIt->second).begin();
    for (int i=0; i<eIndex; i++) {ndIt++;}

    Edge ret = make_pair(alIt->first, *ndIt);
    return ret;
}

int MinCut::mincutVal() {
    unordered_map<Node, Bucket> myAdjList = adjList;

    while (myAdjList.size() > 2) {
        Edge chosen = pickEdge(myAdjList);

        myAdjList[chosen.first].erase(chosen.first);
        myAdjList[chosen.first].erase(chosen.second);
        myAdjList[chosen.second].erase(chosen.first);
        myAdjList[chosen.second].erase(chosen.second);

        //going to merge chosen.second into chosen.first
        //and adjust adjacency lists of everyone affected
        for (const int& nd : myAdjList[chosen.second]) {
            myAdjList[nd].erase(chosen.second);
            myAdjList[nd].insert(chosen.first);
            myAdjList[chosen.first].insert(nd);
        }

        //remove second node, since it was merged into the first
        myAdjList.erase(chosen.second);
    }

    assert(myAdjList.size() == 2);
    auto fit = myAdjList.begin();
    auto sit = fit; sit++; 
    assert((fit->second).size() == (sit->second).size());

    return ((fit->second).size());
}

int MinCut::runKargers() {
    int minval = std::numeric_limits<int>::max();
    for (int i=0; i<numIterations(); i++) {
        int mincut = mincutVal();
        if (mincut < minval) {
            minval = mincut;
            std::cout << "Iteration # " << i << " ; Found mincut = " << mincut << " (New mincut!)" << std::endl;;
        }
    }
    return minval;
}

int main() {
    MinCut mc;
    int ans = mc.runKargers();
    std::cout << ans << std::endl;
}


