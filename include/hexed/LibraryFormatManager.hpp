#ifndef HEXED_LIBRARY_FORMAT_MANAGER_HPP_
#define HEXED_LIBRARY_FORMAT_MANAGER_HPP_

#include <map>

#include <hexed/LibraryFormat.hpp>
#include <hexed/LibraryFormatReader.hpp>

namespace hexed
{
    class LibraryFormatManager
    {
    public:

        bool registerFormat(LibraryFormat *format, bool setDefault);
        void registerBasicFormats();

        LibraryFormat *findFormatForFileExtension(std::string const &e);

    private:
        std::map<int, std::unique_ptr<LibraryFormat>> formats_;
    };
}

#include <hexed/impl/LibraryFormatManager.hpp>

#endif
