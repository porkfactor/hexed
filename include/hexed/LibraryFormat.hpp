#ifndef HEXED_LIBRARY_FORMAT_HPP_
#define HEXED_LIBRARY_FORMAT_HPP_

namespace hexed
{
    class LibraryFormatReader;

    class LibraryFormat
    {
    public:
        LibraryFormat();
        virtual ~LibraryFormat();

        virtual LibraryFormatReader *createReader() = 0;

    private:
    };
}

#include <hexed/impl/LibraryFormat.hpp>

#endif
