#ifndef HEXED_LIBRARY_HPP_
#define HEXED_LIBRARY_HPP_

namespace hexed
{
    class Library
    {
    public:

    protected:
        Library() {}

    private:
    };

    class ImmutableLibrary : public Library
    {
    public:
        ImmutableLibrary() :
            Library()
        {}

    private:
    };

    class MutableLibrary : public Library
    {
    public:
        MutableLibrary() :
            Library()
        {}

    private:
    };
}

#endif
