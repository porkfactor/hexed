#ifndef BLESSED_MEMORY_HPP_
#define BLESSED_MEMORY_HPP_

#include <memory>

namespace blessed
{
    namespace detail
    {
        struct c_deleter
        {
            void operator()(void *p) const { ::free(p); }
        };
    }

    template<typename _Type = void>
    using unique_c_ptr = std::unique_ptr<_Type, detail::c_deleter>;

    template<typename _Type = void>
    unique_c_ptr<_Type> malloc_unique(std::size_t n)
    {
        return unique_c_ptr<_Type>(static_cast<_Type *>(::malloc(n)));
    }
}

#endif
