#ifndef HEXED_LIBRARY_FORMAT_HPP_
#define HEXED_LIBRARY_FORMAT_HPP_

#include <memory>
#include <vector>

namespace hexed
{
    class LibraryFormatReader;
    class LibraryFormatThing;

    class LibraryFormat :
        public std::enable_shared_from_this<LibraryFormat>
    {
    public:
        LibraryFormat();
        virtual ~LibraryFormat();

        virtual std::shared_ptr<LibraryFormatReader> createReader(void) = 0;
        virtual std::vector<std::string> scan(void) const;

    private:
    };
}

#endif
