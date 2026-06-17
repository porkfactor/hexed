#ifndef HEXED_ITUNES_ITL_HPP_
#define HEXED_ITUNES_ITL_HPP_

#include <formats/iTunes/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class haim : public dictionary_segment<_Order>
            {
            public:
                haim(blessed::span<blessed::byte const> s) :
                    dictionary_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("haim");
                }

            private:
            };

            template<blessed::endian _Order>
            class halm : public array_segment<_Order>
            {
            public:
                halm(blessed::span<blessed::byte const> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("halm");
                }

            private:
            };

            template<blessed::endian _Order>
            class hdfm : public array_segment<_Order>
            {
            public:
                hdfm(blessed::span<blessed::byte const> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hdfm");
                }

                uint16_t major_version() const { return this->buffer().uint16(12); }
                uint16_t minor_version() const { return this->buffer().uint16(14); }
                uint32_t file_type() const { return this->buffer().uint32(56); }
                uint32_t track_count() const { return this->buffer().uint32(68); }
                uint32_t playlist_count() const { return this->buffer().uint32(72); }
                uint32_t album_count() const { return this->buffer().uint32(76); }
                uint32_t crypt_length() const { return this->buffer().uint32(92); }
            };

            template <blessed::endian _Order>
            struct hdsm : public data_segment<_Order>
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

                hdsm(blessed::span<blessed::byte const> s) :
                    data_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                inline enum section type() const noexcept
                {
                    return static_cast<section>(super::subtype());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hdsm");
                }

            private:
            };

            template<blessed::endian _Order>
            class hfma : public data_segment<_Order>
            {
            public:
                hfma(blessed::span<blessed::byte const> s) :
                    data_segment<_Order>(s)
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

            template<blessed::endian _Order>
            class hghm : public dictionary_segment<_Order>
            {
            public:
                hghm(blessed::span<blessed::byte const> s) :
                    dictionary_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hghm");
                }

            private:
            };

            template<blessed::endian _Order>
            class hiim : public dictionary_segment<_Order>
            {
            public:
                hiim(blessed::span<blessed::byte const> s) : 
                    dictionary_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hiim");
                }

            private:
            };

            template<blessed::endian _Order>
            class hilm : public array_segment<_Order>
            {
            public:
                hilm(blessed::span<blessed::byte const> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hilm");
                }

            private:
            };

            template<blessed::endian _Order>
            struct hohm : public data_segment<_Order>
            {
            public:
                using super = data_segment<_Order>;

                hohm(blessed::span<blessed::byte const> s) :
                    data_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hohm"); 
                }

            private:
            };

            template<blessed::endian _Order>
            class hplm : public array_segment<_Order>
            {
            public:
                hplm(blessed::span<blessed::byte const> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hplm");
                }

            private:
            };

            template<blessed::endian _Order>
            class hqlm : public array_segment<_Order>
            {
            public:
                hqlm(blessed::span<blessed::byte const> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hqlm");
                }

            private:
            };

            template<blessed::endian _Order>
            class hslm : public array_segment<_Order>
            {
            public:
                hslm(blessed::span<blessed::byte const> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hslm");
                }

            private:
            };

            template<blessed::endian _Order>
            class htlm : public array_segment<_Order>
            {
            public:
                htlm(blessed::span<blessed::byte const> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("htlm");
                }

            private:
            };

            template<blessed::endian _Order>
            class htim : public dictionary_segment<_Order>
            {
            public:
                htim(blessed::span<blessed::byte const> s) : 
                    dictionary_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("htim");
                }

            private:
            };
        }
    }
}

#endif
