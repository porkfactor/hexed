#ifndef HEXED_TRACK_HPP_
#define HEXED_TRACK_HPP_

#include <string>
#include <chrono>

#include "album.hpp"

namespace hexed
{
    class Track
    {
    public:
        typedef std::string string_type;

        void setTitle(string_type &&title) { title_ = title; }
        void setAlbum(string_type &&album) { album_ = album; }
        void setArtist(string_type &&artist) { artist_ = artist; }
        void setComposer(string_type &&composer) { composer_ = composer; }
        void setAlbumArtist(string_type &&albumArtist) { albumArtist_ = albumArtist; }
        void setPath(string_type &&path) { path_ = path; }
        void setSize(std::size_t size) { size_ = size; }

    private:
        string_type title_;
        string_type album_;
        string_type artist_;
        string_type composer_;
        string_type albumArtist_;
        string_type path_;
        std::chrono::milliseconds duration_;
        std::size_t size_;
    };
}

#endif
