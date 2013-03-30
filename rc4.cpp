#include "rc4.h"

void GenStream(TRC4State& state, size_t count, std::vector<unsigned char>& dest) {
    dest.resize(count);
    std::vector<unsigned char>::iterator oIt = dest.begin();
    for (unsigned char i = 0, j = 0; count; --count, ++oIt) {
        i = (i + 1) & 0xFF;
        j = (j + state[i]) & 0xFF;
        std::swap(state[i], state[j]);
        *oIt = state[(state[i] + state[j]) & 0xFF];
    }
}
