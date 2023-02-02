#ifndef HEXED_ITUNES_ITL_FORMAT_HPP_
#define HEXED_ITUNES_ITL_FORMAT_HPP_

#include <hexed/LibraryFormat.hpp>

namespace hexed
{
    namespace itunes
    {
        class ITLFormat : public hexed::LibraryFormat
        {
        public:
            ITLFormat();
            ~ITLFormat() override;

            hexed::LibraryFormatReader *createReader() override;
        };
    }
}

#include <hexed/formats/iTunes/impl/ITLFormat.hpp>

#endif
