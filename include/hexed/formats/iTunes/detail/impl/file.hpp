#ifndef HEXED_ITUNES_DETAIL_FILE_IPP_
#define HEXED_ITUNES_DETAIL_FILE_IPP_

#include <hexed/formats/iTunes/detail/file.hpp>
#include <hexed/formats/iTunes/detail/envelope.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            file::file(std::string const &path) :
                mapping_(path)
            {
                detail::envelope<blessed::endian::big> envelope(mapping_.data(), mapping_.size());

                uint32_t thing_length = 16777216 * 4;

                spdlog::info("segment size {}", envelope.header_length());
                spdlog::info("data size {}", envelope.data_length());

                unique_c_ptr<void> raw(malloc_unique(envelope.crypt_length()));
                unique_c_ptr<void> uncompressed(malloc_unique(thing_length));

                do_decrypt(raw.get(), envelope.crypt_length(), envelope.payload(), envelope.crypt_length());

                ::memcpy(const_cast<void *>(envelope.payload()), raw.get(), envelope.crypt_length());
                
                size_t szData = inflate(uncompressed.get(), thing_length, envelope.payload(), envelope.payload_length());

                spdlog::info("inflated size {0}", szData);
            }
        }
    }
}

#endif
