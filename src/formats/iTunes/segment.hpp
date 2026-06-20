#ifndef HEXED_ITUNES_DETAIL_SEGMENT_HPP_
#define HEXED_ITUNES_DETAIL_SEGMENT_HPP_

#include <locale>
#include <codecvt>
#include <blessed/span.hpp>

#include <formats/iTunes/buffer.hpp>

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

            /**
             * segment -> node
             * 
             * segment::size -> overall segment size
             * segment::
             * segment::buffer [private] -> buffer
             * segment::header [protected] -> span
             * segment::data -> buffer.subspan(header.length())
             * 
             * type()
             * 
             * data_segment 
             *     
             * 
             */

            template <blessed::endian _Order = blessed::endian::native>
            struct basic_segment
            {
            public:
                basic_segment(blessed::span<blessed::byte const> s) :
                    buffer_(s),
                    mnemonic_(buffer_.native_uint32(0)),
                    data_offset_(buffer_.uint32(4))
                {}

                inline uint32_t mnemonic() const noexcept
                {
                    return mnemonic_;
                }

                inline std::size_t data_offset() const noexcept
                {
                    return data_offset_;
                }

                inline blessed::span<blessed::byte const> header() const noexcept
                {
                    return buffer_.data().subspan(0, data_offset_);
                }

                inline blessed::span<blessed::byte const> data() const noexcept
                {
                    return buffer_.data().subspan(data_offset_);
                }

            protected:
                inline segment_buffer<_Order> const &buffer() const noexcept
                {
                    return buffer_;
                }

            private:
                segment_buffer<_Order> buffer_;
                uint32_t mnemonic_;
                uint32_t data_offset_;
            };

            struct array_segment_iterator
            {

            };

            template <blessed::endian _Order>
            struct array_segment : public basic_segment<_Order>
            {
            public:
                using super = basic_segment<_Order>;

                array_segment(blessed::span<blessed::byte const> s) :
                    super(s),
                    count_(super::buffer().uint32(8))
                {
                    payload_ = super::data();
                }

                inline std::size_t size() const noexcept
                {
                    return super::data_offset() + payload().size();
                }

                inline std::size_t count() const noexcept
                {
                    return count_;
                }

                inline blessed::span<blessed::byte const> payload() const noexcept
                {
                    return payload_;
                }

            #if 0
                iterator begin();
                iterator end();
                const_iterator begin();
                const_iterator end();
                const_iterator cbegin();
                const_iterator cend();
            #endif
                template<typename _Handler>
                inline void foreach(_Handler &&handler)
                {
                    auto nItems = count();
                    std::size_t offset{};

                    for(decltype(nItems) i = 0; i < nItems; ++i)
                    {
                        offset += handler(payload_.subspan(offset));
                    }
                }

            private:
                blessed::span<blessed::byte const> payload_;
                uint32_t count_;
            };

            template <blessed::endian _Order>
            struct dictionary_segment : public basic_segment<_Order>
            {
            public:
                using super = basic_segment<_Order>;

                dictionary_segment(blessed::span<blessed::byte const> s) :
                    super(s),
                    length_(super::buffer().uint32(8)),
                    count_(super::buffer().uint32(12))
                {
                    //payload_ = super::data().subspan(0, length_);
                    payload_ = super::data().subspan(0);
                }

                inline std::size_t size() const noexcept
                {
                    return super::data_offset() + length();
                }

                inline std::size_t count() const noexcept
                {
                    return count_;
                }

                inline std::size_t length() const noexcept
                {
                    return length_;
                }

                inline blessed::span<blessed::byte const> payload() const noexcept
                {
                    return payload_;
                }

                template<typename _Handler>
                inline void foreach(_Handler &&handler)
                {
                    uint32_t nItems = count();
                    size_t offset{};

                    for(uint32_t i = 0; i < nItems; ++i)
                    {
                        offset += handler(payload_.subspan(offset));
                    }
                }

            private:
                blessed::span<blessed::byte const> payload_;
                uint32_t count_;
                uint32_t length_;
            };

            template <blessed::endian _Order>
            struct data_segment : public basic_segment<_Order>
            {
            public:
                using super = basic_segment<_Order>;

                data_segment(blessed::span<blessed::byte const> s) :
                    super(s),
                    size_(super::buffer().uint32(8)),
                    subtype_(super::buffer().uint32(12))
                {
                    payload_ = super::data().subspan(0, size_ - super::data_offset());
                }

                inline blessed::span<blessed::byte const> payload() const noexcept
                {
                    return payload_;
                }

                inline std::size_t size() const noexcept
                {
                    return size_;
                }

                inline uint32_t subtype() const noexcept
                {
                    return subtype_;
                }

            private:
                blessed::span<blessed::byte const> payload_;
                uint32_t size_;
                uint32_t subtype_;
            };

            template <blessed::endian _Order>
            struct flex_string
            {
            public:
                enum string_type
                {
                    uri = 0x0,
                    utf16_string = 0x01,
                    url_encoded_uri = 0x02,
                    utf8_string = 0x03,
                };

                flex_string(blessed::span<blessed::byte const> s)
                {
                    segment_buffer<_Order> b(s);
                    type_ = static_cast<string_type>(b.uint32(0));
                    value_ = b.data().subspan(16, b.uint32(4));
                }

                std::string u8() const
                {
                    switch (type_)
                    {
                    case string_type::uri:
                    case string_type::url_encoded_uri:
                    case string_type::utf8_string:
                        return to_u8string(value_);
                    case string_type::utf16_string:
                        return utf16_to_utf8(value_);
                    }

                    return std::string{};
                };

                std::u16string u16() const
                {
                    switch (type_)
                    {
                    case string_type::uri:
                    case string_type::url_encoded_uri:
                    case string_type::utf8_string:
                        return to_u16string(value_);
                    case string_type::utf16_string:
                        return utf8_to_utf16(value_);
                    }

                    return std::u16string{};
                }

            private:
                static std::string to_u8string(blessed::span<blessed::byte const> s)
                {
                    auto b = blessed::span_cast<char>(s);
                    return std::string(b.begin(), b.end());
                }

                static std::u16string to_u16string(blessed::span<blessed::byte const> s)
                {
                    auto b = blessed::span_cast<char16_t>(s);
                    return std::u16string(b.begin(), b.end());
                }

                static std::string utf16_to_utf8(blessed::span<blessed::byte const> s)
                {
                    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
                    auto b = blessed::span_cast<char16_t const>(s);
                    return cvt.to_bytes(b.data(), b.data() + b.size());
                }

                static std::u16string utf8_to_utf16(blessed::span<blessed::byte const> s)
                {
                    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
                    auto b = blessed::span_cast<char const>(s);
                    return cvt.from_bytes(b.data(), b.data() + b.size());
                }

                blessed::span<blessed::byte const> value_;
                string_type type_;
            };
        }
    }
}

#endif
