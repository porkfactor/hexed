#ifndef HEXED_LIBRARY_FORMAT_MANAGER_IPP_
#define HEXED_LIBRARY_FORMAT_MANAGER_IPP_

#include <hexed/LibraryFormatManager.hpp>

#include <hexed/formats/iTunes/ITLFormat.hpp>

namespace hexed
{
    bool LibraryFormatManager::registerFormat(LibraryFormat *format, bool setDefault)
    {
        bool rv{false};

        return rv;
    }
    
    void LibraryFormatManager::registerBasicFormats()
    {
        registerFormat(new hexed::itunes::ITLFormat(), false);
    }
}

#endif
