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
            class halm
            {
            public:
                hiim(void const *data, size_t szData) : 
                    segment_(data, szData)
                {

                }

            private:
                dictionary_segment<_Order> segment_;
            };
        }
    }
}

#endif
