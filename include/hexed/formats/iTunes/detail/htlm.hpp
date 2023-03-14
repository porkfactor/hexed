#ifndef HEXED_ITUNES_DETAIL_HTLM_HPP_
#define HEXED_ITUNES_DETAIL_HTLM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class htlm : public array_segment<_Order>
            {
            public:
                htlm(blessed::span<blessed::byte> s) :
                    array_segment<_Order>(s)
                {
                    
                }

                static uint32_t constexpr identifier() { return char2uint<_Order>("htlm"); }

            private:
            };
        }
    }
}

#endif
