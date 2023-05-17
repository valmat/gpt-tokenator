#pragma once
#include <string>
#include <unicode/regex.h>
#include <iostream>

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