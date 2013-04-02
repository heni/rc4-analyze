#include <vector>
#include <cassert>
#include <iostream>

class UnigramBlocksStatistics {
    static const size_t SYMBOLS = 256;
    const size_t ChunkSize;
    size_t AggregatedBlocksCount;
    std::vector<std::vector<size_t>> Internal;
public:
    UnigramBlocksStatistics(size_t chunksize)
        : ChunkSize(chunksize)
        , AggregatedBlocksCount(0)
        , Internal(chunksize, std::vector<size_t>(SYMBOLS, 0))
    {}

    void Update(const std::vector<unsigned char>& buffer) {
        assert(buffer.size() == ChunkSize);
        for (size_t i = 0; i < ChunkSize; ++i)
            ++Internal[i][buffer[i]];
        ++AggregatedBlocksCount;
    }

    std::vector<double> GetSlice(size_t index) const {
        std::vector<double> retval(SYMBOLS);
        const double expected = AggregatedBlocksCount * 1.0 / SYMBOLS;
        for (size_t i = 0; i < SYMBOLS; ++i)
            retval[i] = Internal[index][i] / expected;
        return retval;
    }

    void Dump(std::ostream& out) const {
        for (size_t i = 0; i < ChunkSize; ++i) {
            assert(accumulate(Internal[i].begin(), Internal[i].end(), 0ull) == AggregatedBlocksCount);
            out.write(reinterpret_cast<const char*>(&Internal[i][0]), SYMBOLS * sizeof(Internal[i][0]));
            if (!out)
                throw runtime_error("statistics dump error");
        }
    }

    void Load(std::istream& in) {
        for (size_t i = 0; i < ChunkSize; ++i) {
            in.read(reinterpret_cast<char*>(&Internal[i][0]), SYMBOLS * sizeof(Internal[i][0]));
            if (!in)
                throw runtime_error("statistics load error");

            if (i == 0)
                AggregatedBlocksCount = accumulate(Internal[i].begin(), Internal[i].end(), 0ull);
            else
                assert(accumulate(Internal[i].begin(), Internal[i].end(), 0ull) == AggregatedBlocksCount);
        }
    }
};
