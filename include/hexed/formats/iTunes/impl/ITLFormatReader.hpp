#ifndef HEXED_ITUNES_ITL_FORMAT_READER_IPP_
#define HEXED_ITUNES_ITL_FORMAT_READER_IPP_

#include <hexed/Log.hpp>
#include <hexed/formats/iTunes/detail/file.hpp>

#include <hexed/formats/iTunes/detail/envelope.hpp>
#include <hexed/formats/iTunes/detail/hdfm.hpp>
#include <hexed/formats/iTunes/detail/hdsm.hpp>
#include <hexed/formats/iTunes/detail/hohm.hpp>
#include <hexed/formats/iTunes/detail/htim.hpp>
#include <hexed/formats/iTunes/detail/htlm.hpp>


void log_uint32(blessed::span<blessed::byte> s)
{
    blessed::span<uint32_t> data = blessed::reinterpret_as<uint32_t>(s);
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

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            class LibraryAdapter
            {
                template<blessed::endian _Order>
                void process(detail::hohm<_Order> const &attribute)
                {

                }
            };

            class AlbumAdapter
            {
                template<blessed::endian _Order>
                void process(detail::hohm<_Order> const &attribute)
                {

                }
            };

            class TrackAdapter
            {
                template<blessed::endian _Order>
                void process(detail::hohm<_Order> const &attribute)
                {

                }
            };
        }

        ITLFormatReader::ITLFormatReader()
        {

        }

        ITLFormatReader::~ITLFormatReader()
        {

        }

        template<blessed::endian _Order>
        static std::size_t process_album_attribute(detail::AlbumAdapter &album, blessed::span<blessed::byte> data)
        {
            return data.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_track_attribute(detail::TrackAdapter &track, blessed::span<blessed::byte> data)
        {
            detail::hohm<_Order> hohm(data);

            switch(hohm.type())
            {
            case detail::hohm<_Order>::album_artist:
            case detail::hohm<_Order>::album_title:
            case detail::hohm<_Order>::track_title:
            case detail::hohm<_Order>::local_path:
                break;
            }

            return data.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_track_metadata(detail::TrackAdapter &track, blessed::span<blessed::byte> data)
        {
            detail::htlm<_Order> htlm(data);

            htlm.foreach([&track](blessed::span<blessed::byte> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::TrackAdapter &, blessed::span<blessed::byte>)>> const functors =
                {
                    { detail::hohm<blessed::endian::big>::identifier(), process_track_attribute<blessed::endian::big> },
                    { detail::hohm<blessed::endian::little>::identifier(), process_track_attribute<blessed::endian::little> },
                };

                detail::basic_segment<> segment(s);
                size_t offset{};

                auto it = functors.find(segment.mnemonic());

                if(it != functors.end())
                {
                    offset = it->second(track, s);
                }
                else
                {
                    throw std::runtime_error("unexpected child of htlm record");
                };

                return offset;
            });

            return data.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_track_list(detail::LibraryAdapter &library, blessed::span<blessed::byte> data)
        {
            detail::htim<_Order> htim(data);

            htim.foreach([&library](blessed::span<blessed::byte> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::TrackAdapter &, blessed::span<blessed::byte>)>> const functors =
                {
                    { detail::htlm<blessed::endian::little>::identifier(), process_track_metadata<blessed::endian::big> },
                    { detail::htlm<blessed::endian::little>::identifier(), process_track_metadata<blessed::endian::little> },
                };

                detail::TrackAdapter track;
                detail::basic_segment<> segment(s);
                size_t offset{};

                auto it = functors.find(segment.mnemonic());

                if(it != functors.end())
                {
                    offset = it->second(track, s);
                }
                else
                {
                    throw std::runtime_error("unexpected child of htim record");
                }

                return offset;
            });

            return data.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_hdfm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::hdfm<_Order> segment(s);

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_hdsm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::hdsm<_Order> segment(s);

            switch(segment.type())
            {
            case detail::hdsm<_Order>::track_metadata:
            case detail::hdsm<_Order>::album_metadata:
            case detail::hdsm<_Order>::purchased_track_metadata:
                break;
            }

            return segment.length();
        }

        std::unique_ptr<hexed::Library> ITLFormatReader::read(std::string const &path)
        {
            std::unique_ptr<hexed::Library> library(new hexed::ImmutableLibrary());
            detail::file f(path);

            log_uint32({f.data().data(), 144});

            detail::basic_segment<> segment(f.data());

            detail::basic_segment<blessed::endian::big> envelope(f.data());
            detail::LibraryAdapter l;

            auto p = envelope.payload();

            std::size_t offset{};

            while(offset < p.size())
            {
                auto sub = p.subspan(offset);
                detail::basic_segment<> segment(sub);

                INFO("envelope : {0:x}", segment.mnemonic());

                static std::map<uint32_t, std::function<size_t(detail::LibraryAdapter &, blessed::span<blessed::byte>)>> const functors =
                {
                    { detail::hdfm<blessed::endian::big>::identifier(), process_hdfm<blessed::endian::big> },
                    { detail::hdfm<blessed::endian::little>::identifier(), process_hdfm<blessed::endian::little> },
                    { detail::hdsm<blessed::endian::big>::identifier(), process_hdsm<blessed::endian::big> },
                    { detail::hdsm<blessed::endian::little>::identifier(), process_hdsm<blessed::endian::little> },
                };

                auto it = functors.find(segment.mnemonic());

                if(it != functors.end())
                {
                    offset += it->second(l, sub);
                }
                else
                {
                    throw std::runtime_error("balls");
                }
            }
            
            return library;
        }
    }
}

#endif
