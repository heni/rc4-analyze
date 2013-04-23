#include "stattables.h"
#include <walsh/dyadiccnvl.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cctype>
#include <limits>
using namespace std;

int GetBestCharacter(const vector<double>& freqs, const vector<double>& expectedFreqs) {
    assert(expectedFreqs.size() == freqs.size());
    int freqMaxPos = max_element(freqs.begin(), freqs.end()) - freqs.begin();
    int expfMaxPos = max_element(expectedFreqs.begin(), expectedFreqs.end()) - expectedFreqs.begin();
    return freqMaxPos ^ expfMaxPos;
}

int main(int argc, const char* argv[]) {
    try {
        const string freqFile = "data/uni-distribution.bin";
        ifstream freqIn(freqFile.c_str(), ios::binary);
        UnigramBlocksStatistics freqStObj(256);
        freqStObj.Load(freqIn);
        
        const string encFile = argv[1];
        ifstream encIn(encFile.c_str(), ios::binary);
        UnigramBlocksStatistics encStObj(256);
        encStObj.Load(encIn);
        
        assert(encIn && freqIn);

        for (size_t i = 0; i < 256; ++i) {
            int ret = GetBestCharacter(encStObj.GetSlice(i), freqStObj.GetSlice(i));
            cout << (isprint(ret) ? static_cast<char>(ret) : '.');
        }
        cout << endl;
        return 0; 
    } catch (const exception& e) {
        cerr << "exception occurred: " << e.what() << endl;
        return 1;
    }
}
