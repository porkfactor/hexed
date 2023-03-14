#ifndef HEXED_ITUNES_DETAIL_HIIM_HPP_
#define HEXED_ITUNES_DETAIL_HIIM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class hiim : public dictionary_segment<_Order>
            {
            public:
                hiim(void const *data, size_t szData) : 
                    dictionary_segment<_Order>(data, szData)
                {
                    assert(this->mnemonic() == identifier());
                }

                static uint32_t constexpr identifier() { return char2uint<_Order>("hiim"); }

            private:
            };
        }
    }
}

#endif
