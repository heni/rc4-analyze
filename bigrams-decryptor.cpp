#include "stattables.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cctype>
#include <limits>
using namespace std;

vector<double> CalcLikelyhoods(const vector<double>& freqs, const vector<double>& expectedFreqs) {
    assert(expectedFreqs.size() == freqs.size());
    assert(fabsl(accumulate(freqs.begin(), freqs.end(), 0.0) - 1.0) < 1e-10);
    assert(fabsl(accumulate(expectedFreqs.begin(), expectedFreqs.end(), 0.0) - 1.0) < 1e-8);

    vector<double> retval(freqs.size());
    for (size_t chp = 0; chp < freqs.size(); ++chp) {
        double likelyhood = 0.0;
        for (size_t i = 0; i < freqs.size(); ++i)
            likelyhood += freqs[i^chp] * (log(expectedFreqs[i]) - log(1.0 - expectedFreqs[i]));
        retval[chp] = likelyhood;
    }
    return retval;
}

void BuildMoseLikelyCharacters(const BigramBlocksStatistics& freqs, const BigramBlocksStatistics& expectedFreqs) {
    typedef vector<pair<double, unsigned char>> TRow;
    vector<TRow> DinTable(256, TRow(256, TRow::value_type(0.0, 0)));
}

int main(int argc, const char* argv[]) {
    const string freqFile = "data/uni-distribution.bin";
    ifstream freqIn(freqFile.c_str(), ios::binary);
    BigramBlocksStatistics freqStObj(256);
    freqStObj.Load(freqIn);
    
    const string encFile = argv[1];
    ifstream encIn(encFile.c_str(), ios::binary);
    BigramBlocksStatistics encStObj(256);
    encStObj.Load(encIn);
    
    assert(encIn && freqIn);

    //BuildMoseLikelyCharacters(encStObj, freqStObj);
    vector<double> f0 = CalcLikelyhoods(encStObj.GetSlice(0), freqStObj.GetSlice(0));
    cout << max_element(f0.begin(), f0.end()) - f0.begin() << endl;
    return 0; 
}
