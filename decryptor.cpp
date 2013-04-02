#include "stattables.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cctype>
#include <limits>
using namespace std;

vector<vector<double>> LoadExpectedFreqs(istream& in) {
    vector<vector<double>> retval;
    for (size_t i = 0; i < 256; ++i) {
        string buffer;
        getline(in, buffer);
        size_t dPos = buffer.find_last_of(']');
        assert(dPos != string::npos);
        stringstream tmpIn(buffer.substr(dPos + 2));
        retval.emplace_back(vector<double>(256));
        for (size_t j = 0; j < 256; ++j) {
            tmpIn >> retval.back()[j];
            retval.back()[j] /= 256;
        }
        assert(tmpIn);
    }
    return retval;
}

int MostLikelyCharacter(const vector<double>& freqs, const vector<double>& expectedFreqs) {
    assert(expectedFreqs.size() == freqs.size());
    assert(fabsl(accumulate(freqs.begin(), freqs.end(), 0.0) - 1.0) < 1e-10);
    assert(fabsl(accumulate(expectedFreqs.begin(), expectedFreqs.end(), 0.0) - 1.0) < 1e-8);
    int bestChar = -1;
    double bestLikelyhood = -numeric_limits<double>::infinity(), worstLikelyhood = numeric_limits<double>::infinity();
    for (size_t ch = 0; ch < freqs.size(); ++ch) {
        double likelyhood = 0.0;
        for (size_t i = 0; i < freqs.size(); ++i) {
            likelyhood += freqs[i^ch] * (log(expectedFreqs[i]) - log(1.0 - expectedFreqs[i]));
        }
        if (bestLikelyhood < likelyhood) 
            bestChar = ch;
        bestLikelyhood = max(likelyhood, bestLikelyhood);
        worstLikelyhood = min(likelyhood, worstLikelyhood);
    }
    double d = bestLikelyhood - worstLikelyhood;
    cerr << d << " " << exp(d)/(1 + exp(d)) << " " << bestLikelyhood 
         << " " << 256 * (*max_element(freqs.begin(), freqs.end()) - *min_element(freqs.begin(), freqs.end())) 
         << " " << 256 * (*max_element(expectedFreqs.begin(), expectedFreqs.end()) - *min_element(expectedFreqs.begin(), expectedFreqs.end())) 
         << endl;
    return bestChar;
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
