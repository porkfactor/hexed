#ifndef HEXED_ITUNES_DETAIL_FILE_IPP_
#define HEXED_ITUNES_DETAIL_FILE_IPP_

#include <blessed/memory.hpp>
#include <hexed/detail/Compression.hpp>
#include <hexed/detail/Encryption.hpp>
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
                detail::envelope<blessed::endian::big> envelope(mapping_.data());

                blessed::unique_c_ptr<blessed::byte> raw(blessed::malloc_unique<blessed::byte>(envelope.crypt_length()));

                auto payload = envelope.payload();

                hexed::detail::aes128decrypt(raw.get(), envelope.crypt_length(), static_cast<void *>(payload.data()), envelope.crypt_length());

                std::copy(raw.get(), raw.get() + envelope.crypt_length(), payload.data());

                std::vector<blessed::byte> v;
                hexed::detail::inflate(payload.begin(), payload.end(), std::back_inserter(v));

                data_ = blessed::malloc_unique<blessed::byte>(v.size() + envelope.header_length());

                auto end = std::copy(envelope.data().begin(), envelope.data().begin() + envelope.header_length(), data_.get());
                end = std::copy(v.begin(), v.end(), end);

                szData_ = static_cast<std::size_t>(reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(data_.get()));

                INFO("inflated size {0}", szData_);
            }

            blessed::span<blessed::byte> file::data() const
            {
                return blessed::span<blessed::byte>{ static_cast<blessed::byte *>(data_.get()), szData_ };
            }
        }
    }
}

#endif
