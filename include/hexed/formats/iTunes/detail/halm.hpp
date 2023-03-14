#ifndef HEXED_ITUNES_DETAIL_HALM_HPP_
#define HEXED_ITUNES_DETAIL_HALM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class halm : public array_segment<_Order>
            {
            public:
                halm(void const *data, size_t szData) :
                    array_segment<_Order>(data, szData)
                {
                    assert(this->mnemonic() == identifier());
                }

                static uint32_t const identifier() { return char2uint<_Order>("halm"); }

            private:
            };
        }
    }
}

#endif
