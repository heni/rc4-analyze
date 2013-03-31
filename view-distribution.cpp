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

const size_t CHUNK_SIZE = 256;
const size_t N_ROWS = CHUNK_SIZE - 1;
const size_t N_COLS = 256 * 256;

class OneThreadTest {
    static mutex OutMutex;
    vector<vector<size_t>> StatTable;
    size_t ThreadId;
    size_t Count;
public:
    OneThreadTest(size_t id, size_t count) 
        : StatTable(N_ROWS, vector<size_t>(N_COLS, 0))
        , ThreadId(id)
        , Count(count)
    {}

    void operator()() {
        TRC4State state;
        vector<unsigned char> items(CHUNK_SIZE);
        InitializeRandomState(state, default_random_engine(ThreadId * Count + 1));
        for (size_t i = 0; i < Count; ++i) {
            if (0 == i % 1000000) {
                lock_guard<mutex> lk(OutMutex);
                //cerr << ThreadId << " " << i << endl;
            }
            GenStream(state, CHUNK_SIZE, items);
            for (size_t j = 0; j < N_ROWS; ++j) 
                ++StatTable[j][items[j] << 8 + items[j+1]];
        }
    }

    friend void PrintStatTables(vector<OneThreadTest>&, const size_t);
};
mutex OneThreadTest::OutMutex;


void PrintStatTables(vector<OneThreadTest>& testers, const size_t nTests) {
    for (size_t i = 0; i < N_ROWS; ++i) {
        vector<size_t> elements(N_COLS, 0);
        cout << i << "\t";
        for (vector<OneThreadTest>::const_iterator it = testers.begin(); it != testers.end(); ++it)
            for (size_t j = 0; j < N_COLS; ++j)
                elements[j] += it->StatTable[i][j];
        for (size_t j = 0; j < N_COLS; ++j)
            cout << elements[j] << " ";
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
