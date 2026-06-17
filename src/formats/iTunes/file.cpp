#include <vector>

#include <blessed/memory.hpp>
#include <hexed/detail/Compression.hpp>
#include <hexed/detail/Encryption.hpp>
#include <hexed/Log.hpp>
#include <formats/iTunes/file.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
        #if 0
            template<blessed::endian _Order>
            void file::process_itl(void)
            {
                using hdfm = detail::hdfm<_Order>;
                
                hdfm envelope(mapping_.data());

                blessed::unique_c_ptr<blessed::byte> raw(blessed::malloc_unique<blessed::byte>(envelope.crypt_length()));

                auto payload = envelope.payload();

                hexed::detail::aes128decrypt(raw.get(), envelope.crypt_length(), static_cast<void const *>(payload.data()), envelope.crypt_length());

                std::copy(raw.get(), raw.get() + envelope.crypt_length(), const_cast<blessed::byte *>(payload.data()));

                std::vector<blessed::byte> v;
                hexed::detail::inflate(payload.begin(), payload.end(), std::back_inserter(v));

                data_ = blessed::malloc_unique<blessed::byte>(v.size() + envelope.header().size());

                auto end = std::copy(envelope.header().begin(), envelope.header().end(), data_.get());
                end = std::copy(v.begin(), v.end(), end);

                szData_ = static_cast<std::size_t>(reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(data_.get()));

                INFO("inflated size {0}", szData_);
            }

            template<blessed::endian _Order>
            void file::process_musicdb(void)
            {
                using hfma = detail::hfma<_Order>;
                
                hfma envelope(mapping_.data());

                blessed::unique_c_ptr<blessed::byte> raw(blessed::malloc_unique<blessed::byte>(envelope.crypt_length()));

                auto payload = envelope.payload();

                hexed::detail::aes128decrypt(raw.get(), envelope.crypt_length(), static_cast<void const *>(payload.data()), envelope.crypt_length());

                std::copy(raw.get(), raw.get() + envelope.crypt_length(), const_cast<blessed::byte *>(payload.data()));

                std::vector<blessed::byte> v;
                hexed::detail::inflate(payload.begin(), payload.end(), std::back_inserter(v));

                data_ = blessed::malloc_unique<blessed::byte>(v.size() + envelope.header().size());

                auto end = std::copy(envelope.header().begin(), envelope.header().end(), data_.get());
                end = std::copy(v.begin(), v.end(), end);

                szData_ = static_cast<std::size_t>(reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(data_.get()));

                INFO("inflated size {0}", szData_);
            }
            #endif
        

            file::file(std::string const &path) :
                mapping_(path)
            {
                // detail::basic_segment<blessed::endian::native> seg(mapping_.data());

            #if 0
                switch (seg.mnemonic())
                {
                case detail::hdfm<blessed::endian::big>::identifier():
                    process_itl<blessed::endian::big>();
                    break;
                case detail::hdfm<blessed::endian::little>::identifier():
                    process_itl<blessed::endian::little>();
                    break;
                case detail::hfma<blessed::endian::little>::identifier():
                    process_musicdb<blessed::endian::little>();
                    break;
                case detail::hfma<blessed::endian::big>::identifier():
                    process_musicdb<blessed::endian::big>();
                    break;
                }
            #endif
            }

            blessed::span<blessed::byte const> file::data() const
            {
                return blessed::span<blessed::byte const>{ static_cast<blessed::byte const *>(data_.get()), szData_ };
            }
        }
    }
}
