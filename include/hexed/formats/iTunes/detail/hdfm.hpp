#ifndef HEXED_ITUNES_DETAIL_HDFM_HPP_
#define HEXED_ITUNES_DETAIL_HDFM_HPP_

#include <hexed/formats/iTunes/detail/segment.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            template<blessed::endian _Order>
            class hdfm : public data_segment<_Order>
            {

            };
        }
    }
}

#endif
