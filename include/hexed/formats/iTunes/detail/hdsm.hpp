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
                    track_metadata = 0x01,
                    album_metadata = 0x00,
                    subtype_hplm = 0x02,
                    subtype_file = 0x04,
                    subtype_halm = 0x09,
                    subtype_hilm = 0x0b,
                    subtype_hghm = 0x0c,
                    purchased_track_metadata = 0x0d,
                    subtype_hdfm = 0x10,
                } section_type;

                hdsm(blessed::span<blessed::byte> s) :
                    data_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                uint32_t type() const
                {
                    return this->buffer().uint32(12);
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
