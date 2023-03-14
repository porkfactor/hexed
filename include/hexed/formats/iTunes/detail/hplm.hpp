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
            class hplm : array_segment<_Order>
            {
            public:
                hplm(blessed::span<blessed::byte> s) :
                    array_segment<_Order>(s)
                {
                    
                }

            private:
            };
        }
    }
}

#endif
