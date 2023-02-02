#ifndef HEXED_ITUNES_IMPL_ITL_FORMAT_HPP_
#define HEXED_ITUNES_IMPL_ITL_FORMAT_HPP_

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
