#ifndef HEXED_DETAIL_MAPPED_FILE_HPP_
#define HEXED_DETAIL_MAPPED_FILE_HPP_

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

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
                    ::fstat(fd, &st);
                    if((ptr_ = ::mmap(nullptr, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) != MAP_FAILED)
                    {

                    }

                    ::close(fd);
                }
            }

            ~MemoryMappedFile()
            {
                ::munmap(ptr_, size_);
            }

            void *data() { return ptr_; }
            size_t size() const { return size_; }

        private:
            void *ptr_;
            size_t size_;
        };
    }
}

#endif
