#ifndef HEXED_ITUNES_DETAIL_BUFFER_HPP_
#define HEXED_ITUNES_DETAIL_BUFFER_HPP_

#include <type_traits>

#include <blessed/bit.hpp>
#include <blessed/byteswap.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template <blessed::endian _Order>
            struct buffer
            {
                buffer(void const *data, size_t szData) :
                    data_(data),
                    szData_(szData)
                {
                }

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

                template <typename _Type>
                _Type value(size_t offset) const
                {
                    return *byte_index<_Type>(offset);
                }

                template <typename _Type = void>
                _Type const *byte_index(size_t offset) const
                {
                    return reinterpret_cast<_Type const *>(static_cast<char const *>(data_) + offset);
                }

            private:
                void const *data_;
                size_t szData_;
            };
        }
    }
}

#endif
