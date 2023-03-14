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
            class hilm : array_segment<_Order>
            {
            public:
                hilm(blessed::span<blessed::byte> s) :
                    array_segment<_Order>(s)
                {
                    
                }

            private:
                array_segment<_Order> segment_;
            };
        }
    }
}

#endif
