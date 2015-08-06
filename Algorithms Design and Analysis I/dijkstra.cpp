// -------------------------------------------------------------------------------------
//         Author: Sourabh S Joshi (cbrghostrider); Copyright - All rights reserved.
//                       For email, run on linux (perl v5.8.5):
//   perl -e 'print pack "H*","736f75726162682e732e6a6f73686940676d61696c2e636f6d0a"'
// -------------------------------------------------------------------------------------
//
// Note: This is a modified version of my submission from the HackerRank/Algorithms/Graph section
//       Basically I have modified the input function, to conform to the assignment data, and,
//       modified the startnode, and the targets (both are 0-indexed)
//
#include <iostream>
#include <cstdio>
#include <vector>
#include <set>
#include <utility>
#include <functional>
#include <queue>
#include <sstream>
#include <string>

using std::priority_queue;
using std::pair;
using std::make_pair;
using std::vector;
using std::set;
using std::string;
using std::istringstream;

typedef unsigned int Node;
typedef int Weight;

//////////////////////////////////////////////////////////////////////////////
//                              assignment data
//////////////////////////////////////////////////////////////////////////////
const Node NUMNODES = 200; //for assignment
const Node STARTNODE = 0;
const vector<Node> targets = {6,36,58,81,98,114,132,164,187,196};
//const Node NUMNODES = 50; //for assignment
//const Node STARTNODE = 28;
//const vector<Node> targets = {6};
//////////////////////////////////////////////////////////////////////////////

struct Edge {
    Node to;
    Weight weight;
    Edge(Node t, Weight w) : to(t), weight(w) {}
};

typedef pair<Node, Edge> QuElem;

class Dijkstra {
public:
    Dijkstra();
    void printShortestPaths();
private:
    void readInput();
    void runDijkstra();
    Node startNode;
    vector<vector<Edge> > adjList;
    vector<Weight> pathLens;
};

Dijkstra::Dijkstra()
: startNode(0), adjList(), pathLens() {
    readInput();
}

void Dijkstra::readInput() {
    int n = NUMNODES; 
    adjList = vector<vector<Edge> >(n, vector<Edge>());
    pathLens = vector<Weight>(n, -1);

    //read information for all the nodes
    for (int i=0; i<n; i++) {
        string str;
        getline(std::cin, str);
        istringstream ss(str);

        Node start; ss >> start; 
        start -= 1;
        string ipstr;

        while (ss >> ipstr) {
            size_t splitat = ipstr.find_first_of(',');
            string endstr = ipstr.substr(0, splitat);
            string wtstr = ipstr.substr(splitat+1);
            Node end = std::stoi(endstr);
            Weight wt = std::stoi(wtstr);
            end -= 1;
            adjList[start].push_back(Edge(end, wt));
        }
    }

    startNode = STARTNODE;
}

void Dijkstra::runDijkstra() {
    //compare function for minimum heap
    std::function<bool (const QuElem&, const QuElem& )> HeapCompare = 
      [this] (const QuElem& lhs, const QuElem& rhs) -> bool {
          return ((pathLens[lhs.first]+lhs.second.weight) > (pathLens[rhs.first]+rhs.second.weight));
      };

    //the minimum heap for edges on the frontier
    priority_queue<QuElem, vector<QuElem>, decltype(HeapCompare)> frontier(HeapCompare);

    //to mark which ones are already done
    vector<bool> processed(adjList.size(), false);

    //initialize starting conditions
    pathLens[startNode] = 0;
    processed[startNode] = true;
    for (const Edge& edge : adjList[startNode]) frontier.push(make_pair(startNode, edge));

    bool done = false;
    
    while (!done && !frontier.empty()) {
        QuElem cand = frontier.top(); frontier.pop();
        while (processed[cand.second.to]) {
            //because C++ priority_queue has no API to delete non-min element, I am forced to do this
            if (frontier.empty()) {done = true; break;}
            cand = frontier.top(); frontier.pop();
        }
        if (done) break;

        //this node now has its shortest path determined
        Node absorb = cand.second.to;
        pathLens[absorb] = pathLens[cand.first] + cand.second.weight;
        processed[absorb] = true;

        //add this node's edges to frontier
        for (const Edge& edge : adjList[absorb]) {
            frontier.push(make_pair(absorb, edge));
        }
    }
}

void Dijkstra::printShortestPaths() {
    //run dijkstra's shortest paths algorithm
    runDijkstra();

    //and print the results
    for (int i=0; i<targets.size(); i++) {
        std::cout << pathLens[targets[i]] << " ";
    }
    std::cout << std::endl;
}

int main() {
    Dijkstra dijkstra;
    dijkstra.printShortestPaths();
}
