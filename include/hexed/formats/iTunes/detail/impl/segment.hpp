#ifndef HEXED_ITUNES_DETAIL_SEGMENT_IPP_
#define HEXED_ITUNES_DETAIL_SEGMENT_IPP_

#include <codecvt>

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            basic_segment<_Order>::basic_segment(blessed::span<blessed::byte const> s) :
                buffer_(s),
                mnemonic_(buffer_.uint32(0)),
                data_offset_(buffer_.uint32(4))
            {}

            template<blessed::endian _Order>
            array_segment<_Order>::array_segment(blessed::span<blessed::byte const> s) :
                super(s),
                count_(super::buffer().uint32(8))
            {
                payload_ = super::data();
            }

            template<blessed::endian _Order>
            template<typename _Handler>
            inline void array_segment<_Order>::foreach(_Handler &&handler)
            {
                auto nItems = count();
                std::size_t offset{};

                for(decltype(nItems) i = 0; i < nItems; ++i)
                {
                    offset += handler(payload_.subspan(offset));
                }
            }

            template<blessed::endian _Order>
            dictionary_segment<_Order>::dictionary_segment(blessed::span<blessed::byte const> s) :
                super(s),
                length_(super::buffer().uint32(8)),
                count_(super::buffer().uint32(12))
            {
                //payload_ = super::data().subspan(0, length_);
                payload_ = super::data().subspan(0);
            }

            template<blessed::endian _Order>
            blessed::span<blessed::byte const> dictionary_segment<_Order>::payload() const noexcept
            {
                return payload_;
            }

            template<blessed::endian _Order>
            template<typename _Handler>
            inline void dictionary_segment<_Order>::foreach(_Handler &&handler)
            {
                uint32_t nItems = count();
                size_t offset{};

                for(uint32_t i = 0; i < nItems; ++i)
                {
                    offset += handler(payload_.subspan(offset));
                }
            }

            template<blessed::endian _Order>
            data_segment<_Order>::data_segment(blessed::span<blessed::byte const> s) :
                super(s),
                size_(super::buffer().uint32(8)),
                subtype_(super::buffer().uint32(12))
            {
                payload_ = super::data().subspan(0, size_ - super::data_offset());
            }
        }
    }
}

#endif
