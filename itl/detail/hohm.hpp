#ifndef HEXED_ITL_DETAIL_HOHM_HPP_
#define HEXED_ITL_DETAIL_HOHM_HPP_

#include "header.hpp"

namespace hexed
{
    namespace itl
    {
        namespace detail
        {
            template<blessed::endian _Order>
            struct hohm :
                public data_segment<_Order>
            {
            public:
                enum
                {
                    track_title = 0x02,
                    album_title = 0x03,
                    artist = 0x04,
                    genre = 0x05,
                    file_type = 0x06,
                    comments = 0x08,
                    local_path = 0x0b,
                    composer = 0x0c,
                    grouping = 0x0e,
                    album_artist = 0x1b,
                    xml_block_asset_info = 0x1d,
                    sort_title = 0x1e,
                    sort_album = 0x1f,
                    sort_artist = 0x20,
                    sort_album_artist = 0x21,
                    sort_composer = 0x22,
                    isrc = 0x2b,
                    copyright = 0x2e,
                    xml_block_artwork = 0x36,
                    purchaser_email = 0x3b,
                    purchaser_name = 0x3c,
                    playlist_name = 0x64,
                    smart_criteria = 0x65,
                    library_name = 0x1fc,
                };  

                hohm(void const *data, size_t szData) :
                    data_segment<_Order>(data, szData)
                {}

                uint32_t type() const { return this->uint32(12); }

                std::string utf8() const { return ""; }
            };
        }
    }
}

#endif