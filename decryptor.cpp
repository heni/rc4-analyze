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

int MostLikelyCharacter(const vector<double>& freqs, const vector<double>& expectedFreqs) {
    assert(expectedFreqs.size() == freqs.size());
    assert(fabsl(accumulate(freqs.begin(), freqs.end(), 0.0) - 1.0) < 1e-10);
    assert(fabsl(accumulate(expectedFreqs.begin(), expectedFreqs.end(), 0.0) - 1.0) < 1e-8);

    vector<double> xx(freqs.size());
    for (size_t i = 0; i < freqs.size(); ++i)
        xx[i] = log(expectedFreqs[i]) - log(1.0 - expectedFreqs[i]);
    dyadic_convolution<double>(const_cast<double*>(&*freqs.begin()), &*xx.begin(), 8);
    return max_element(xx.begin(), xx.end()) - xx.begin();
}

int main(int argc, const char* argv[]) {
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
        int ret = MostLikelyCharacter(encStObj.GetSlice(i), freqStObj.GetSlice(i));
        cout << (isprint(ret) ? static_cast<char>(ret) : '.');
    }
    cout << endl;
    return 0; 
}
