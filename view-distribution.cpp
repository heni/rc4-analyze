#include "rc4.h"
#include <iomanip>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <random>
#include <string>
#include <cassert>
using namespace std;

class OneThreadTest {
    static mutex OutMutex;
    vector<vector<size_t>> StatTable;
    size_t ThreadId;
    size_t Count;
public:
    OneThreadTest(size_t id, size_t count) 
        : StatTable(256, vector<size_t>(256, 0))
        , ThreadId(id)
        , Count(count)
    {}

    void operator()() {
        TRC4State state;
        vector<unsigned char> items(256);
        InitializeRandomState(state, default_random_engine(ThreadId * Count + 1));
        for (size_t i = 0; i < Count; ++i) {
            if (0 == i % 1000000) {
                lock_guard<mutex> lk(OutMutex);
                //cerr << ThreadId << " " << i << endl;
            }
            GenStream(state, 256, items);
            for (size_t j = 0; j < 256; ++j) 
                ++StatTable[j][items[j]];
        }
    }

    friend void PrintStatTables(vector<OneThreadTest>&, const size_t);
};
mutex OneThreadTest::OutMutex;


void PrintStatTables(vector<OneThreadTest>& testers, const size_t nTests) {
    const double expected = nTests * 256.0 / (256 * 256);
    for (size_t i = 0; i < 256; ++i) {
        vector<size_t> elements(256, 0);
        cout << i << "\t";
        for (vector<OneThreadTest>::const_iterator it = testers.begin(); it != testers.end(); ++it)
            for (size_t j = 0; j < 256; ++j)
                elements[j] += it->StatTable[i][j];// / expected;
        //cout << setprecision(6) << fixed;
        //vector<double>::const_iterator minIt = min_element(elements.begin(), elements.end());
        //vector<double>::const_iterator maxIt = max_element(elements.begin(), elements.end());
        //cout << "min:" << setw(3) << (minIt - elements.begin()) << "[" << setprecision(6) << fixed << *minIt << "] ";
        //cout << "max:" << setw(3) << (maxIt - elements.begin()) << "[" << setprecision(6) << fixed << *maxIt << "] ";
        for (size_t j = 0; j < 256; ++j) {
            cout << elements[j] << " ";
        }
        cout << endl;
    }
}


int main(int argc, const char* argv[]) {
    if (argc < 2) {
        cerr << "usage: " << argv[0] << " <threads count> [<iterations count>]" << endl;
        return 1;
    }
    size_t sz;
    const size_t WorkersCount = stoll(argv[1], &sz);
    assert(sz == string(argv[1]).size());
    const size_t NTests = 256 * 256 * ((2 < argc) ? stoll(argv[2], &sz) : 10);
    assert(sz == string(argv[2]).size());

    string task;
    while (getline(cin, task)) {
        const size_t seed = stoll(task, &sz);
        assert(sz == task.size() || task[sz] == '\t');
        vector<OneThreadTest> testers;
        vector<thread> workers;
        for (size_t i = 0; i < WorkersCount; ++i)
            testers.emplace_back(OneThreadTest(seed * WorkersCount + i, NTests / WorkersCount));
        for (size_t i = 0; i < WorkersCount; ++i)
            workers.emplace_back(thread(ref(testers[i])));
        for (size_t i = 0; i < WorkersCount; ++i)
            workers[i].join();

        PrintStatTables(testers, NTests);
    }
    return 0;
}
