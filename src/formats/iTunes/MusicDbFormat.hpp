#ifndef HEXED_ITUNES_MUSICDBFORMAT_HPP_
#define HEXED_ITUNES_MUSICDBFORMAT_HPP_

#include <hexed/LibraryFormat.hpp>

namespace hexed
{
    namespace itunes
    {
        class MusicDbLibraryFormat : public LibraryFormat
        {
        public:
            std::shared_ptr<LibraryFormatReader> createReader(void) override;

            std::vector<std::string> scan(void) const override;

            static std::string getUserHomeFolder(void);
            static std::string getDefaultPreferencesPath(void);

        private:
        };
    }
}

#endif
