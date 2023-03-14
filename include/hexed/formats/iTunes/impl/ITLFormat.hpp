#ifndef HEXED_ITUNES_ITL_FORMAT_IPP_
#define HEXED_ITUNES_ITL_FORMAT_IPP_

#include <hexed/formats/iTunes/ITLFormatReader.hpp>

namespace hexed
{
    namespace itunes
    {
        ITLFormat::ITLFormat()
        {

        }

        ITLFormat::~ITLFormat()
        {

        }

        hexed::LibraryFormatReader *ITLFormat::createReader()
        {
            return new ITLFormatReader();
        }
    }
}

#endif
