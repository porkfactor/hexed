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
                hiim(blessed::span<blessed::byte const> s) : 
                    dictionary_segment<_Order>(s)
                {
                    assert(this->mnemonic() == identifier());
                }

                static constexpr uint32_t identifier()
                {
                    return char2uint<_Order>("hiim");
                }

            private:
            };
        }
    }
}

#endif
