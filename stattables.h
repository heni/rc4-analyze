#include <vector>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>


template <typename Derived, size_t CELL_SIZE>
class StatisticsBase {
protected:
    static const size_t CellSize = CELL_SIZE;
    const size_t ChunkSize;
    size_t AggregatedBlocksCount;
    std::vector<std::vector<size_t>> Internal;
public:
    StatisticsBase(size_t chunksize)
        : ChunkSize(chunksize)
        , AggregatedBlocksCount(0)
        , Internal(ChunkSize, std::vector<size_t>(CELL_SIZE, 0))
    {}

    void Dump(std::ostream& out) const {
        for (const auto& subStat: Internal) {
            assert(accumulate(subStat.begin(), subStat.end(), 0ull) == AggregatedBlocksCount);
            out.write(reinterpret_cast<const char*>(&*subStat.begin()), CELL_SIZE * sizeof(subStat[0]));
            if (!out)
                throw std::runtime_error("statics dump error");
        }
    }

    std::vector<double> GetSlice(size_t index) const {
        std::vector<double> retval(CELL_SIZE);
        const double expected = AggregatedBlocksCount * 1.0;
        for (size_t i = 0; i < CELL_SIZE; ++i)
            retval[i] = Internal[index][i] / expected;
        return retval;
    }

    void Load(std::istream& in) {
        for (auto& subStat: Internal) {
            in.read(reinterpret_cast<char*>(&*subStat.begin()), CELL_SIZE * sizeof(subStat[0]));
            if (!in)
                throw std::runtime_error("statistics load error");
            if (&subStat == &*Internal.begin())
                AggregatedBlocksCount = accumulate(subStat.begin(), subStat.end(), 0ull);
            else
                assert(accumulate(subStat.begin(), subStat.end(), 0ull) == AggregatedBlocksCount);
        }
    }
};


class UnigramBlocksStatistics : public StatisticsBase<UnigramBlocksStatistics, 256> {
    typedef StatisticsBase<UnigramBlocksStatistics, 256> TBase;
    
public:
    UnigramBlocksStatistics(size_t chunksize)
        : TBase(chunksize)
    {}

    void Update(const std::vector<unsigned char>& buffer) {
        assert(buffer.size() == ChunkSize);
        for (size_t i = 0; i < ChunkSize; ++i)
            ++Internal[i][buffer[i]];
        ++AggregatedBlocksCount;
    }
};


class BigramBlocksStatistics : public StatisticsBase<BigramBlocksStatistics, 256 * 256> {
    typedef StatisticsBase<BigramBlocksStatistics, 256 * 256> TBase;
public:
    BigramBlocksStatistics(size_t chunksize)
        : TBase(chunksize)
    {}

    void Update(const std::vector<unsigned char>& buffer) {
        assert(buffer.size() == ChunkSize);
        for(size_t i = 0; i + 1 < ChunkSize; ++i) {
            const size_t offset = buffer[i] * 256 + buffer[i + 1];
            ++Internal[i][offset];
        }
        ++AggregatedBlocksCount;
    }
};
