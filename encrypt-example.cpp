#include "rc4.h"
#include "stattables.h"
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;

const string ex_text = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum";

void SaveMassiveCryptoSamples(const string& outfilebase, const string& text, size_t count) {
    TRC4State state;
    UnigramBlocksStatistics statObj(text.size());
    InitializeRandomState(state, default_random_engine(system_clock::now().time_since_epoch().count()));
    size_t nextSavePoint = 1<<20, spIndex = 20;
    for (size_t i = 0; i < count; ++i) {
        if (i == nextSavePoint) {
            stringstream fnamePrn;
            fnamePrn << outfilebase << "." << spIndex;
            ofstream out(fnamePrn.str().c_str(), ios::binary);
            statObj.Dump(out);

            ++spIndex;
            nextSavePoint <<= 1;
        }
        vector<unsigned char> keystream;
        GenStream(state, text.size(), keystream);
        for (size_t j = 0; j < keystream.size(); ++j)
            keystream[j] ^= text[j];
        statObj.Update(keystream);
    }
    ofstream out(outfilebase.c_str());
    statObj.Dump(out);
}

int main() {
    SaveMassiveCryptoSamples("encrypted.dat", ex_text.substr(0, 256), 1ull<<32);
    return 0;
}
