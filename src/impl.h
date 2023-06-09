#pragma once

#include <string>
#include <unordered_map>

namespace tokenator::details {

struct Impl
{
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };
    using bpe_ranks_t = std::unordered_map<std::pair<std::string, std::string>, uint16_t, pair_hash>;

    static size_t count(const std::string& text) noexcept;
    static size_t count(const char* data, size_t len) noexcept;
private:
    static const bpe_ranks_t _bpe_ranks;
};

} // end of namespace tokenator::details