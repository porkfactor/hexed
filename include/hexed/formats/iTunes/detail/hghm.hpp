#ifndef HEXED_ITUNES_DETAIL_HGHM_HPP_
#define HEXED_ITUNES_DETAIL_HGHM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class hghm : public dictionary_segment<_Order>
            {
            public:
                hghm(blessed::span<blessed::byte const> s) :
                    dictionary_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hghm");
                }

            private:
            };
        }
    }
}

#endif
