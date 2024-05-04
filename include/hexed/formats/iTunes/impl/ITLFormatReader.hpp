#ifndef HEXED_ITUNES_ITL_FORMAT_READER_IPP_
#define HEXED_ITUNES_ITL_FORMAT_READER_IPP_

#include <hexed/Log.hpp>
#include <hexed/formats/iTunes/detail/file.hpp>

#include <hexed/formats/iTunes/detail/envelope.hpp>
#include <hexed/formats/iTunes/detail/halm.hpp>
#include <hexed/formats/iTunes/detail/hdfm.hpp>
#include <hexed/formats/iTunes/detail/hdsm.hpp>
#include <hexed/formats/iTunes/detail/hghm.hpp>
#include <hexed/formats/iTunes/detail/hilm.hpp>
#include <hexed/formats/iTunes/detail/hohm.hpp>
#include <hexed/formats/iTunes/detail/hplm.hpp>
#include <hexed/formats/iTunes/detail/hqlm.hpp>
#include <hexed/formats/iTunes/detail/hslm.hpp>
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
        static std::size_t process_track_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::htim<_Order> htim(s);

            htim.foreach([&library](blessed::span<blessed::byte> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::TrackAdapter &, blessed::span<blessed::byte>)>> const functors =
                {
                    { detail::htlm<blessed::endian::big>::identifier(), process_track_metadata<blessed::endian::big> },
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

            return s.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_purchased_track_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            using htim = detail::htim<_Order>;

            htim header(s);

            header.foreach([&library](blessed::span<blessed::byte> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::TrackAdapter &, blessed::span<blessed::byte>)>> const functors =
                {
                    { detail::htlm<blessed::endian::big>::identifier(), process_track_metadata<blessed::endian::big> },
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

            return s.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_hdfm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::hdfm<_Order> segment(s);

            INFO("hdfm : segment {0}", segment.header_length());

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_hghm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::hghm<_Order> segment(s);

            INFO("hghm : segment {0}", segment.header_length());

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_halm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::halm<_Order> segment(s);

            INFO("halm : segment {0}", segment.header_length());

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_hplm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::hplm<_Order> segment(s);

            INFO("hplm : segment {0}", segment.header_length());

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_hqlm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::hqlm<_Order> segment(s);

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_hilm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::hilm<_Order> segment(s);

            INFO("hilm : segment {0}", segment.header_length());

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_hslm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::hslm<_Order> segment(s);

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_htlm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            detail::htlm<_Order> segment(s);

            INFO("htlm : segment {0}", segment.header_length());

            return segment.header_length();
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_5(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_6(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_7(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_8(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_10(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_15(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_16(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_17(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_18(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_19(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_20(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_21(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static size_t process_hdsm(detail::LibraryAdapter &library, blessed::span<blessed::byte> s)
        {
            using hdsm = detail::hdsm<_Order>;

            static std::map<typename hdsm::section_type, std::function<size_t(detail::LibraryAdapter &, blessed::span<blessed::byte>)>> const functors =
            {
                { hdsm::track_metadata, process_track_metadata<_Order> },
                { hdsm::playlist_metadata, process_track_metadata<_Order> },
                { hdsm::file_metadata, process_track_metadata<_Order> },
                { hdsm::subtype_unknown_5, process_subtype_5<_Order> },
                { hdsm::subtype_unknown_6, process_subtype_6<_Order> },
                { hdsm::subtype_unknown_7, process_subtype_7<_Order> },
                { hdsm::subtype_unknown_8, process_subtype_8<_Order> },
                { hdsm::album_metadata, process_track_metadata<_Order> },
                { hdsm::subtype_unknown_10, process_subtype_10<_Order> },
                { hdsm::subtype_hilm, process_hilm<_Order> },
                { hdsm::subtype_hghm, process_hghm<_Order> },
                { hdsm::purchased_track_metadata, process_purchased_track_metadata<_Order> },
                { hdsm::subtype_hdfm, process_hdfm<_Order> },
                { hdsm::subtype_unknown_15, process_subtype_15<_Order> },
                { hdsm::subtype_unknown_16, process_subtype_16<_Order> },
                { hdsm::subtype_unknown_17, process_subtype_17<_Order> },
                { hdsm::subtype_unknown_18, process_subtype_18<_Order> },
                { hdsm::subtype_unknown_19, process_subtype_19<_Order> },
                { hdsm::subtype_unknown_20, process_subtype_20<_Order> },
                { hdsm::subtype_unknown_21, process_subtype_21<_Order> },
            };

            hdsm header(s);

            INFO("hdsm : segment {0:x} header {1} payload {2} type {3}", header.mnemonic(), header.header_length(), header.payload().size(), header.type());

            auto it = functors.find(header.type());

            if(it != functors.end())
            {
                it->second(library, header.payload());
            }

            return header.length();
        }

        std::unique_ptr<hexed::Library> ITLFormatReader::read(std::string const &path)
        {
            std::unique_ptr<hexed::Library> library(new hexed::ImmutableLibrary());
            detail::file f(path);

            log_uint32({f.data().data(), 144});

            detail::basic_segment<> segment(f.data());

            detail::basic_segment<blessed::endian::big> envelope(f.data());
            detail::LibraryAdapter l;

            auto p = envelope.data().subspan(envelope.header_length());

            std::size_t offset{};

            while(offset < p.size())
            {
                auto sub = p.subspan(offset);
                detail::basic_segment<> segment(sub);

                INFO("envelope : {0:x}", segment.mnemonic());

                static std::map<uint32_t, std::function<size_t(detail::LibraryAdapter &, blessed::span<blessed::byte>)>> const functors =
                {
                    /*
                    { detail::hdfm<blessed::endian::big>::identifier(), process_hdfm<blessed::endian::big> },
                    { detail::hdfm<blessed::endian::little>::identifier(), process_hdfm<blessed::endian::little> },
                    */
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
                    throw std::runtime_error("unexpected child of envelope");
                }
            }
            
            return library;
        }
    }
}

#endif
