#ifndef HEXED_LIBRARY_FORMAT_MANAGER_HPP_
#define HEXED_LIBRARY_FORMAT_MANAGER_HPP_

#include <map>

#include <hexed/LibraryFormat.hpp>
#include <hexed/LibraryFormatReader.hpp>

namespace hexed
{
    class LibraryFormatThing
    {
    public:
        LibraryFormatThing(std::string const &, std::shared_ptr<LibraryFormat>);

        std::shared_ptr<LibraryFormat> format() const
        {
            return format_;
        }

        std::string const &path() const
        {
            return path_;
        }

    private:
        std::shared_ptr<LibraryFormat> format_;
        std::string path_;
    };

    class LibraryFormatManager
    {
    public:

        bool registerFormat(std::shared_ptr<LibraryFormat> format, bool setDefault);
        void registerBasicFormats();

        std::shared_ptr<LibraryFormat> findFormatForFileExtension(std::string const &e);

        std::vector<LibraryFormatThing> scan(void);

    private:
        std::map<int, std::shared_ptr<LibraryFormat>> formats_;
    };
}

#endif
