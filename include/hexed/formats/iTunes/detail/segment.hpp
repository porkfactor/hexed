#ifndef HEXED_ITUNES_DETAIL_SEGMENT_HPP_
#define HEXED_ITUNES_DETAIL_SEGMENT_HPP_

#include <blessed/span.hpp>

#include <hexed/formats/iTunes/detail/buffer.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order, typename std::enable_if<_Order == blessed::endian::big, bool>::type = true>
            static constexpr uint32_t char2uint(char const *s)
            {
                return 
                    (static_cast<uint32_t>(s[0])) |
                    (static_cast<uint32_t>(s[1]) << 8) |
                    (static_cast<uint32_t>(s[2]) << 16) |
                    (static_cast<uint32_t>(s[3]) << 24);
            }

            template<blessed::endian _Order, typename std::enable_if<_Order == blessed::endian::little, bool>::type = true>
            static constexpr uint32_t char2uint(char const *s)
            {
                return 
                    (static_cast<uint32_t>(s[3])) |
                    (static_cast<uint32_t>(s[2]) << 8) |
                    (static_cast<uint32_t>(s[1]) << 16) |
                    (static_cast<uint32_t>(s[0]) << 24);
            }

            template <blessed::endian _Order = blessed::endian::native>
            struct basic_segment
            {
            public:
                basic_segment(blessed::span<blessed::byte> s);

                uint32_t mnemonic() const noexcept;
                std::size_t header_length() const noexcept;

                inline blessed::span<blessed::byte> payload(std::size_t extent = blessed::dynamic_extent) const noexcept
                {
                    return data().subspan(header_length(), extent);
                }

                inline blessed::span<blessed::byte> const &data() const noexcept
                {
                    return buffer_.data();
                }

            protected:
                inline segment_buffer<_Order> const &buffer() const noexcept
                {
                    return buffer_;
                }

            private:
                segment_buffer<_Order> buffer_;
            };

            template <blessed::endian _Order>
            struct array_segment : public basic_segment<_Order>
            {
            public:
                using super = basic_segment<_Order>;

                array_segment(blessed::span<blessed::byte> s);

                std::size_t count() const noexcept;

                template<typename _Handler>
                inline void foreach(_Handler &&handler);

            private:
                blessed::span<blessed::byte> data_;
            };

            template <blessed::endian _Order>
            struct dictionary_segment : public basic_segment<_Order>
            {
            public:
                using super = basic_segment<_Order>;

                dictionary_segment(blessed::span<blessed::byte> s);

                std::size_t count() const noexcept;
                std::size_t length() const noexcept;

                template<typename _Handler>
                inline void foreach(_Handler &&handler);

            private:
                blessed::span<blessed::byte> data_;
            };

            template <blessed::endian _Order>
            struct data_segment : public basic_segment<_Order>
            {
            public:
                using super = basic_segment<_Order>;

                data_segment(blessed::span<blessed::byte> s);

                std::size_t length() const noexcept;
                uint32_t type() const noexcept;
                inline segment_buffer<_Order> const &content() const noexcept { return content_; }

            private:
                segment_buffer<_Order> content_;
            };
        }
    }
}

#include <hexed/formats/iTunes/detail/impl/segment.hpp>

#endif
