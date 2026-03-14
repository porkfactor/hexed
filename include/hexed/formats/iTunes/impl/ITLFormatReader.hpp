#ifndef HEXED_ITUNES_ITL_FORMAT_READER_IPP_
#define HEXED_ITUNES_ITL_FORMAT_READER_IPP_

#include <hexed/Log.hpp>
#include <hexed/formats/iTunes/detail/file.hpp>

#include <hexed/formats/iTunes/detail/envelope.hpp>
#include <hexed/formats/iTunes/detail/haim.hpp>
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

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            class LibraryAdapter
            {
            public:
                template<blessed::endian _Order>
                void process(detail::hohm<_Order> const &attribute)
                {

                }
            };

            class AlbumAdapter
            {
            public:
                template<blessed::endian _Order>
                void process(detail::hohm<_Order> const &attribute)
                {
                    enum
                    {
                        album_title = 0x12c,
                        album_artist = 0x12d,
                        artist = 0x12e,
                        composer = 0x0,
                        series_title = 0x130,
                    };

                    switch (attribute.subtype())
                    {
                    case album_title:
                        {
                            detail::flex_string<_Order> s(attribute.payload());
                            INFO("album title: {0}", s.u8().c_str());
                        }
                        break;
                    case album_artist:
                        {
                            detail::flex_string<_Order> s(attribute.payload());
                            INFO("album title: {0}", s.u8().c_str());
                        }
                        break;
                    case artist:
                        {
                            detail::flex_string<_Order> s(attribute.payload());
                            INFO("album title: {0}", s.u8().c_str());
                        }
                        break;
                    default:
                        INFO("unknown attribute {0}", attribute.subtype());
                        log_data(attribute.payload());
                    }
                }
            };

            class TrackAdapter
            {
            public:
                template<blessed::endian _Order>
                void process(detail::hohm<_Order> const &attribute)
                {
                    enum
                    {
                        track_title = 0x02,
                        album_title = 0x03,
                        artist = 0x04,
                        genre = 0x05,
                        file_type = 0x06,
                        comments = 0x08,
                        local_path = 0x0b,
                        composer = 0x0c,
                        grouping = 0x0e,
                        unknown1 = 0x12,
                        album_artist = 0x1b,
                        xml_block_asset_info = 0x1d,
                        sort_order_track_title = 0x1e,
                        sort_order_album_title = 0x1f,
                        sort_order_artist = 0x20,
                        sort_order_album_artist = 0x21,
                        sort_composer = 0x22,
                        isrc = 0x2b,
                        copyright = 0x2e,
                        unknown2 = 0x34,
                        xml_block_artwork = 0x36,
                        unknown3 = 0x38, // XML - price?
                        purchaser_email = 0x3b,
                        purchaser_name = 0x3c,
                        unknown4 = 0x3f, // 8-byte integer?
                        unknown5 = 0x42,
                        playlist_name = 0x64,
                        smart_criteria = 0x65,
                        library_name = 0x1fc,
                    };

                    switch(attribute.subtype())
                    {
                    case album_artist:
                        {
                            detail::flex_string<_Order> s(attribute.payload());
                            INFO("artist: {0}", s.u8().c_str());
                        }
                        break;
                    case album_title:
                        {
                            detail::flex_string<_Order> s(attribute.payload());
                            INFO("album title: {0}", s.u8().c_str());
                        }
                        break;
                    case track_title:
                        {
                            detail::flex_string<_Order> s(attribute.payload());
                            INFO("track title: {0}", s.u8().c_str());
                        }
                        break;
                    case local_path:
                        {
                            detail::flex_string<_Order> s(attribute.payload());
                            INFO("path: {0}", s.u8().c_str());
                        }
                        break;
                    case artist:
                    case genre:
                    case file_type:
                    case comments:
                    case composer:
                    case grouping:
                    case xml_block_asset_info:
                    case sort_order_track_title:
                    case sort_order_album_title:
                    case sort_order_artist:
                    case sort_order_album_artist:
                    case sort_composer:
                    case isrc:
                    case copyright:
                    case xml_block_artwork:
                    case purchaser_email:
                    case purchaser_name:
                    case playlist_name:
                    case smart_criteria:
                    case library_name:
                        break;
                    case unknown1:
                    case unknown2:
                    case unknown3:
                    case unknown4:
                    case unknown5:
                        break;
                    default:
                        INFO("unknown attribute {0}", attribute.subtype());
                        log_data(attribute.payload());
                    }
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
        static std::size_t process_track_attribute(detail::TrackAdapter &track, blessed::span<blessed::byte const> data)
        {
            using hohm = detail::hohm<_Order>;

            hohm header(data);
            INFO("hohm : segment {0:x} header {1} payload {2} subutype {3:08x}", header.mnemonic(), header.header().size(), header.payload().size(), header.subtype());

            track.process(header);

            return header.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_trackx_metadata(detail::TrackAdapter &track, blessed::span<blessed::byte const> data)
        {
            detail::htlm<_Order> htlm(data);

            htlm.foreach([&track](blessed::span<blessed::byte const> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::TrackAdapter &, blessed::span<blessed::byte const>)>> const functors =
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
                    INFO("unexpected child {}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htlm record");
                };

                return offset;
            });

            return data.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_htim(detail::TrackAdapter &track, blessed::span<blessed::byte const> data)
        {
            using htim = detail::htim<_Order>;
            htim header(data);

            INFO("htim : segment {0:x} header {1} payload {2} count {3}", header.mnemonic(), header.header().size(), header.payload().size(), static_cast<int>(header.count()));

            log_data(header.header());

            header.foreach([&track](blessed::span<blessed::byte const> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::TrackAdapter &, blessed::span<blessed::byte const>)>> const functors =
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
                    INFO("unexpected child {:08x}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htim record");
                };

                return offset;
            });

            return header.length();
        }

        template<blessed::endian _Order>
        static std::size_t process_track_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using htlm = detail::htlm<_Order>;

            htlm header(data);

            INFO("htlm : segment {0:x} header {1} payload {2} count {3}", header.mnemonic(), header.header().size(), header.payload().size(), static_cast<int>(header.count()));

            header.foreach([&library](blessed::span<blessed::byte const> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::TrackAdapter &, blessed::span<blessed::byte const>)>> const functors =
                {
                    //{ detail::htlm<blessed::endian::big>::identifier(), process_htlm<blessed::endian::big> },
                    //{ detail::htlm<blessed::endian::little>::identifier(), process_htlm<blessed::endian::little> },
                    { detail::htim<blessed::endian::big>::identifier(), process_htim<blessed::endian::big> },
                    { detail::htim<blessed::endian::little>::identifier(), process_htim<blessed::endian::little> },
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
                    INFO("unexpected child {:08x}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htlm record");
                }

                return offset;
            });

            return data.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_purchased_track_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using htlm = detail::htlm<_Order>;

            htlm header(data);

            INFO("htlm : segment {0:x} header {1} payload {2} count {3}", header.mnemonic(), header.size(), header.payload().size(), static_cast<int>(header.count()));

            header.foreach([&library](blessed::span<blessed::byte const> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::TrackAdapter &, blessed::span<blessed::byte const>)>> const functors =
                {
                    { detail::htim<blessed::endian::big>::identifier(), process_htim<blessed::endian::big> },
                    { detail::htim<blessed::endian::little>::identifier(), process_htim<blessed::endian::little> },
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
                    INFO("unexpected child {}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htlm record");
                }

                return offset;
            });

            return data.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_hdfm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            detail::hdfm<_Order> segment(s);

            INFO("hdfm : segment {0}", segment.size());

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_hghm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            detail::hghm<_Order> segment(s);

            INFO("hghm : segment {0}", segment.size());

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_album_attribute(detail::AlbumAdapter &album, blessed::span<blessed::byte const> data)
        {
            using hohm = detail::hohm<_Order>;

            hohm header(data);
            INFO("hohm : segment {0:x} header {1} payload {2} subutype {3:08x}", header.mnemonic(), header.header().size(), header.payload().size(), header.subtype());

            album.process(header);

            return header.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_haim(detail::AlbumAdapter &album, blessed::span<blessed::byte const> data)
        {
            using haim = detail::haim<_Order>;

            haim header(data);

            INFO("haim : segment {0:x} header {1} payload {2} count {3}", header.mnemonic(), header.header().size(), header.payload().size(), static_cast<int>(header.count()));

            log_data(header.header());

            header.foreach([&album](blessed::span<blessed::byte const> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::AlbumAdapter &, blessed::span<blessed::byte const>)>> const functors =
                {
                    { detail::hohm<blessed::endian::big>::identifier(), process_album_attribute<blessed::endian::big> },
                    { detail::hohm<blessed::endian::little>::identifier(), process_album_attribute<blessed::endian::little> },
                };

                detail::basic_segment<> segment(s);
                size_t offset{};

                auto it = functors.find(segment.mnemonic());

                if(it != functors.end())
                {
                    offset = it->second(album, s);
                }
                else
                {
                    INFO("unexpected child {:08x}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htim record");
                };

                return offset;
            });

            return header.length();
        }

        template<blessed::endian _Order>
        static std::size_t process_album_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using halm = detail::halm<_Order>;
            
            halm header(data);

            INFO("halm : segment {0:x} header {1} payload {2} count {3}", header.mnemonic(), header.size(), header.payload().size(), static_cast<int>(header.count()));

            header.foreach([&library](blessed::span<blessed::byte const> s)
            {
                static std::map<uint32_t, std::function<std::size_t(detail::AlbumAdapter &, blessed::span<blessed::byte const>)>> const functors =
                {
                    { detail::haim<blessed::endian::big>::identifier(), process_haim<blessed::endian::big> },
                    { detail::haim<blessed::endian::little>::identifier(), process_haim<blessed::endian::little> },
                };

                detail::AlbumAdapter album;
                detail::basic_segment<> segment(s);
                size_t offset{};

                auto it = functors.find(segment.mnemonic());

                if(it != functors.end())
                {
                    offset = it->second(album, s);
                }
                else
                {
                    INFO("unexpected child {}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htlm record");
                }

                return offset;
            });

            return data.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_playlist_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            using hplm = detail::hplm<_Order>;
            hplm segment(s);

            INFO("hplm : segment {0}", segment.size());

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_file_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            log_uint32(s);

            return s.size_bytes();
        }

        template<blessed::endian _Order>
        static std::size_t process_hplm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            using hplm = detail::hplm<_Order>;
            hplm segment(s);

            INFO("hplm : segment {0}", segment.size());

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_hqlm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            detail::hqlm<_Order> segment(s);

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_hilm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            detail::hilm<_Order> segment(s);

            INFO("hilm : segment {0}", segment.size());

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_hslm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            detail::hslm<_Order> segment(s);

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_htlm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            detail::htlm<_Order> segment(s);

            INFO("htlm : segment {0}", segment.size());

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_5(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_6(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_7(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_8(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_10(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_15(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_16(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_17(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_18(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_19(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_20(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static std::size_t process_subtype_21(detail::LibraryAdapter &library, blessed::span<blessed::byte const> s)
        {
            return 0;
        }

        template<blessed::endian _Order>
        static size_t process_hdsm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using hdsm = detail::hdsm<_Order>;

            static std::map<typename hdsm::section, std::function<size_t(detail::LibraryAdapter &, blessed::span<blessed::byte const>)>> const functors =
            {
                { hdsm::track_metadata, process_track_metadata<_Order> },
                { hdsm::playlist_metadata, process_playlist_metadata<_Order> },
                { hdsm::file_metadata, process_file_metadata<_Order> },
                { hdsm::subtype_unknown_5, process_subtype_5<_Order> },
                { hdsm::subtype_unknown_6, process_subtype_6<_Order> },
                { hdsm::subtype_unknown_7, process_subtype_7<_Order> },
                { hdsm::subtype_unknown_8, process_subtype_8<_Order> },
                { hdsm::album_metadata, process_album_metadata<_Order> },
                { hdsm::subtype_unknown_10, process_subtype_10<_Order> },
                { hdsm::subtype_hilm, process_hilm<_Order> },
                { hdsm::subtype_hghm, process_hghm<_Order> },
                { hdsm::purchased_track_metadata, process_purchased_track_metadata<_Order> },
                { hdsm::subtype_hdfm, process_hplm<_Order> },
                { hdsm::subtype_unknown_15, process_subtype_15<_Order> },
                { hdsm::subtype_unknown_16, process_subtype_16<_Order> },
                { hdsm::subtype_unknown_17, process_subtype_17<_Order> },
                { hdsm::subtype_unknown_18, process_subtype_18<_Order> },
                { hdsm::subtype_unknown_19, process_subtype_19<_Order> },
                { hdsm::subtype_unknown_20, process_subtype_20<_Order> },
                { hdsm::subtype_unknown_21, process_subtype_21<_Order> },
            };

            hdsm header(data);

            INFO("hdsm : segment {0:x} header {1} payload {2} size {3} type {4}", header.mnemonic(), header.header().size(), header.payload().size(), header.size(), static_cast<int>(header.type()));

            auto it = functors.find(header.type());

            if(it != functors.end())
            {
                it->second(library, header.payload());
            }

            return header.size();
        }

        std::unique_ptr<hexed::Library> ITLFormatReader::read(std::string const &path)
        {
            std::unique_ptr<hexed::Library> library(new hexed::ImmutableLibrary());
            detail::file f(path);

            detail::basic_segment<> segment(f.data());

            detail::basic_segment<blessed::endian::big> envelope(f.data());
            detail::LibraryAdapter l;

            auto p = envelope.data();

            std::size_t offset{};

            while(offset < p.size())
            {
                auto sub = p.subspan(offset);
                detail::basic_segment<> segment(sub);

                INFO("envelope : {0:x} header {1} data {2}", segment.mnemonic(), segment.header().size(), segment.data().size());

                static std::map<uint32_t, std::function<size_t(detail::LibraryAdapter &, blessed::span<blessed::byte const>)>> const functors =
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
