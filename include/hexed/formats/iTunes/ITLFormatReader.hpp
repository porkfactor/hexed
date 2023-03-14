#ifndef HEXED_ITUNES_ITL_FORMAT_READER_HPP_
#define HEXED_ITUNES_ITL_FORMAT_READER_HPP_

#include <hexed/LibraryFormatReader.hpp>

namespace hexed
{
    namespace itunes
    {
        class ITLFormatReader : public hexed::LibraryFormatReader
        {
        public:
            ITLFormatReader();
            ~ITLFormatReader() override;

            std::unique_ptr<hexed::Library> read(std::string const &path) override;

        private:
        };
    }
}

#include <hexed/formats/iTunes/impl/ITLFormatReader.hpp>

#endif
