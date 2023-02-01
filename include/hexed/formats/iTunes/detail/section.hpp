#ifndef HEXED_ITUNES_DETAIL_SECTION_HPP_
#define HEXED_ITUNES_DETAIL_SECTION_HPP_

#include "header.hpp"

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template <blessed::endian _Order>
            struct section : public data_segment<_Order>
            {
            public:
                typedef enum
                {
                    subtype_htlm = 0x01,
                    subtype_hplm = 0x02,
                    subtype_file = 0x04,
                    subtype_halm = 0x09,
                    subtype_hilm = 0x0b,
                    subtype_hghm = 0x0c,
                    subtype_htlm_alt = 0x0d,
                    subtype_hdfm = 0x10,
                } block_type;

                section(void const *p, size_t sz) : data_segment<_Order>(p, sz)
                {
                }

                uint32_t type() const
                {
                    return this->data().uint32(12);
                }

            private:
            };

        }
    }
}

#endif
