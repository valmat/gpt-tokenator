#pragma once

#include <string>

namespace tokenator {
    size_t count(const std::string& text) noexcept;
    size_t count(const char* data, size_t len) noexcept;
}