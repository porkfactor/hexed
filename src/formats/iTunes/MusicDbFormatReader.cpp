#include <hexed/Log.hpp>
#include <hexed/Library.hpp>
#include <hexed/detail/Compression.hpp>
#include <hexed/detail/Encryption.hpp>

#include <formats/iTunes/MusicDbFormatReader.hpp>
#include <formats/iTunes/musicdb.hpp>
#include <formats/iTunes/file.hpp>

#include <CoreFoundation/CFPropertyList.h>

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
                void process(detail::hsma<_Order> const &attribute)
                {

                }
            };

            class AlbumAdapter
            {
            public:
                template<blessed::endian _Order>
                void process(detail::iama<_Order> const &attribute)
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
                void process(detail::boma<_Order> const &attribute)
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

        template<blessed::endian _Order>
        static std::size_t process_track_attribute(detail::TrackAdapter &track, blessed::span<blessed::byte const> data)
        {
            using boma = detail::boma<_Order>;

            boma header(data);
            INFO("boma : segment {0:x} header {1} payload {2} subutype {3:08x}", header.mnemonic(), header.header().size(), header.payload().size(), header.subtype());

            track.process(header);

            return header.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_track_metadata(detail::TrackAdapter &track, blessed::span<blessed::byte const> data)
        {
            using itma = detail::itma<_Order>;
            itma header(data);

            INFO("itma : segment {0:x} header {1} payload {2} count {3}", header.mnemonic(), header.header().size(), header.payload().size(), static_cast<int>(header.count()));

            log_data(header.header());

            header.foreach([&track](blessed::span<blessed::byte const> s)
            {
                detail::basic_segment<> segment(s);
                std::size_t offset{};

                switch(segment.mnemonic())
                {
                case detail::boma<blessed::endian::big>::identifier():
                    offset = process_track_attribute<blessed::endian::big>(track, s);
                    break;
                case detail::boma<blessed::endian::little>::identifier():
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
        static std::size_t process_track_metadata_list(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using ltma = detail::ltma<_Order>;

            ltma segment(data);

            segment.foreach([&library](blessed::span<blessed::byte const> s)
            {
                detail::TrackAdapter track;
                detail::basic_segment<> segment(s);
                size_t offset{};

                switch (segment.mnemonic())
                {
                case detail::itma<blessed::endian::big>::identifier():
                    offset = process_track_metadata<blessed::endian::little>(track, s);
                    break;
                case detail::itma<blessed::endian::little>::identifier():
                    offset = process_track_metadata<blessed::endian::little>(track, s);
                    break;
                default:
                    break;
                }

                return offset;
            });

            return data.size_bytes();
        }

        template<blessed::endian _Order>
        static std::size_t process_playlist_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            // log_uint32(data);

            return data.size_bytes();
        }

        template<blessed::endian _Order>
        static std::size_t process_file_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            // log_uint32(data);

            return data.size_bytes();
        }

        template<blessed::endian _Order>
        static std::size_t process_album_metadata(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            // log_uint32(data);

            return data.size_bytes();
        }

        template<blessed::endian _Order>
        static std::size_t process_hsma(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using hsma = detail::hsma<_Order>;

            hsma segment(data);

            INFO("hsma : segment {0:x} header {1} payload {2} size {3} type {4}", segment.mnemonic(), segment.header().size(), segment.payload().size(), segment.size(), static_cast<int>(segment.type()));

            switch (segment.type())
            {
            case hsma::track_metadata:
                process_track_metadata_list<_Order>(library, segment.payload());
                break;
            case hsma::playlist_metadata:
                process_playlist_metadata<_Order>(library, segment.payload());
                break;
            case hsma::file_metadata:
                process_file_metadata<_Order>(library, segment.payload());
                break;
            case hsma::subtype_unknown_5:
            case hsma::subtype_unknown_6:
            case hsma::subtype_unknown_7:
            case hsma::subtype_unknown_8:
                break;
            case hsma::album_metadata:
                process_album_metadata<_Order>(library, segment.payload());
                break;
            case hsma::subtype_unknown_10:
                
            case hsma::subtype_hilm:
            case hsma::subtype_hghm:
            case hsma::purchased_track_metadata:
            case hsma::subtype_hplm:
            case hsma::subtype_unknown_15:
            case hsma::subtype_unknown_16:
            case hsma::subtype_unknown_17:
            case hsma::subtype_unknown_18:
            case hsma::subtype_unknown_19:
            case hsma::subtype_unknown_20:
            case hsma::subtype_unknown_21:
                break;
            }

            return segment.size();
        }

        template<blessed::endian _Order>
        static std::size_t process_hfma(detail::LibraryAdapter &library, blessed::span<blessed::byte const> data)
        {
            using hfma = detail::hfma<_Order>;

            hfma envelope(data);

            auto p = envelope.data();

            std::size_t offset{};

            while(offset < p.size())
            {
                auto sub = p.subspan(offset);
                detail::basic_segment<> segment(sub);

                INFO("hfma : {0:x} header {1} data {2}", segment.mnemonic(), segment.header().size(), segment.data().size());

                switch (segment.mnemonic())
                {
                case detail::hsma<blessed::endian::big>::identifier():
                    offset += process_hsma<blessed::endian::little>(library, sub);
                    break;
                case detail::hsma<blessed::endian::little>::identifier():
                    offset += process_hsma<blessed::endian::little>(library, sub);
                    break;
                default:
                    ERROR("unexpected child of hdfm: {0:08x}", segment.mnemonic());
                    throw std::runtime_error("unexpected child of hdfm");
                }
            }

            return data.size_bytes();
        }

        class MusicDbFile : public detail::file
        {
        public:
            MusicDbFile(std::string const &path) :
                file(path)
            {
                detail::basic_segment<blessed::endian::native> seg(mapping_.data());

                switch (seg.mnemonic())
                {
                case detail::hfma<blessed::endian::big>::identifier():
                    inflate<blessed::endian::big>();
                    break;
                case detail::hfma<blessed::endian::little>::identifier():
                    inflate<blessed::endian::little>();
                    break;
                }
            }

            template<blessed::endian _Order>
            void inflate()
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
            }
        };

        std::unique_ptr<hexed::Library> MusicDbFormatReader::read(std::string const &path)
        {
            fprintf(stderr, "reading file %s\n", path.c_str());

            std::unique_ptr<hexed::Library> library(new hexed::ImmutableLibrary());
            MusicDbFile f(path);

            detail::basic_segment<> segment(f.data());

            detail::LibraryAdapter l(library.get());

            INFO("mnemonic: {0:08x}", segment.mnemonic());

            switch (segment.mnemonic())
            {
            case detail::hfma<blessed::endian::little>::identifier():
                process_hfma<blessed::endian::little>(l, f.data());
                break;
            case detail::hfma<blessed::endian::big>::identifier():
                process_hfma<blessed::endian::big>(l, f.data());
                break;
            default:
                library.reset();
                break;
            }

            return library;
        }

        MusicDbFormatReader::MusicDbFormatReader() :
            LibraryFormatReader()
        {}

        MusicDbFormatReader::~MusicDbFormatReader()
        {}
    }
}