#include "rc4.h"
#include "stattables.h"
#include <string>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;

const string ex_text = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum";

void SaveMassiveCryptoSamples(ostream& out, const string& text, size_t count) {
    TRC4State state;
    UnigramBlocksStatistics statObj(text.size());
    InitializeRandomState(state, default_random_engine(system_clock::now().time_since_epoch().count()));
    for (size_t i = 0; i < count; ++i) {
        vector<unsigned char> keystream;
        GenStream(state, text.size(), keystream);
        for (size_t i = 0; i < keystream.size(); ++i)
            keystream[i] ^= text[i];
        statObj.Update(keystream);
    }
    statObj.Dump(out);
}

int main() {
    ofstream out("encrypted.dat", ios::binary);
    SaveMassiveCryptoSamples(out, ex_text.substr(0, 256), 64<<20);
    out.close();
    return 0;
}
