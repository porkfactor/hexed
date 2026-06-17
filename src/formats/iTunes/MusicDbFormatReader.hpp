#ifndef HEXED_ITUNES_MUSICDB_FORMAT_READER_HPP_
#define HEXED_ITUNES_MUSICDB_FORMAT_READER_HPP_

#include <hexed/LibraryFormatReader.hpp>

#include <CoreFoundation/CFPropertyList.h>

namespace hexed
{
    namespace itunes
    {
        class MusicDbFormatReader : public hexed::LibraryFormatReader
        {
        public:
            MusicDbFormatReader();
            ~MusicDbFormatReader() override;

            std::unique_ptr<hexed::Library> read(std::string const &path) override;

        private:
        };
    }
}

#endif
