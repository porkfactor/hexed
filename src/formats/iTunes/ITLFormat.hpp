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

            std::shared_ptr<hexed::LibraryFormatReader> createReader(void) override;
        };
    }
}

#endif
