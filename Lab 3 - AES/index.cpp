#include <iostream>
#include <vector>
#include <unordered_set>
#include <list>
#include <math.h>
using namespace std;

int calculateCacheHitPercentage(int X, const vector<int>& requests) {
    if (X <= 0 || requests.empty()) return 0.0;

    unordered_set<int> cache; // To store the pages in the cache
    list<int> lruQueue;       // To maintain the order of LRU
    int hits = 0;

    for (int request : requests) {
        cout << "Requesting page: " << request << endl;

        if (cache.find(request) != cache.end()) {
            hits++;
            lruQueue.remove(request);
        } else {
            if (cache.size() == X) {
                int leastRecentlyUsed = lruQueue.back();
                lruQueue.pop_back();
                cache.erase(leastRecentlyUsed);
            }
            cache.insert(request);
        }
        lruQueue.push_front(request); // Add the current request to the front of the LRU queue

        cout << "Cache status: ";
        for (int page : lruQueue) {
            cout << page << " ";
        }
        cout << endl;
    }

    return (int)(floor((static_cast<double>(hits) / requests.size()) * 100.0));
}

int main() {
    int cacheSize = 3;
    vector<int> pageRequests = {1, 2, 3, 4, 2, 1, 3, 4, 5, 1};

    double hitPercentage = calculateCacheHitPercentage(cacheSize, pageRequests);
    cout << "Cache hit percentage: " << hitPercentage << "%" << endl;

    return 0;
}
