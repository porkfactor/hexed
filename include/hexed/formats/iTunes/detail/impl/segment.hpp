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
            basic_segment<_Order>::basic_segment(blessed::span<blessed::byte> s) :
                buffer_(s)
            {}

            template<blessed::endian _Order>
            uint32_t basic_segment<_Order>::mnemonic() const noexcept
            {
                return buffer_.uint32(0);
            }

            template<blessed::endian _Order>
            std::size_t basic_segment<_Order>::header_length() const noexcept
            {
                return buffer_.uint32(4);
            }

            template<blessed::endian _Order>
            array_segment<_Order>::array_segment(blessed::span<blessed::byte> s) :
                super(s)
            {
                data_ = super::buffer().data().subspan(super::header_length());
            }

            template<blessed::endian _Order>
            std::size_t array_segment<_Order>::count() const noexcept
            {
                return super::buffer().uint32(8);
            }

            template<blessed::endian _Order>
            template<typename _Handler>
            inline void array_segment<_Order>::foreach(_Handler &&handler)
            {
                auto nItems = count();
                std::size_t offset{};

                for(decltype(nItems) i = 0; i < nItems; ++i)
                {
                    offset += handler(data_.subspan(offset));
                }
            }

            template<blessed::endian _Order>
            dictionary_segment<_Order>::dictionary_segment(blessed::span<blessed::byte> s) :
                super(s)
            {
                data_ = super::buffer().data().subspan(length() - super::header_length());
            }

            template<blessed::endian _Order>
            std::size_t dictionary_segment<_Order>::length() const noexcept
            {
                return super::buffer().uint32(8);
            }

            template<blessed::endian _Order>
            std::size_t dictionary_segment<_Order>::count() const noexcept
            {
                return super::buffer().uint32(12);
            }

            template<blessed::endian _Order>
            template<typename _Handler>
            inline void dictionary_segment<_Order>::foreach(_Handler &&handler)
            {
                uint32_t nItems = count();
                size_t offset{};

                for(uint32_t i = 0; i < nItems; ++i)
                {
                    offset += handler(data_.subspan(offset));
                }
            }

            template<blessed::endian _Order>
            data_segment<_Order>::data_segment(blessed::span<blessed::byte> s) :
                super(s),
                content_(super::payload(length() - super::header_length()))
            {

            }

            template<blessed::endian _Order>
            std::size_t data_segment<_Order>::length() const noexcept
            {
                return super::buffer().uint32(8);
            }

            template<blessed::endian _Order>
            uint32_t data_segment<_Order>::type() const noexcept
            {
                return super::buffer().uint32(12);
            }
        }
    }
}

#endif
