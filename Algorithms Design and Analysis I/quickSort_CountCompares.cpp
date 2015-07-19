// -------------------------------------------------------------------------------------
//         Author: Sourabh S Joshi (cbrghostrider); Copyright - All rights reserved.
//                       For email, run on linux (perl v5.8.5):
//   perl -e 'print pack "H*","736f75726162682e732e6a6f73686940676d61696c2e636f6d0a"'
// -------------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <vector>
#include <utility>
#include <algorithm>

using std::pair;
using std::make_pair;
using std::vector;
typedef int Elem;

const int NUMS = 10000;

class QuickSort {
public:
    enum PivotType {FIRST, LAST, MEDOFTHREE};
    QuickSort(const vector<Elem>& ns, PivotType typ);
    unsigned long long numCompares();
private:
    void quickSort(int from, int to);
    int partition(const int from, const int to);

    //pivot strategies
    void pivotOnFirst(const int from, const int to);
    void pivotOnLast(const int from, const int to);
    void pivotOnMedOfThree(const int from, const int to);

    vector<Elem> nums;
    PivotType type;
    unsigned long long compares;
};

QuickSort::QuickSort(const vector<Elem>& ns, PivotType typ)
: nums(ns), type(typ), compares(0) {
}

void QuickSort::pivotOnFirst(const int from, const int to) {
    //do nothing
}

void QuickSort::pivotOnLast(const int from, const int to) {
    std::swap(nums[from], nums[to]);
}

void QuickSort::pivotOnMedOfThree(const int from, const int to) {
    int mid = (from+to)/2;
    vector<pair<int, Elem>> vals;
    vals.push_back(make_pair(from, nums[from]));
    vals.push_back(make_pair(to,   nums[to]));
    vals.push_back(make_pair(mid,  nums[mid]));
    std::sort(vals.begin(), vals.end(), [] (const pair<int, Elem>& lhs, const pair<int, Elem>& rhs) -> bool {
        return (lhs.second < rhs.second);
    });
    int pivotindex = vals[1].first;
    std::swap(nums[from], nums[pivotindex]);
}

//returns position of pivot in final place
int QuickSort::partition(const int from, const int to) {
    if (from == to) return from;

    switch (type) {
      case FIRST: {
          pivotOnFirst(from, to);
          break;
      }
      case LAST: {
          pivotOnLast(from, to);
          break;
      }
      case MEDOFTHREE: {
          pivotOnMedOfThree(from, to);
          break;
      }
    }

    int pivotInd = from;
    int i = from+1;

    for (int j=from+1; j<=to; j++) {
        if (nums[j] < nums[pivotInd]) {
            std::swap(nums[i], nums[j]);
            i++;
        }
    }
    std::swap(nums[pivotInd], nums[i-1]);
    compares += (to-from);
    return (i-1);
}

void QuickSort::quickSort(int from, int to) {
    if (from == to) return;

    int pivotPosn = partition(from, to);

    if (pivotPosn != from) {
        quickSort(from, pivotPosn-1);
    }
    if (pivotPosn != to) {
        quickSort(pivotPosn+1, to);
    }
}

unsigned long long QuickSort::numCompares() {
    quickSort(0, nums.size()-1);
    return compares;
}

int main () {
    vector<Elem> nums(NUMS, 0);
    for (int i=0; i<NUMS; i++) {
        std::cin >> nums[i];
    }
    QuickSort qs1(nums, QuickSort::FIRST);
    std::cout << qs1.numCompares() << std::endl;
    QuickSort qs2(nums, QuickSort::LAST);
    std::cout << qs2.numCompares() << std::endl;
    QuickSort qs3(nums, QuickSort::MEDOFTHREE);
    std::cout << qs3.numCompares() << std::endl;
}


