#ifndef HEXED_ITUNES_DETAIL_BUFFER_HPP_
#define HEXED_ITUNES_DETAIL_BUFFER_HPP_

#include <type_traits>

#include <blessed/bit.hpp>
#include <blessed/byteswap.hpp>
#include <blessed/span.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template <blessed::endian _Order>
            struct segment_buffer
            {
                segment_buffer(blessed::span<blessed::byte> s) :
                    data_(s)
                {}

                segment_buffer(segment_buffer const &other) :
                    data_(other.data_)
                {}

                template <blessed::endian _O = _Order, typename std::enable_if<_O == blessed::endian::big, bool>::type = true>
                uint16_t uint16(size_t offset) const
                {
                    return be16toh(value<uint16_t>(offset));
                }

                template <blessed::endian _O = _Order, typename std::enable_if<_O == blessed::endian::little, bool>::type = true>
                uint16_t uint16(size_t offset) const
                {
                    return le16toh(value<uint16_t>(offset));
                }

                template <blessed::endian _O = _Order, typename std::enable_if<_O == blessed::endian::big, bool>::type = true>
                uint32_t uint32(size_t offset) const
                {
                    return be32toh(value<uint32_t>(offset));
                }

                template <blessed::endian _O = _Order, typename std::enable_if<_O == blessed::endian::little, bool>::type = true>
                uint32_t uint32(size_t offset) const
                {
                    return le32toh(value<uint32_t>(offset));
                }

                template <blessed::endian _O = _Order, typename std::enable_if<_O == blessed::endian::big, bool>::type = true>
                uint64_t uint64(size_t offset) const
                {
                    return be64toh(value<uint64_t>(offset));
                }

                template <typename _Type = void>
                _Type const *reinterpret_as(size_t offset) const
                {
                    return blessed::reinterpret_as<_Type const>(data_.subspan(offset));
                }

                inline blessed::span<blessed::byte> const &data() const noexcept
                {
                    return data_;
                }

            protected:
                template<typename _Type, blessed::enable_if_t<std::is_trivially_copyable<_Type>::value, bool> = true>
                _Type value(std::size_t offset) const noexcept
                {
                    _Type v;
                    std::memcpy(&v, &data_[offset], sizeof(_Type));
                    return v;
                }

            private:
                blessed::span<blessed::byte> data_;
            };
        }
    }
}

#endif
