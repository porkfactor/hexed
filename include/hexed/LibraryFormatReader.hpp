#ifndef HEXED_LIBRARY_FORMAT_READER_HPP_
#define HEXED_LIBRARY_FORMAT_READER_HPP_

#include <memory>

#include <hexed/Library.hpp>

namespace hexed
{
    class LibraryFormatReader
    {
    public:
        virtual ~LibraryFormatReader() {}

        virtual std::unique_ptr<Library> read(std::string const &s) = 0;
    private:
    };
}

#endif
