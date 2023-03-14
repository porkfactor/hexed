#ifndef HEXED_ITUNES_DETAIL_HTIM_HPP_
#define HEXED_ITUNES_DETAIL_HTIM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class htim : public dictionary_segment<_Order>
            {
            public:
                htim(void const *data, size_t szData) : 
                    dictionary_segment<_Order>(data, szData)
                {
                    assert(this->mnemonic() == identifier());
                }

                static uint32_t constexpr identifier() { return char2uint<_Order>("htim"); }

            private:
            };
        }
    }
}

#endif
