#ifndef HEXED_COMPRESSION_HPP_
#define HEXED_COMPRESSION_HPP_

#include <cstring>
#include <zlib.h>

namespace hexed
{
    namespace detail
    {
        template<typename _InputIterator, typename _OutputIterator>
        _OutputIterator inflate(_InputIterator first, _InputIterator last, _OutputIterator dest)
        {
            z_stream stream;
            int result{Z_OK};

            typename _InputIterator::value_type buffer[1024];

            ::memset(&stream, 0, sizeof(stream));

            stream.avail_in = std::distance(first, last);
            stream.next_in = reinterpret_cast<Bytef *>(std::addressof(*first));

            inflateInit(&stream);

            do
            {
                stream.next_out = reinterpret_cast<Bytef *>(buffer);
                stream.avail_out = sizeof(buffer);

                result = ::inflate(&stream, Z_NO_FLUSH);

                dest = std::copy(buffer, buffer + (sizeof(buffer) - stream.avail_out), dest);
            }
            while (stream.avail_out == 0);

            inflateEnd(&stream);

            return dest;
        }

        size_t inflate(void *dest, size_t szDest, void const *source, size_t szSource)
        {
        #if 0
            compression_algorithm algorithm = COMPRESSION_LZBITMAP;

            size_t bufsize = compression_decode_scratch_buffer_size(algorithm);
            void *scratch = ::malloc(bufsize);

            return compression_decode_buffer(
                static_cast<uint8_t *>(dest),
                szDest,
                static_cast<uint8_t const *>(source),
                szSource,
                scratch,
                algorithm);
        #else
            z_stream stream;
            unsigned int chunk_size = 8192;

            ::memset(&stream, 0, sizeof(stream));

            stream.avail_in = szSource;
            stream.avail_out = szDest;
            stream.next_out = (Bytef *)dest;
            stream.next_in = (Bytef *)source;

            inflateInit(&stream);

            int result = Z_OK;

            while(result == Z_OK)
            {
                result = inflate(&stream, Z_NO_FLUSH);
            }
            
            inflateEnd(&stream);

            return stream.total_out;
        #endif
        }
    }
}

#endif
