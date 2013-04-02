#include "stattables.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
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
        for (size_t j = 0; j < 256; ++j)
            tmpIn >> retval.back()[j];
        assert(tmpIn);
    }
    return retval;
}

int MostLikelyCharacter(const vector<double>& freqs, const vector<double>& expectedFreqs) {
    assert(expectedFreqs.size() == freqs.size());
    int bestChar = -1;
    double bestLikelyhood = -numeric_limits<double>::infinity();
    for (size_t ch = 0; ch < freqs.size(); ++ch) {
        double likelyhood = 0.0;
        for (size_t i = 0; i < freqs.size(); ++i) {
            likelyhood += log(expectedFreqs[i ^ ch]) * freqs[i];
        }
        cerr << likelyhood << " ";
        if (bestLikelyhood < likelyhood) {
            bestLikelyhood = likelyhood;
            bestChar = ch;
        }
    }
    cerr << endl;
    cerr << bestLikelyhood << endl;
    return bestChar;
}

int main() {
    const string freqFile = "data/rc4-distribution-1g.log";
    ifstream freqIn(freqFile.c_str());
    vector<vector<double>> expectedFreqs = LoadExpectedFreqs(freqIn);
    
    const string encFile = "encrypted.bin";
    ifstream encIn(encFile.c_str(), ios::binary);
    UnigramBlocksStatistics encStObj(256);
    encStObj.Load(encIn);
    
    assert(encIn && freqIn);

    for (size_t i = 0; i < 256; ++i)
        cout << static_cast<char>(MostLikelyCharacter(encStObj.GetSlice(i), expectedFreqs[i]));
    cout << endl;
    return 0; 
}
