#ifndef HEXED_ITUNES_DETAIL_HPLM_HPP_
#define HEXED_ITUNES_DETAIL_HPLM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class hplm : public array_segment<_Order>
            {
            public:
                hplm(blessed::span<blessed::byte> s) :
                    array_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hplm");
                }

            private:
            };
        }
    }
}

#endif
