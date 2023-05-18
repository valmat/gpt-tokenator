#include "impl.h"

extern "C" {
    size_t tokenator_count(const char* data, size_t len)
    {
        return tokenator::details::Impl::count(std::string(data, len));
    }
}
