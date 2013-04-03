#include "stattables.h"
#include <walsh/dyadiccnvl.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cctype>
#include <limits>
using namespace std;

char printchar(int v) {
    return isprint(v) ? v : '.';
}

vector<double> CalcLikelyhoods(const vector<double>& freqs, const vector<double>& expectedFreqs) {
    assert(expectedFreqs.size() == freqs.size());
    assert(fabsl(accumulate(freqs.begin(), freqs.end(), 0.0) - 1.0) < 1e-10);
    assert(fabsl(accumulate(expectedFreqs.begin(), expectedFreqs.end(), 0.0) - 1.0) < 1e-10);
    vector<double> retval(freqs.size());
    for (size_t i = 0; i < expectedFreqs.size(); ++i)
        retval[i] = log(expectedFreqs[i]) - log(1.0 - expectedFreqs[i]);
    dyadic_convolution<double>(const_cast<double*>(&*freqs.begin()), &*retval.begin(), 16);
    return retval;
}

void BuildMostLikelyCharacters(const BigramBlocksStatistics& freqs, const BigramBlocksStatistics& expectedFreqs) {
    typedef vector<pair<double, unsigned char>> TRow;
    vector<TRow> DinTable(256, TRow(256, TRow::value_type(-numeric_limits<double>::infinity(), 0)));
    fill(DinTable[0].begin(), DinTable[0].end(), TRow::value_type(0.0, 0));
    const size_t FENCE = 255;
    for (size_t i = 0; i < FENCE; ++i) {
        vector<double> probs = CalcLikelyhoods(freqs.GetSlice(i), expectedFreqs.GetSlice(i));
        for (size_t offset = 0; offset < probs.size(); ++offset) {
            const unsigned char ch0 = offset / 256, ch1 = offset % 256;
            const double v = DinTable[i][ch0].first + probs[offset];
            if (DinTable[i + 1][ch1].first < v) 
                DinTable[i + 1][ch1] = TRow::value_type(v, ch0);
        }
    }
    string decrypted_message = "";
    unsigned char ch = max_element(DinTable[FENCE].begin(), DinTable[FENCE].end()) - DinTable[FENCE].begin();
    decrypted_message.push_back(printchar(ch));
    for (size_t i = FENCE; i; --i) {
        ch = DinTable[i][ch].second;
        decrypted_message.push_back(printchar(ch));
    }
    reverse(decrypted_message.begin(), decrypted_message.end());
    cout << decrypted_message << endl;
}

int main(int argc, const char* argv[]) {
    const string freqFile = "data/bi-distribution.bin";
    ifstream freqIn(freqFile.c_str(), ios::binary);
    BigramBlocksStatistics freqStObj(256);
    freqStObj.Load(freqIn);
    
    const string encFile = argv[1];
    ifstream encIn(encFile.c_str(), ios::binary);
    BigramBlocksStatistics encStObj(256);
    encStObj.Load(encIn);
    
    assert(encIn && freqIn);

    BuildMostLikelyCharacters(encStObj, freqStObj);
    return 0; 
}
