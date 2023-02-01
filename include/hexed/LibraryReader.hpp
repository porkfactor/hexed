#ifndef HEXED_LIBRARY_READER_HPP_
#define HEXED_LIBRARY_READER_HPP_

#include <memory>

#include <hexed/Library.hpp>

namespace hexed
{
    class LibraryReader
    {
    public:
        virtual ~LibraryReader() {}

        virtual std::unique_ptr<Library> read() = 0;
    private:
    };
}

#endif
