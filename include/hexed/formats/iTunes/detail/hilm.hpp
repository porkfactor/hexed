#ifndef HEXED_ITUNES_DETAIL_HILM_HPP_
#define HEXED_ITUNES_DETAIL_HILM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class hilm : public array_segment<_Order>
            {
            public:
                hilm(blessed::span<blessed::byte> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hilm");
                }

            private:
            };
        }
    }
}

#endif
