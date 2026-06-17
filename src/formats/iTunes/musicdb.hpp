#ifndef HEXED_ITUNES_MUSICDB_HPP_
#define HEXED_ITUNES_MUSICDB_HPP_

#include <formats/iTunes/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            /**
             * the BOMA record is a generic attribute container for which
             * the type field interpretation is context dependant. 
             */
            template<blessed::endian _Order>
            struct boma : public data_segment<_Order>
            {
            public:
                using super = data_segment<_Order>;

                boma(blessed::span<blessed::byte const> s) :
                    super(s)
                {}

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("amob");
                }
            };

            template<blessed::endian _Order>
            struct iama : public dictionary_segment<_Order>
            {
            public:
                using super = dictionary_segment<_Order>;

                iama(blessed::span<blessed::byte const> s) :
                    super(s)
                {}
            };

            /**
             * Track Metadata container. This may be used in a track or
             * purchased track context.
             */
            template<blessed::endian _Order>
            class itma : public dictionary_segment<_Order>
            {
            public:
                using super = dictionary_segment<_Order>;

                itma(blessed::span<blessed::byte const> s) : 
                    super(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("amti");
                }

            private:
            };

            /**
             * Track Metadata list container. This may be used in a track or
             * purchased track context.
             */
            template<blessed::endian _Order>
            struct ltma : public array_segment<_Order>
            {
            public:
                using super = array_segment<_Order>;

                ltma(blessed::span<blessed::byte const> s) :
                    super(s)
                {}

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("amtl");
                }
            };

            template <blessed::endian _Order>
            struct hsma : public data_segment<_Order>
            {
            public:
                using super = data_segment<_Order>;

                enum section
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
                    subtype_hplm = 14,
                    subtype_unknown_15 = 15,
                    subtype_unknown_16 = 16,
                    subtype_unknown_17 = 17,
                    subtype_unknown_18 = 18,
                    subtype_unknown_19 = 19,
                    subtype_unknown_20 = 20,
                    subtype_unknown_21 = 21,
                };

                hsma(blessed::span<blessed::byte const> s) :
                    super(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                inline enum section type() const noexcept
                {
                    return static_cast<section>(super::subtype());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("amsh");
                }

            private:
            };

            template<blessed::endian _Order>
            class hfma : public data_segment<_Order>
            {
            public:
                using super = data_segment<_Order>;

                hfma(blessed::span<blessed::byte const> s) :
                    super(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("amfh");
                }

                uint32_t file_length() const { return this->buffer().uint32(8); }
                uint16_t major_version() const { return this->buffer().uint16(12); }
                uint16_t minor_version() const { return this->buffer().uint16(14); }
                uint32_t file_type() const { return this->buffer().uint32(56); }
                uint32_t track_count() const { return this->buffer().uint32(68); }
                uint32_t playlist_count() const { return this->buffer().uint32(72); }
                uint32_t album_count() const { return this->buffer().uint32(76); }
                uint32_t crypt_length() const { return this->buffer().uint32(84); }
            };
        }
    }
}

#endif
