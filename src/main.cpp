#include <hexed/LibraryFormatManager.hpp>

int main(int argc, char const *argv[])
{
    hexed::LibraryFormatManager lfm;

    lfm.registerBasicFormats();

    auto candidates = lfm.scan();

    for (auto const &candidate : candidates)
    {
        auto r = candidate.format()->createReader();
    
        r->read(candidate.path());
    }

    return 0;
}
