#ifndef HEXED_ITUNES_DETAIL_ENVELOPE_HPP_
#define HEXED_ITUNES_DETAIL_ENVELOPE_HPP_

#include "header.hpp"

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template <blessed::endian _Order>
            struct envelope : public data_segment<_Order>
            {
            public:
                envelope(void const *p, size_t sz) : data_segment<_Order>(p, sz)
                {
                }

                uint16_t major_version() const { return this->data().uint16(12); }
                uint16_t minor_version() const { return this->data().uint16(14); }
                uint32_t file_type() const { return this->data().uint32(56); }
                uint32_t track_count() const { return this->data().uint32(68); }
                uint32_t playlist_count() const { return this->data().uint32(72); }
                uint32_t album_count() const { return this->data().uint32(76); }
                uint32_t crypt_length() const { return this->data().uint32(92); }

            private:
            };

        }
    }
}

#endif
