#include "impl.h"

extern "C" {
    size_t tokenator_count(const char* data, size_t len) noexcept
    {
        return tokenator::details::Impl::count(data, len);
    }
}
