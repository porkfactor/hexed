#ifndef HEXED_ITUNES_DETAIL_HDFM_HPP_
#define HEXED_ITUNES_DETAIL_HDFM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class hdfm : public basic_segment<_Order>
            {
            public:
                hdfm(blessed::span<blessed::byte> s) :
                    basic_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hdfm");
                }
            };
        }
    }
}

#endif
