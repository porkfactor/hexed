#include <hexed/LibraryFormat.hpp>
#include <hexed/LibraryFormatManager.hpp>

namespace hexed
{
    LibraryFormat::LibraryFormat()
    {}

    LibraryFormat::~LibraryFormat()
    {}

    std::vector<std::string> LibraryFormat::scan(void) const
    {
        return std::vector<std::string>{};
    }
}
