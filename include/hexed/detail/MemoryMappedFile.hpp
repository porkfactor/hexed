#ifndef HEXED_DETAIL_MAPPED_FILE_HPP_
#define HEXED_DETAIL_MAPPED_FILE_HPP_

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <blessed/span.hpp>

namespace hexed
{
    namespace detail
    {
        struct MemoryMappedFile
        {
        public:
            MemoryMappedFile(std::string const &path)
            {
                int fd = -1;
                struct stat st;

                if((fd = ::open(path.c_str(), O_RDONLY)) >= 0)
                {
                    void *ptr{};
                    ::fstat(fd, &st);

                    if((ptr = ::mmap(nullptr, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) != MAP_FAILED)
                    {
                        data_ = blessed::span<blessed::byte>(static_cast<blessed::byte *>(ptr), st.st_size);
                    }

                    ::close(fd);
                }
            }

            ~MemoryMappedFile()
            {
                ::munmap(data_.data(), data_.size());
            }

            inline blessed::span<blessed::byte> const &data() const noexcept { return data_; }

        private:
            blessed::span<blessed::byte> data_;
        };
    }
}

#endif
