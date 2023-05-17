#include "tokenator.hpp"
#include "tokenator.hpp"

extern "C" {
    size_t tokenator_count(const char* data, size_t len)
    {
        return Tokenator::count(std::string(data, len));
    }
}
