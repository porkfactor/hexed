#ifndef HEXED_ALBUM_HPP_
#define HEXED_ALBUM_HPP_

#include <string>

namespace hexed
{
    class Album
    {
    public:
        Album();
        ~Album();

        void setTitle(std::string const &title) { title_ = title; };
        void setArtist(std::string const &artist) { artist_ = artist; }

        std::string const &getTitle() const { return title_; }
        std::string const &getArtist() const { return artist_; }

    private:
        std::string title_;
        std::string artist_;
    };
}

#endif
