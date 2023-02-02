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
            class halm : 
            {
            public:
                halm(void const *data, size_t szData) :
                    segment_(data, szData)
                {
                    
                }

            private:
                array_segment<_Order> segment_;
            };
        }
    }
}

#endif
