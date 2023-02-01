#ifndef HEXED_LIBRARY_FORMAT_MANAGER_HPP_
#define HEXED_LIBRARY_FORMAT_MANAGER_HPP_

#include <hexed/LibraryFormat.hpp>
#include <hexed/LibraryReader.hpp>

namespace hexed
{
    class LibraryFormatManager
    {
    public:

        bool registerFormat(LibraryFormat *format, bool setDefault);
        void registerBasicFormats();

    private:
    };
}

#include <hexed/impl/LibraryFormatManager.hpp>

#endif
