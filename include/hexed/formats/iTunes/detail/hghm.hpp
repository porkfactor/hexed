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
            class hghm : 
            {
            public:
                hghm(void const *data, size_t szData) :
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
