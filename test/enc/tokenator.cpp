#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <limits>
#include <utility>
#include "unicode/unistr.h"
#include "tokenator.h"
#include "tokenizer.h"

using bpe_ranks_t = Tokenator::bpe_ranks_t;

static
std::string byte_encode(uint32_t x) noexcept
{
    uint32_t encoded_char = x;
    if ((x < 33 || x > 126) && (x < 161 || x > 172) && (x < 174 || x > 255)) {
        uint32_t n = 0;
        for (uint32_t b = 0; b < 256; ++b) {
            if (!((b >= 33 && b <= 126) || (b >= 161 && b <= 172) || (b >= 174 && b <= 255))) {
                if (b == x) {
                    encoded_char = 256 + n;
                    break;
                }
                ++n;
            }
        }
    }

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(static_cast<char32_t>(encoded_char));
}

static
std::string encodeTok(const std::string& str) noexcept
{
    std::string result{};
    for (unsigned char ch : str) {
        result += byte_encode(static_cast<unsigned int>(ch));
    }

    return result;
}

static
std::set<std::pair<std::string, std::string>> get_pairs(const std::vector<std::string>& words) noexcept
{
    std::set<std::pair<std::string, std::string>> pairs;
    std::string prev = words.empty() ? std::string() : words[0];
    for (size_t i = 1; i < words.size(); ++i) {
        pairs.emplace(std::pair{prev, words[i]});
        prev = words[i];
    }
    return pairs;
}

static
std::vector<std::string> split_utf8(const std::string& str) noexcept
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

static
size_t find(const std::vector<std::string>& where, const std::string& what, int32_t statr_index = 0) noexcept
{
    for (size_t i = statr_index; i < where.size(); ++i) {
        if( where[i] == what ) {
            return i;
        }
    }
    return std::numeric_limits<size_t>::max();
}

static
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

static
std::vector<std::string> bpe(const std::string& token, const bpe_ranks_t& bpe_ranks) noexcept
{
    std::vector<std::string> words = split_utf8(token);
    auto pairs = get_pairs(words);

    while (!pairs.empty()) {
        auto [first, second] = get_min_bigram(pairs, bpe_ranks);;
        if (first.empty() && second.empty()) {
            break;
        }
        std::vector<std::string> new_words;
        for (size_t i = 0; i < words.size();) {
            size_t j = find(words, first, i);
            if (j == std::numeric_limits<size_t>::max()) {
                new_words.insert(new_words.end(), words.begin() + i, words.end());
                break;
            }
            new_words.insert(new_words.end(), words.begin() + i, words.begin() + j);
            i = j;

            if (words[i] == first && i + 1 < words.size() && words[i + 1] == second) {
                new_words.push_back(first + second);
                i = i + 2;
            } else {
                new_words.push_back(words[i]);
                i = i + 1;
            }
        }
        words = std::move(new_words);
        if (words.size() == 1) {
            break;
        } else {
            pairs = get_pairs(words);
        }        
    }

    return words;
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