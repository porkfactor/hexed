#ifndef HEXED_ITUNES_DETAIL_FILE_HPP_
#define HEXED_ITUNES_DETAIL_FILE_HPP_

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

            private:
                hexed::detail::MemoryMappedFile mapping_;
            };
        }
    }
}

#include <hexed/formats/iTunes/detail/impl/file.hpp>

#endif
