#ifndef HEXED_ITUNES_DETAIL_HDSM_HPP_
#define HEXED_ITUNES_DETAIL_HDSM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template <blessed::endian _Order>
            struct hdsm : public data_segment<_Order>
            {
            public:
                typedef enum
                {
                    track_metadata = 1,
                    playlist_metadata = 2,
                    file_metadata = 4,
                    subtype_unknown_5 = 5,
                    subtype_unknown_6 = 6,
                    subtype_unknown_7 = 7,
                    subtype_unknown_8 = 8,
                    album_metadata = 9,
                    subtype_unknown_10 = 10,
                    subtype_hilm = 11,
                    subtype_hghm = 12,
                    purchased_track_metadata = 13,
                    subtype_hdfm = 14,
                    subtype_unknown_15 = 15,
                    subtype_unknown_16 = 16,
                    subtype_unknown_17 = 17,
                    subtype_unknown_18 = 18,
                    subtype_unknown_19 = 19,
                    subtype_unknown_20 = 20,
                    subtype_unknown_21 = 21,
                } section_type;

                hdsm(blessed::span<blessed::byte> s) :
                    data_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                section_type type() const
                {
                    return static_cast<section_type>(this->buffer().uint32(12));
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hdsm");
                }

            private:
            };

        }
    }
}

#endif
