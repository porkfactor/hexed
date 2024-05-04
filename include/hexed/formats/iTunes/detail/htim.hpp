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
                htim(blessed::span<blessed::byte> s) : 
                    dictionary_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("htim");
                }

            private:
            };
        }
    }
}

#endif
