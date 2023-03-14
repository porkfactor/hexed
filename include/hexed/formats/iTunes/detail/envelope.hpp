#ifndef HEXED_ITUNES_DETAIL_ENVELOPE_HPP_
#define HEXED_ITUNES_DETAIL_ENVELOPE_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template <blessed::endian _Order>
            struct envelope : public array_segment<_Order>
            {
            public:
                envelope(blessed::span<blessed::byte> s) :
                    array_segment<_Order>(s)
                {
                }

                uint16_t major_version() const { return this->buffer().uint16(12); }
                uint16_t minor_version() const { return this->buffer().uint16(14); }
                uint32_t file_type() const { return this->buffer().uint32(56); }
                uint32_t track_count() const { return this->buffer().uint32(68); }
                uint32_t playlist_count() const { return this->buffer().uint32(72); }
                uint32_t album_count() const { return this->buffer().uint32(76); }
                uint32_t crypt_length() const { return this->buffer().uint32(92); }

            private:
            };

        }
    }
}

#endif
