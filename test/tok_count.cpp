#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <unicode/regex.h>
#include <cassert>
#include "enc/bpe_ranks.h"

class Tokenizer
{
    UErrorCode _status = U_ZERO_ERROR;
    icu::UnicodeString pattern = icu::UnicodeString::fromUTF8(R"((?:'s|'t|'re|'ve|'m|'ll|'d| ?\p{L}+| ?\p{N}+| ?[^\s\p{L}\p{N}]+|\s+(?!\S)|\s+))");
    icu::UnicodeString _input;
    icu::RegexMatcher _matcher;
    std::string _current;
    bool _active = true;

public:
    Tokenizer(const std::string& text) noexcept :
        _input(icu::UnicodeString::fromUTF8(text)),
        _matcher(pattern, 0, _status)
    {
        if (U_FAILURE(_status)) {
            std::cerr << "Error creating RegexMatcher: " << u_errorName(_status) << std::endl;
            _active = false;
            return;
        }
        _matcher.reset(_input);
        next();
    }

    const std::string& current() const &  noexcept { return _current; }
    std::string        current() const && noexcept { return _current; }

    bool empty() const noexcept { return !_active; }

    bool next() noexcept
    {
        _active = _matcher.find(_status);
        if (_active) {
            if (U_FAILURE(_status)) {
                std::cerr << "Error in RegexMatcher::find: " << u_errorName(_status) << std::endl;
                _active = false;
                return false;
            }
            icu::UnicodeString token = _matcher.group(_status);
            _current.clear();
            token.toUTF8String(_current);
        }

        return _active;
    }
};

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

class TokCounter
{
    bpe_ranks_t _bpe_ranks{bpe_ranks_recive()};
public:

    size_t count(const std::string& text) const noexcept
    {
        size_t result = 0;
        Tokenizer toks(text);
        for (; !toks.empty(); toks.next()) {
            result += bpe(encodeTok(toks.current()), _bpe_ranks).size();
        }

        return result;
    }
    
};

int main() {
    TokCounter counter;

    std::initializer_list<std::pair<size_t, std::string>> phrases = {
        { 4   , "Ddfgdgd"},
        { 4   , "dfgdfg"},
        { 11  , "df  f dfg dr  rg  d "},
        { 2   , "  "},
        { 1   , "_"},
        { 0   , ""},
        { 1   , "_______"},
        { 10  , "This is a test, and it's working!"},
        { 24  , "This is an example sentence to try encoding out on! &#8211; Привет!"},
        { 156 , "Соединив два слова мы получаем «гамарджоба генацвале» გამარჯობა, что на русском языке будет звучать как «здравствуй, добрый друг»"},
        { 1   , "This"},
        { 27  , "გამარჯობა"},
        { 55  , R"({"title": string,"totalResults": string,"searchTerms": string,"count": integer,"startIndex": integer,"startPage": integer,"language": string,"inputEncoding": string,"outputEncoding": string,"safe": string,"cx": string})"},
        { 58  , "😢😟🤓😎😭😶‍🌫️🥵😦🥶😳✅❤️😳🤞😩😞😢❌🌷➕🇺🇦"},
        { 299 , "Приветствую! (русский); Hello! (английский); Bonjour! (французский); Hola! (испанский); Hallo! (немецкий); Ciao! (итальянский); Olá! (португальский); こんにちは！(японский); 안녕하세요! (корейский); 你好！(китайский); прывітанне! (белорусский); Здраво! (сербский); Здраво! (македонский); Здравейте! (болгарский)"},
        { 197 , "Hello! Вітаю! Здравствуйте! Bonjour! ¡Hola! Ciao! Olá! こんにちは！(Konnichiwa!) 안녕하세요! (Annyeonghaseyo!) 你好！(Nǐ hǎo!) გამარჯობა! (Gamardjoba!) Բարև Ձեզ! (Barev Dzez!) Γεια σας! (Geia sas!) Merhaba! Исәнмесез! (Isänmesez!)"},
    };
    for (const auto& phrase: phrases) {
        size_t count = counter.count(phrase.second);
        std::cout << "phrase (" << count << " ):\t" << phrase.second <<"\n";
        assert(count == phrase.first);

    }

    return 0;
}
