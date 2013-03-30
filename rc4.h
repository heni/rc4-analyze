#include <vector>
#include <algorithm>

typedef std::vector<unsigned char> TRC4State;
void GenStream(TRC4State& state, size_t count, std::vector<unsigned char>& dest);

template <class URNG>
void InitializeRandomState(TRC4State& state, URNG&& g) {
    state.resize(256);
    for (size_t i = 0; i < 256; ++i)
        state[i] = static_cast<unsigned char>(i);
    std::shuffle(state.begin(), state.end(), g);
}
