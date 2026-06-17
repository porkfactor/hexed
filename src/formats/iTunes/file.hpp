#ifndef HEXED_ITUNES_DETAIL_FILE_HPP_
#define HEXED_ITUNES_DETAIL_FILE_HPP_

#include <blessed/memory.hpp>
#include <blessed/span.hpp>
#include <blessed/bit.hpp>
#include <hexed/detail/MemoryMappedFile.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            class file
            {
            public:
                file(std::string const &);

                blessed::span<blessed::byte const> data() const;

            protected:
                hexed::detail::MemoryMappedFile mapping_;
                blessed::unique_c_ptr<blessed::byte> data_;
                size_t szData_;
            };
        }
    }
}

#endif
