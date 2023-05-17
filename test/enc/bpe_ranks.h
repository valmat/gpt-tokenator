/**
 *  bpe_ranks.h
 */
#pragma once

#include <string>
// #include <vector>
#include <unordered_map>

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

using bpe_ranks_t = std::unordered_map<std::pair<std::string, std::string>, uint32_t, pair_hash>;


bpe_ranks_t bpe_ranks_recive() noexcept;