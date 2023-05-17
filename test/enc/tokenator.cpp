// #include "types.h"
#include "tokenator.h"
#include "tokenizer.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <codecvt>
#include <locale>

using bpe_ranks_t = Tokenator::bpe_ranks_t;

std::string byte_encode(uint32_t x) noexcept
{
    uint32_t cs = x;
    if ((x < 33 || x > 126) && (x < 161 || x > 172) && (x < 174 || x > 255)) {
        uint32_t n = 0;
        for (uint32_t b = 0; b < 256; ++b) {
            if (!((b >= 33 && b <= 126) || (b >= 161 && b <= 172) || (b >= 174 && b <= 255))) {
                if (b == x) {
                    cs = 256 + n;
                    break;
                }
                ++n;
            }
        }
    }

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(static_cast<char32_t>(cs));
}

std::string encodeTok(const std::string& str) noexcept
{
    std::string result{};
    for (unsigned char ch : str) {
        result += byte_encode(static_cast<unsigned int>(ch));
    }

    return result;
}

std::set<std::pair<std::string, std::string>> get_pairs(const std::vector<std::string>& words) noexcept
{
    std::set<std::pair<std::string, std::string>> pairs;
    if(words.empty()) {
        return pairs;
    }
    std::string prev = words[0];
    for (size_t i = 1; i < words.size(); ++i) {
        pairs.insert({prev, words[i]});
        prev = words[i];
    }
    return pairs;
}

std::vector<std::string> split_utf8(const std::string str) noexcept
{
    icu::UnicodeString u_str = icu::UnicodeString::fromUTF8(str);
    std::vector<std::string> result;
    result.reserve(u_str.length());
    std::string item;
    for (int32_t i = 0; i < u_str.length(); ++i) {
        u_str.tempSubString(i, 1).toUTF8String(item);
        result.emplace_back(std::move(item));
    }

    return result;
}

size_t _find(const std::vector<std::string>& where, const std::string& what, int32_t statr_index = 0) noexcept
{
    for (size_t i = statr_index; i < where.size(); ++i) {
        if( where[i] == what ) {
            return i;
        }
    }
    return std::string::npos;
}

std::pair<std::string, std::string> get_min_bigram(const std::set<std::pair<std::string, std::string>>& pairs, const bpe_ranks_t& bpe_ranks) noexcept
{
    uint min_rank = std::numeric_limits<uint>::max();
    std::pair<std::string, std::string> min_bigram{};

    for (const auto& pair : pairs) {
        auto it = bpe_ranks.find(pair);
        if (it != bpe_ranks.end()) {
            if (it->second < min_rank) {
                min_rank = it->second;
                min_bigram = pair;
            }
        }
    }

    return min_bigram;
}

std::vector<std::string> bpe(const std::string& token, const bpe_ranks_t& bpe_ranks) noexcept
{
    std::vector<std::string> word = split_utf8(token);
    auto pairs = get_pairs(word);

    while (!pairs.empty()) {
        auto [first, second] = get_min_bigram(pairs, bpe_ranks);;
        if (first.empty() && second.empty()) {
            break;
        }
        std::vector<std::string> new_word;
        for (size_t i = 0; i < word.size();) {
            size_t j = _find(word, first, i);
            if (j == std::string::npos) {
                new_word.insert(new_word.end(), word.begin() + i, word.end());
                break;
            }
            new_word.insert(new_word.end(), word.begin() + i, word.begin() + j);
            i = j;

            if (word[i] == first && i + 1 < word.size() && word[i + 1] == second) {
                new_word.push_back(first + second);
                i = i + 2;
            } else {
                new_word.push_back(word[i]);
                i = i + 1;
            }
        }
        word = std::move(new_word);
        if (word.size() == 1) {
            break;
        } else {
            pairs = get_pairs(word);
        }        
    }

    return word;
}



size_t Tokenator::count(const std::string& text) noexcept
{
    size_t result = 0;
    Tokenizer toks(text);
    for (; !toks.empty(); toks.next()) {
        result += bpe(encodeTok(toks.current()), _bpe_ranks).size();
    }

    return result;
}
