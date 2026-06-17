#include <blessed/byteswap.hpp>

#include <hexed/Log.hpp>

void log_data(blessed::span<blessed::byte const> s)
{
    static std::size_t constexpr max_words = 4;
    static std::size_t constexpr max_bytes = max_words * sizeof(uint32_t);

    blessed::span<uint32_t const> data = blessed::reinterpret_as<uint32_t>(s);
    size_t offset = 0;

    char line[128];

    for(size_t i = 0; i < data.size(); i += 8)
    {
        offset = 0;
        size_t limit = std::min(max_bytes, s.size() - (i * sizeof(uint32_t)));

        for(size_t j = 0; j < 4; ++j)
        {
            if((i + j) >= data.size())
                break;

            if(j)
            {
                offset += snprintf(line + offset, sizeof(line) - offset, " %08x", be32toh(data[i + j]));
            }
            else
            {
                offset += snprintf(line + offset, sizeof(line) - offset, "%08x", be32toh(data[i + j]));
            }
        }

        offset += snprintf(line + offset, sizeof(line) - offset, " ");

        for (size_t j = 0; j < limit; ++j)
        {
            offset += snprintf(line + offset, sizeof(line) - offset, "%02x", s[(i * 4) + j]);
        }

        offset += snprintf(line + offset, sizeof(line) - offset, "%-*s ", static_cast<int>(max_bytes - limit) ," ");

        for (size_t j = 0; j < limit; ++j)
        {
            char c = static_cast<char>(s[(i * 4) + j]);
            offset += snprintf(line + offset, sizeof(line) - offset, "%c", isprint(c) && !iscntrl(c) ? c : '.');
        }

        fwrite(line, sizeof(char), offset, stdout);
        fputc('\n', stdout);
    }

    fflush(stdout);
}

void log_uint32(blessed::span<blessed::byte const> s)
{
    blessed::span<uint32_t const> data = blessed::reinterpret_as<uint32_t>(s);
    size_t offset = 0;

    for(size_t i = 0; i < data.size(); i += 8)
    {
        for(size_t j = 0; j < 8; ++j)
        {
            if((offset + j) >= data.size())
                break;

            if(j)
            {
                fprintf(stdout, " %08x", be32toh(data[offset + j]));
            }
            else
            {
                fprintf(stdout, "%08x", be32toh(data[offset + j]));
            }
        }

        printf("\n");

        offset += 8;
    }
}
