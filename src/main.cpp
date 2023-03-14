#include <hexed/LibraryFormatManager.hpp>

int main(int argc, char const *argv[])
{
    hexed::LibraryFormatManager lfm;

    lfm.registerBasicFormats();

    auto f = lfm.findFormatForFileExtension("itl");
    auto r = f->createReader();

    r->read(argv[1]);

    return 0;
}