#include <hexed/Log.hpp>
#include <hexed/Library.hpp>
#include <hexed/detail/Compression.hpp>
#include <hexed/detail/Encryption.hpp>

#include <formats/iTunes/file.hpp>
#include <formats/iTunes/itl.hpp>
#include <formats/iTunes/ITLFormatReader.hpp>

namespace hexed
{
    namespace itunes
    {
        namespace detail
        {
            class LibraryAdapter
            {
            public:
                LibraryAdapter(Library *)
                {}

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
                detail::basic_segment<> segment(s);
                std::size_t offset{};

                switch(segment.mnemonic())
                {
                case detail::hohm<blessed::endian::big>::identifier():
                    offset = process_track_attribute<blessed::endian::big>(track, s);
                    break;
                case detail::hohm<blessed::endian::little>::identifier():
                    offset = process_track_attribute<blessed::endian::little>(track, s);
                    break;
                default:
                    INFO("unexpected child {}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htlm record");
                }

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
                detail::basic_segment<> segment(s);
                std::size_t offset{};

                switch(segment.mnemonic())
                {
                case detail::hohm<blessed::endian::big>::identifier():
                    offset = process_track_attribute<blessed::endian::big>(track, s);
                    break;
                case detail::hohm<blessed::endian::little>::identifier():
                    offset = process_track_attribute<blessed::endian::little>(track, s);
                    break;
                default:
                    INFO("unexpected child {:08x}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htim record");
                }

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
                detail::TrackAdapter track;
                detail::basic_segment<> segment(s);
                size_t offset{};

                switch(segment.mnemonic())
                {
                case detail::htim<blessed::endian::big>::identifier():
                    offset = process_htim<blessed::endian::big>(track, s);
                    break;
                case detail::htim<blessed::endian::little>::identifier():
                    offset = process_htim<blessed::endian::little>(track, s);
                    break;
                default:
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
                detail::TrackAdapter track;
                detail::basic_segment<> segment(s);
                size_t offset{};

                switch (segment.mnemonic())
                {
                case detail::htim<blessed::endian::big>::identifier():
                    offset = process_htim<blessed::endian::big>(track, s);
                    break;
                case detail::htim<blessed::endian::little>::identifier():
                    offset = process_htim<blessed::endian::little>(track, s);
                    break;
                default:
                    INFO("unexpected child {}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of htlm record");
                }
            
                return offset;
            });

            return data.size();
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
        static std::size_t process_haim(detail::AlbumAdapter &library, blessed::span<blessed::byte const> data)
        {
            using haim = detail::haim<_Order>;

            haim header(data);
            // INFO("haim : segment {0:x} header {1} payload {2} subutype {3:08x}", header.mnemonic(), header.header().size(), header.payload().size(), header.subtype());

            // album.process(header);

            return header.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_album_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using halm = detail::halm<_Order>;
            
            halm header(data);

            INFO("halm : segment {0:x} header {1} payload {2} count {3}", header.mnemonic(), header.size(), header.payload().size(), static_cast<int>(header.count()));

            header.foreach([&library](blessed::span<blessed::byte const> s)
            {
                detail::AlbumAdapter album;
                detail::basic_segment<> segment(s);
                size_t offset{};

                switch (segment.mnemonic())
                {
                case detail::haim<blessed::endian::big>::identifier():
                    offset = process_haim<blessed::endian::big>(album, s);
                    break;
                case detail::haim<blessed::endian::little>::identifier():
                    offset = process_haim<blessed::endian::little>(album, s);
                    break;
                default:
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

            hdsm header(data);

            INFO("hdsm : segment {0:x} header {1} payload {2} size {3} type {4}", header.mnemonic(), header.header().size(), header.payload().size(), header.size(), static_cast<int>(header.type()));

            switch (header.type())
            {
            case hdsm::track_metadata:
                process_track_metadata<_Order>(library, header.payload());
                break;
            case hdsm::playlist_metadata:
                process_playlist_metadata<_Order>(library, header.payload());
                break;
            case hdsm::file_metadata:
                process_file_metadata<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_5:
                process_subtype_5<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_6:
                process_subtype_6<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_7:
                process_subtype_7<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_8:
                process_subtype_8<_Order>(library, header.payload());
                break;
            case hdsm::album_metadata:
                process_album_metadata<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_10:
                process_subtype_10<_Order>(library, header.payload());
                break;
            case hdsm::subtype_hilm:
                process_hilm<_Order>(library, header.payload());
                break;
            case hdsm::subtype_hghm:
                process_hghm<_Order>(library, header.payload());
                break;
            case hdsm::purchased_track_metadata:
                process_purchased_track_metadata<_Order>(library, header.payload());
                break;
            case hdsm::subtype_hplm:
                process_hplm<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_15:
                process_subtype_15<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_16:
                process_subtype_16<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_17:
                process_subtype_17<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_18:
                process_subtype_18<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_19:
                process_subtype_19<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_20:
                process_subtype_20<_Order>(library, header.payload());
                break;
            case hdsm::subtype_unknown_21:
                process_subtype_21<_Order>(library, header.payload());
                break;
            default:
                break;
            }

            return header.size();
        }

        template<blessed::endian _Order>
        static void process_hdfm(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using hdfm = detail::hdfm<_Order>;

            hdfm envelope(data);

            auto p = envelope.data();

            std::size_t offset{};

            while(offset < p.size())
            {
                auto sub = p.subspan(offset);
                detail::basic_segment<> segment(sub);

                INFO("envelope : {0:x} header {1} data {2}", segment.mnemonic(), segment.header().size(), segment.data().size());

                switch (segment.mnemonic())
                {
                case detail::hdsm<blessed::endian::big>::identifier():
                    offset += process_hdsm<blessed::endian::little>(library, sub);
                    break;
                case detail::hdsm<blessed::endian::little>::identifier():
                    offset += process_hdsm<blessed::endian::little>(library, sub);
                    break;
                default:
                    ERROR("unexpected child of hdfm: {0:08x}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of hdfm");
                }
            }
        }

        class ITLFile : public detail::file
        {
        public:
            ITLFile(std::string const &path) :
                file(path)
            {
                detail::basic_segment<blessed::endian::native> seg(mapping_.data());

                switch (seg.mnemonic())
                {
                case detail::hdfm<blessed::endian::big>::identifier():
                    inflate<blessed::endian::big>();
                    break;
                case detail::hdfm<blessed::endian::little>::identifier():
                    inflate<blessed::endian::little>();
                    break;
                }
            }

            template<blessed::endian _Order>
            void inflate()
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
            }
        };

        std::unique_ptr<hexed::Library> ITLFormatReader::read(std::string const &path)
        {
            std::unique_ptr<hexed::Library> library(new hexed::ImmutableLibrary());
            ITLFile f(path);

            detail::basic_segment<> segment(f.data());

            detail::LibraryAdapter l(library.get());

            INFO("mnemonic: %08x", segment.mnemonic());

            switch (segment.mnemonic())
            {
            case detail::hdfm<blessed::endian::big>::identifier():
                process_hdfm<blessed::endian::big>(l, f.data());
                break;
            case detail::hdfm<blessed::endian::little>::identifier():
                process_hdfm<blessed::endian::little>(l, f.data());
                break;
            default:
                library.reset();
                break;
            }

            return library;
        }
    }
}
