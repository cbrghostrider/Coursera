// -------------------------------------------------------------------------------------
//         Author: Sourabh S Joshi (cbrghostrider); Copyright - All rights reserved.
//                       For email, run on linux (perl v5.8.5):
//   perl -e 'print pack "H*","736f75726162682e732e6a6f73686940676d61696c2e636f6d0a"'
// -------------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <queue>
#include <algorithm>
#include <vector>

using std::vector;
using std::priority_queue;

template <typename T>
class CompareLesser {
public:
    bool operator () (const T& lhs, const T& rhs) {
        return (lhs > rhs);
    }
};

template <typename T>
class CompareGreater {
public:
    bool operator () (const T& lhs, const T& rhs) {
        return (lhs < rhs);
    }
};

typedef long long Elem;
typedef priority_queue<Elem, vector<Elem>, CompareLesser<Elem>>  MinHeap;
typedef priority_queue<Elem, vector<Elem>, CompareGreater<Elem>> MaxHeap;

const Elem MODNUM = 10000;

class MedianM {
public:
    MedianM(int n);
    Elem sumOfMedians();
private:
    void readInput();
    Elem findMedian();        //finds the current median
    void feedValue(Elem val); //feeds the next value into the flow
    const int N;
    vector<Elem> nums;
    MinHeap minh;             //keeps the bigger half of the elements
    MaxHeap maxh;             //keeps the smaller half of the elements
};

MedianM::MedianM(int n)
: N(n), nums(), minh(), maxh() {
    readInput();
}

void MedianM::readInput() {
    Elem elem;
    nums.reserve(N);
    for (int i=0; i<N; i++) {
        std::cin >> elem;
        nums.push_back(elem);
    }
}

Elem MedianM::findMedian() {
    if (minh.empty() && maxh.empty()) { return 0; }
    if (minh.empty() || maxh.size() > minh.size()) {return maxh.top();}
    if (maxh.empty() || minh.size() > maxh.size()) {return minh.top();}
    return maxh.top();
}

void MedianM::feedValue(Elem val) {
    if (minh.empty() && maxh.empty()) {
        minh.push(val);
        return;
    }

    if (val > minh.top()) {
        minh.push(val);
    } else {
        maxh.push(val);
    }
    if (minh.size() > (maxh.size() + 1)) {
        maxh.push(minh.top());
        minh.pop();
    } else if (maxh.size() > (minh.size() + 1)) {
        minh.push(maxh.top());
        maxh.pop();
    }
}

Elem MedianM::sumOfMedians() {
    Elem sum = 0;
    for (const Elem& e : nums) {
        feedValue(e);
        Elem median = findMedian();
        sum += median;
    }
    sum %= MODNUM;
    return sum;
}

int main() {
    MedianM mm(10000);
    Elem ans = mm.sumOfMedians();
    std::cout << ans << std::endl;
}
