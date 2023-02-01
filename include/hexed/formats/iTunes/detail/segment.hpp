#ifndef HEXED_ITUNES_DETAIL_HEADER_HPP_
#define HEXED_ITUNES_DETAIL_HEADER_HPP_

#include "buffer.hpp"

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template <blessed::endian _Order>
            struct basic_segment
            {
            public:
                basic_segment(void const *p, size_t sz) : data_(p, sz)
                {
                }

                uint32_t mnemonic() const { return data_.uint32(0); }
                size_t header_length() const { return static_cast<size_t>(data_.uint32(4)); }
                size_t data_length() const { return static_cast<size_t>(data_.uint32(8)); }

                void const *header() const { return data_.byte_index(0); }
                void const *payload() const { return data_.byte_index(header_length()); }
                size_t payload_length() const { return data_length() - header_length(); }

                uint32_t uint32(size_t offset) const { return data_.uint32(offset); }

            protected:
                buffer<_Order> const &data() const { return data_; }

            private:
                buffer<_Order> data_;
            };

            template <blessed::endian _Order>
            struct array_segment : public basic_segment<_Order>
            {
            public:
                array_segment(void const *p, size_t sz) :
                    basic_segment<_Order>(p, sz)
                {}

                size_t count() const { return this->uint32(8); }

                template<typename _Handler>
                inline void foreach(_Handler &&handler)
                {
                    uint32_t nItems = count();
                    size_t offset{};
                    void const *data = this->payload();
                    size_t szData = this->payload_length();

                    for(uint32_t i = 0; i < nItems; ++i)
                    {
                        offset += handler(static_cast<char const *>(data) + offset, szData - offset);
                    }
                }

            private:
            };

            template <blessed::endian _Order>
            struct dictionary_segment : public basic_segment<_Order>
            {
            public:
                dictionary_segment(void const *p, size_t sz) :
                    basic_segment<_Order>(p, sz)
                {}

                size_t count() const { return this->uint32(12); }

                template<typename _Handler>
                inline void foreach(_Handler &&handler)
                {
                    uint32_t nItems = count();
                    size_t offset{};
                    void const *data = this->payload();
                    size_t szData = this->payload_length();

                    for(uint32_t i = 0; i < nItems; ++i)
                    {
                        offset += handler(static_cast<char const *>(data) + offset, szData - offset);
                    }
                }

            private:
            };


            template <blessed::endian _Order>
            struct data_segment : public basic_segment<_Order>
            {
            public:
                data_segment(void const *p, size_t sz) :
                    basic_segment<_Order>(p, sz)
                {}

            private:
            };
        }
    }
}

#endif
