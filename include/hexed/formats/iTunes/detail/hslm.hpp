#ifndef HEXED_ITUNES_DETAIL_HSLM_HPP_
#define HEXED_ITUNES_DETAIL_HSLM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class hslm : public array_segment<_Order>
            {
            public:
                hslm(blessed::span<blessed::byte const> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hslm");
                }

            private:
            };
        }
    }
}

#endif
