#ifndef HEXED_TRACK_HPP_
#define HEXED_TRACK_HPP_

#include <string>

#include "album.hpp"

namespace hexed
{
    class Track
    {
    public:

        void setTitle(std::string &&title)
        {
            title_ = title;
        }

    private:
        std::string title_;
        std::string artist_;
        std::string album_;
    };
}

#endif
