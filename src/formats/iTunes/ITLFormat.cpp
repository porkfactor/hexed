#include <formats/iTunes/ITLFormat.hpp>
#include <formats/iTunes/ITLFormatReader.hpp>

namespace hexed
{
    namespace itunes
    {
        ITLFormat::ITLFormat()
        {

        }

        ITLFormat::~ITLFormat()
        {

        }

        std::shared_ptr<hexed::LibraryFormatReader> ITLFormat::createReader(void)
        {
            return std::make_shared<ITLFormatReader>();
        }
    }
}
