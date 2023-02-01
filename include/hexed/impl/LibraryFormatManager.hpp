#ifndef HEXED_IMPL_LIBRARY_FORMAT_MANAGER_IPP_
#define HEXED_IMPL_LIBRARY_FORMAT_MANAGER_IPP_

#include <hexed/LibraryFormatManager.hpp>

namespace hexed
{
    bool LibraryFormatManager::registerFormat(LibraryFormat *format, bool setDefault)
    {
        bool rv{false};

        return rv;
    }
    
    void LibraryFormatManager::registerBasicFormats()
    {
        registerFormat(new iTunesITLFormat());
    }
}

#endif
