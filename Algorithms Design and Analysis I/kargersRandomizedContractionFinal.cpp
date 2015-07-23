// -------------------------------------------------------------------------------------
//         Author: Sourabh S Joshi (cbrghostrider); Copyright - All rights reserved.
//                       For email, run on linux (perl v5.8.5):
//   perl -e 'print pack "H*","736f75726162682e732e6a6f73686940676d61696c2e636f6d0a"'
// -------------------------------------------------------------------------------------
//
// Randomized Contraction Algorithm (a.k.a. Karger's Algorithm) to find the min-cut of a given graph
//
// This version corrects the uniform randomization problem that the previous version manifested, in the function pickEdge()
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
    Edge pickEdge(const unordered_map<Node, Bucket>& myAdjList, int numEdges);
    int numNodes;
    int totalEdges;
    unordered_map<Node, Bucket> adjList;
};

MinCut::MinCut()
: numNodes(0), totalEdges(0), adjList() {
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
            if (nd < neighbor) {
                totalEdges++;
            }
        }
    }
    numNodes = adjList.size();
}

int MinCut::numIterations() {
    //(n^2 log n) number of iterations for (1/n) probability of finding the mincut
    return (numNodes * numNodes * (std::log(numNodes))); 
}

// Note: Uniformly random now!
Edge MinCut::pickEdge(const unordered_map<Node, Bucket>& myAdjList, int numEdges) {

    int randMod = numEdges * 2; //each edge remaining shows up twice
    int picked = (std::rand() % randMod);
    int from = -1, to = -1;

    int alistInd = 0;

    for (const pair<Node, Bucket>& ndal : myAdjList) {
        if (picked < ndal.second.size()) {
            from = ndal.first;
            auto ndIt = ndal.second.begin();
            for (int i=0; i<picked; i++) ndIt++;
            to = *ndIt;
            break;
        } else {
            picked -= ndal.second.size();
        }
    }

    Edge ret = make_pair(from, to);
    return ret;
}

int MinCut::mincutVal() {
    unordered_map<Node, Bucket> myAdjList = adjList;
    int numEdges = totalEdges;

    while (myAdjList.size() > 2) {
        Edge chosen = pickEdge(myAdjList, numEdges);

        numEdges -= myAdjList[chosen.first].count(chosen.second);

        myAdjList[chosen.first].erase(chosen.second);
        myAdjList[chosen.second].erase(chosen.first);

        //going to merge chosen.second into chosen.first
        //and adjust adjacency lists of everyone affected
        for (const int& nd : myAdjList[chosen.second]) {
            myAdjList[nd].erase(myAdjList[nd].find(chosen.second));
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



