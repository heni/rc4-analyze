#include "stattables.h"
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

template <class StatTable>
class OneThreadTest {
    static const size_t CHUNK_SIZE = 256;
    size_t ThreadId;
    size_t Count;
    StatTable& StatObject;
public:
    OneThreadTest(size_t id, size_t count, StatTable& stObject) 
        : ThreadId(id)
        , Count(count)
        , StatObject(stObject)
    {}

    void operator()() {
        TRC4State state;
        vector<unsigned char> items(CHUNK_SIZE);
        InitializeRandomState(state, default_random_engine(ThreadId * Count + 1));
        for (size_t i = 0; i < Count; ++i) {
            GenStream(state, CHUNK_SIZE, items);
            StatObject.Update(items);
        }
    }
};


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
        
        vector<UnigramBlocksStatistics> statTables(WorkersCount, UnigramBlocksStatistics(256));
        vector<OneThreadTest<UnigramBlocksStatistics>> testers;
        vector<thread> workers;
        for (size_t i = 0; i < WorkersCount; ++i) 
            testers.emplace_back(OneThreadTest<UnigramBlocksStatistics>(seed * WorkersCount + i, NTests / WorkersCount, statTables[i]));
        for (size_t i = 0; i < WorkersCount; ++i)
            workers.emplace_back(thread(ref(testers[i])));
    
        for (size_t i = 0; i < WorkersCount; ++i)
            workers[i].join();
        PrintStatTables(&*statTables.begin(), statTables.size(), cout);
    }
    return 0;
}
