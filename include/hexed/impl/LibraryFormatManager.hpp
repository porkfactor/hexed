#ifndef HEXED_LIBRARY_FORMAT_MANAGER_IPP_
#define HEXED_LIBRARY_FORMAT_MANAGER_IPP_

#include <hexed/LibraryFormatManager.hpp>

#include <hexed/formats/iTunes/ITLFormat.hpp>

namespace hexed
{
    bool LibraryFormatManager::registerFormat(LibraryFormat *format, bool setDefault)
    {
        bool rv{false};

        formats_.emplace(0, std::move(std::unique_ptr<LibraryFormat>(format)));

        return rv;
    }
    
    void LibraryFormatManager::registerBasicFormats()
    {
        registerFormat(new hexed::itunes::ITLFormat(), false);
    }

    LibraryFormat *LibraryFormatManager::findFormatForFileExtension(std::string const &s)
    {
        return formats_[0].get();
    }
}

#endif
