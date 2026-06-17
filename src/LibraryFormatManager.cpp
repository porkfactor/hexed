#include <hexed/LibraryFormatManager.hpp>

#include <formats/iTunes/ITLFormat.hpp>
#include <formats/iTunes/MusicDbFormat.hpp>

namespace hexed
{
    LibraryFormatThing::LibraryFormatThing(std::string const &p, std::shared_ptr<LibraryFormat> f) :
        path_(p),
        format_(f)
    {}

    bool LibraryFormatManager::registerFormat(std::shared_ptr<LibraryFormat> format, bool setDefault)
    {
        bool rv{false};

        formats_.emplace(static_cast<int>(formats_.size()), format);

        return rv;
    }
    
    void LibraryFormatManager::registerBasicFormats()
    {
        registerFormat(std::make_shared<hexed::itunes::ITLFormat>(), false);
        registerFormat(std::make_shared<hexed::itunes::MusicDbLibraryFormat>(), false);
    }

    std::shared_ptr<LibraryFormat> LibraryFormatManager::findFormatForFileExtension(std::string const &s)
    {
        return formats_[0];
    }

    std::vector<LibraryFormatThing> LibraryFormatManager::scan(void)
    {
        std::vector<LibraryFormatThing> rv{};

        for (auto const &format : formats_)
        {
            auto paths = format.second->scan();
            
            for (auto const &path : paths)
            {
                rv.emplace_back(path, format.second);
            }
        }

        return rv;
    }
}
