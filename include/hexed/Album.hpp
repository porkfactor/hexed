#ifndef HEXED_ALBUM_HPP_
#define HEXED_ALBUM_HPP_

#include <string>

namespace hexed
{
    class Album
    {
    public:
        typedef std::string string_type;

        Album();
        ~Album();

        void setTitle(string_type &&title) { title_ = title; };
        void setArtist(string_type &&artist) { artist_ = artist; }

        string_type const &getTitle() const { return title_; }
        string_type const &getArtist() const { return artist_; }

    private:
        string_type title_;
        string_type artist_;
    };
}

#include <hexed/impl/Album.hpp>

#endif
