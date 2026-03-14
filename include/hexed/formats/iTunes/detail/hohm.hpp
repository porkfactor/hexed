#ifndef HEXED_ITUNES_DETAIL_HOHM_HPP_
#define HEXED_ITUNES_DETAIL_HOHM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            struct hohm : public data_segment<_Order>
            {
            public:
                using super = data_segment<_Order>;

                hohm(blessed::span<blessed::byte const> s) :
                    data_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hohm"); 
                }

            private:
            };
        }
    }
}

#endif