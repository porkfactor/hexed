#ifndef HEXED_ITUNES_ITL_HPP_
#define HEXED_ITUNES_ITL_HPP_

#include <codecvt>

#include <hexed/LibraryFormat.hpp>

void log_uint32(void const *data, size_t szData)
{
    uint32_t const *ptr = static_cast<decltype(ptr)>(data);
    size_t len = szData / sizeof(uint32_t);
    size_t offset = 0;

    for(size_t i = 0; i < len; i += 8)
    {
        for(size_t j = 0; j < 8; ++j)
        {
            if(j)
            {
                fprintf(stdout, " %08x", be32toh(ptr[offset + j]));
            }
            else
            {
                fprintf(stdout, "%08x", be32toh(ptr[offset + j]));
            }
        }

        printf("\n");

        offset += 8;
    }
}


void log_uint8(void const *data, size_t szData)
{
    size_t offset = 0;

    for(size_t i = 0; i < szData; i += 16)
    {
        for(size_t j = 0; j < 16; ++j)
        {
            uint8_t c = static_cast<uint8_t const *>(data)[offset + j];

            if(j)
            {
                printf("%02hhx", c);
            }
            else
            {
                printf(" %02hhx", c);

            }
        }

        printf("  ");

        for(size_t j = 0; j < 16; ++j)
        {
            char c = static_cast<char const *>(data)[offset + j];

            if(j)
            {
                printf(" ");
            }

            if(isprint(c) && !iscntrl(c))
            {
                printf("%c", c);
            }
            else
            {
                printf(".");
            }
        }

        printf("\n");
        offset += 16;
    }
}

static inline size_t do_decrypt(void *dest, size_t szDest, void const *source, size_t szSource)
{
    CCCryptorRef cryptorRef = nullptr;
    uint8_t const key[kCCKeySizeAES128] = { 0x42, 0x48, 0x55, 0x49, 0x4c, 0x75, 0x69, 0x6c, 0x66, 0x67, 0x68, 0x75, 0x69, 0x6c, 0x61, 0x33 };
    size_t szWritten = 0;
    size_t dSize{};

    szSource -= szSource % 16;

    if(CCCryptorCreateWithMode(
        kCCDecrypt,
        kCCModeECB,
        kCCAlgorithmAES128,
        ccNoPadding,
        nullptr,
        key,
        sizeof(key),
        nullptr,
        0,
        0,
        0,
        &cryptorRef) == kCCSuccess)
    {
        dSize = CCCryptorGetOutputLength(cryptorRef, szSource, true);

        CCCryptorUpdate(cryptorRef, source, szSource, dest, szDest, &szWritten);
        CCCryptorFinal(cryptorRef, static_cast<char *>(dest) + szWritten, szDest - szWritten, &szWritten);
        CCCryptorRelease(cryptorRef);
    }

    return dSize;
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

struct c_deleter
{
    void operator()(void *p) const { ::free(p); }
};

template<typename _Type = void>
using unique_c_ptr = std::unique_ptr<_Type, c_deleter>;

template<typename _Type = void>
unique_c_ptr<_Type> malloc_unique(std::size_t n)
{
    return unique_c_ptr<_Type>(static_cast<_Type *>(::malloc(n)));
}

#define CHAR2UINT(a,b,c,d)              ((static_cast<uint32_t>(d) << 24) | \
                                         (static_cast<uint32_t>(c) << 16) | \
                                         (static_cast<uint32_t>(b) << 8) | \
                                         (static_cast<uint32_t>(a)))

#define CHAR2UINT_BE(x)                 CHAR2UINT(x[0], x[1], x[2], x[3])
#define CHAR2UINT_LE(x)                 CHAR2UINT(x[3], x[2], x[1], x[0])

static char constexpr envelope_mnemonic[] = "hfma";
static char constexpr album_list_mnemonic[] = "Iama";
static char constexpr album_item_mnemonic[] = "iama";

static char constexpr playlist_list_mnemonic[] = "IPma";
static char constexpr playlist_item_mnemonic[] = "Ipma";

enum
{
    ITL_HEADER_ENVELOPE_LE              = CHAR2UINT_LE(envelope_mnemonic),
    ITL_HEADER_ENVELOPE_BE              = CHAR2UINT_BE(envelope_mnemonic),
    ITL_HEADER_SECTION_BOUNDARY_LE      = CHAR2UINT_LE("hsma"),
    ITL_HEADER_SECTION_BOUNDARY_BE      = CHAR2UINT_BE("hsma"),
    ITL_HEADER_HDFM_LE                  = CHAR2UINT_LE("hdfm"),
    ITL_HEADER_HDFM_BE                  = CHAR2UINT_BE("hdfm"),
    ITL_HEADER_HDSM_LE                  = CHAR2UINT_LE("hdsm"),
    ITL_HEADER_HDSM_BE                  = CHAR2UINT_BE("hdsm"),
    ITL_HEADER_HOHM_LE                  = CHAR2UINT_LE("hohm"),
    ITL_HEADER_HTIM_LE                  = CHAR2UINT_LE("htim"),
    ITL_HEADER_ALBUM_LIST_LE            = CHAR2UINT_LE(album_list_mnemonic),
    ITL_HEADER_ALBUM_LIST_BE            = CHAR2UINT_BE(album_list_mnemonic),
    ITL_HEADER_ALBUM_ITEM_LE            = CHAR2UINT_LE(album_item_mnemonic),
    ITL_HEADER_ALBUM_ITEM_BE            = CHAR2UINT_BE(album_item_mnemonic),
};

namespace hexed
{
    namespace itl
    {
        class AttributeAdapter
        {
        public:
            virtual ~AttributeAdapter() {}

            virtual void setTrackTitle(std::string &&) {}
            virtual void setAlbumTitle(std::string &&) {}
            virtual void setArtist(std::string &&) {}

        private:
        };

        class TrackAdapter : public AttributeAdapter
        {
        public:
            TrackAdapter(hexed::Track &track) :
                track_(track)
            {}

            void setTrackTitle(std::string &&s) override
            {
                track_.setTitle(std::move(s));
            } 

            void setAlbumTitle(std::string &&s) override
            {}

        private:
            hexed::Track &track_;
        };

        class AlbumAdapater : public AttributeAdapter
        {
        public:
            AlbumAdapater(hexed::Album &album) :
                album_(album)
            {}

            void setAlbumTitle(std::string &&s) override {}

        private:
            hexed::Album &album_;
        };

        class PlaylistAdapter : public AttributeAdapter
        {
        public:
            PlaylistAdapter(hexed::Playlist &playlist) :
                playlist_(playlist)
            {}

        private:
            hexed::Playlist &playlist_;
        };

        class thinger
        {
        public:
            thinger(std::string const &path) : 
                mapping_(path)
            {

            }

            void process(hexed::Library &library)
            {
                detail::basic_segment<blessed::endian::native> itl(mapping_.data(), mapping_.size());

                switch(itl.mnemonic())
                {
                case ITL_HEADER_HDFM_BE:
                    process_envelope<blessed::endian::big>(library);
                    break;
                case ITL_HEADER_HDFM_LE:
                    process_envelope<blessed::endian::little>(library);
                    break;
                }        
            }

            static void print_string(uint32_t flags, void const *data, size_t szData)
            {
                if(flags & 0x2)
                {
                    std::string_view s(static_cast<char const *>(data), szData);

                    spdlog::info("ascii: {0}", s);
                }
                else
                {
                    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cnv;
                    std::u16string_view s(static_cast<char16_t const *>(data), szData / sizeof(char16_t));

                    std::string s1 = cnv.to_bytes(std::u16string(s));

                    spdlog::info("utf-16: {0}", s1);
                }
            }

            template<blessed::endian _Order>
            uint32_t process_hohm(AttributeAdapter &handler, void const *data, size_t szData)
            {
                detail::hohm<_Order> hohm(data, szData);

                spdlog::info("hohm : mnemonic {0:08x} segment {1} data {2} type {3:04x}",
                    hohm.mnemonic(),
                    hohm.header_length(),
                    hohm.data_length(),
                    hohm.type());

                log_uint32(data, hohm.header_length() + 16);

                switch(hohm.type())
                {
                case detail::hohm<_Order>::track_title:
//                    handler.setTrackTitle(std::move(hohm.utf8()));
//                    break;

                case detail::hohm<_Order>::album_title:
//                    handler.setAlbumTitle(std::move(hohm.utf8()));
//                    break;

                case detail::hohm<_Order>::artist:
//                    handler.setArtist(std::move(hohm.utf8()));
//                    break;

                case detail::hohm<_Order>::genre:
                case detail::hohm<_Order>::file_type:
                case detail::hohm<_Order>::comments:
                case detail::hohm<_Order>::local_path:
                case detail::hohm<_Order>::composer:
                case detail::hohm<_Order>::playlist_name:
                case detail::hohm<_Order>::library_name:
                case detail::hohm<_Order>::sort_artist:
                case detail::hohm<_Order>::sort_title:
                case detail::hohm<_Order>::sort_album:
                case detail::hohm<_Order>::sort_composer:
                case detail::hohm<_Order>::purchaser_email:
                case detail::hohm<_Order>::purchaser_name:

                case 0x2b: /* <publisher>:isrc:<isrc> */
                case 0x2e:

                case 0x012c: /* album::title */
                case 0x012d: /* album::artist */
                case 0x012e: /* album::album artist */

                case 0x2d:
                case 0x1b: /* track::album artist */
                case 0x1d:
                case 0x0190: /* artist::artist name */
                    print_string(hohm.uint32(24), static_cast<char const *>(hohm.payload()) + 16, hohm.uint32(28));
                    break;
                    
                case 0x36: /* XML something or other */
                case 0x38:
                case 0x3f:
                case 0x6d:
                case 0x0192: /* artist::thumbnail ?? */
                    {
                        
                        std::string_view s(static_cast<char const *>(hohm.payload()), hohm.payload_length());
                        spdlog::info("string: {0}", s);
                    }
                    break;

                case detail::hohm<_Order>::smart_criteria:
                case 0x42: /* 'book' section */
                default:
                    log_uint32(data, hohm.data_length());
                    break;
                }


                return hohm.data_length();
            }

            /* dictionary of audio track metadata */
            template<blessed::endian _Order>
            uint32_t process_htim(hexed::Track &track, void const *data, size_t szData)
            {
                detail::dictionary_segment<_Order> segment(data, szData);

                spdlog::info("htim : mnemonic {0:08x} segment {1} data {2} count {3}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.payload_length(),
                    segment.count());

                log_uint32(data, segment.header_length());

                TrackAdapter adapter(track);

                segment.foreach([this, &adapter](void const *data, size_t szData) {
                    return process_hohm<_Order>(adapter, data, szData);
                });

                return segment.data_length();
            }

            /* list of audio tracks */
            template<blessed::endian _Order>
            uint32_t process_htlm(hexed::Library &library, void const *data, size_t szData)
            {
                detail::array_segment<_Order> segment(data, szData);
                /* what exactly is this section? */

                spdlog::info("htlm : mnemonic {0:08x} segment {1} count {2}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.count());

                segment.foreach([this, &library](void const *data, size_t szData) {
                    hexed::Track track;

                    return process_htim<_Order>(track, data, szData);
                });

                return segment.data_length();
            }

            /* dictionary of ??? metadata */
            template<blessed::endian _Order>
            uint32_t process_hpim(hexed::Track &track, void const *data, size_t szData)
            {
                detail::dictionary_segment<_Order> segment(data, szData);

                spdlog::info("hpim : mnemonic {0:08x} segment {1} data {2} count {3}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.data_length(),
                    segment.uint32(12));

                // log_uint32(data, segment.header_length());
                log_uint32(segment.payload(), segment.payload_length());

                AttributeAdapter adapter;

                segment.foreach([this, &adapter](void const *data, size_t szData) {
                    return process_hohm<_Order>(adapter, data, szData);
                });

                return segment.data_length();
            }

            /* list of ???? */
            template<blessed::endian _Order>
            uint32_t process_hplm(hexed::Library &library, void const *data, size_t szData)
            {
                detail::array_segment<_Order> segment(data, szData);
                /* what exactly is this section? */

                spdlog::info("hplm : mnemonic {0:08x} segment {1} count {2}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.count());

                segment.foreach([this, &library](void const *data, size_t szData) {
                    hexed::Track track;

                    return process_hpim<_Order>(track, data, szData);
                });

                return segment.data_length();
            }

            /* thing dictionary */
            template<blessed::endian _Order>
            uint32_t process_haim(void const *data, size_t szData)
            {
                detail::dictionary_segment<_Order> segment(data, szData);

                spdlog::info("haim : mnemonic {0:08x} segment {1} data {2} count {3}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.payload_length(),
                    segment.count());

                log_uint32(data, segment.header_length());

                AttributeAdapter adapter;

                segment.foreach([this, &adapter](void const *data, size_t szData) {
                    return process_hohm<_Order>(adapter, data, szData);
                });

                return segment.data_length();
            }

            /* list of things */
            template<blessed::endian _Order>
            uint32_t process_halm(hexed::Library &library, void const *data, size_t szData)
            {
                detail::array_segment<_Order> segment(data, szData);

                spdlog::info("halm : mnemonic {0:08x} segment {1} count {2}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.count());

                segment.foreach([this, &library](void const *data, size_t szData) {
                    return process_haim<_Order>(data, szData);
                });

                return segment.data_length();
            }

            /* artist dictionary */
            template<blessed::endian _Order>
            uint32_t process_hiim(void const *data, size_t szData)
            {
                detail::dictionary_segment<_Order> segment(data, szData);

                spdlog::info("hiim : mnemonic {0:08x} segment {1} data {2} count {3}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.payload_length(),
                    segment.count());

                log_uint32(data, segment.header_length());

                AttributeAdapter adapter;

                segment.foreach([this, &adapter](void const *data, size_t szData) {
                    return process_hohm<_Order>(adapter, data, szData);
                });

                return segment.data_length();
            }

            /* list of artists */
            template<blessed::endian _Order>
            uint32_t process_hilm(hexed::Library &library, void const *data, size_t szData)
            {
                detail::array_segment<_Order> segment(data, szData);

                spdlog::info("hilm : mnemonic {0:08x} segment {1} count {2}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.count());

                segment.foreach([this, &library](void const *data, size_t szData) {
                    return process_hiim<_Order>(data, szData);
                });

                return segment.data_length();
            }

            /* list of ???? */
            template<blessed::endian _Order>
            uint32_t process_hghm(hexed::Library &library, void const *data, size_t szData)
            {
                detail::array_segment<_Order> segment(data, szData);

                spdlog::info("hghm : mnemonic {0:08x} segment {1} count {2}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.count());

                AttributeAdapter adapter;

                segment.foreach([this, &adapter](void const *data, size_t szData) {
                    return process_hohm<_Order>(adapter, data, szData);
                });

                return segment.data_length();
            }

            /* dictionary of audio track metadata */
            template<blessed::endian _Order>
            uint32_t process_htim_alt(hexed::Track &track, void const *data, size_t szData)
            {
                detail::dictionary_segment<_Order> segment(data, szData);

                spdlog::info("htim_alt : mnemonic {0:08x} segment {1} data {2} count {3}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.payload_length(),
                    segment.count());

                log_uint32(segment.header(), segment.header_length());

                TrackAdapter adapter(track);

                segment.foreach([this, &adapter](void const *data, size_t szData) {
                    return process_hohm<_Order>(adapter, data, szData);
                });

                return segment.data_length();
            }

            /* list of audio tracks */
            template<blessed::endian _Order>
            uint32_t process_htlm_alt(hexed::Library &library, void const *data, size_t szData)
            {
                detail::array_segment<_Order> segment(data, szData);
                /* what exactly is this section? */

                spdlog::info("htlm_alt : mnemonic {0:08x} segment {1} count {2}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.count());

                log_uint32(segment.header(), segment.header_length());

                segment.foreach([this, &library](void const *data, size_t szData) {
                    hexed::Track track;

                    return process_htim_alt<_Order>(track, data, szData);
                });

                return segment.data_length();
            }

            template<blessed::endian _Order>
            uint32_t process_hdfm(void const *data, size_t szData)
            {
                detail::data_segment<_Order> segment(data, szData);

                spdlog::info("hdfm : mnemonic {0:08x} header {1}",
                    segment.mnemonic(),
                    segment.header_length());

                log_uint32(segment.header(), segment.header_length());

                return segment.data_length();
            }

            template<blessed::endian _Order>
            uint32_t process_section(hexed::Library &library, void const *data, size_t szData)
            {
                detail::section<_Order> segment(data, szData);

                spdlog::info("hdsm : mnemonic {0:08x} segment {1} data {2} type {3}",
                    segment.mnemonic(),
                    segment.header_length(),
                    segment.data_length(),
                    segment.type());

                log_uint32(segment.header(), segment.header_length());

                switch(segment.type())
                {
                /* track list (1) */
                case detail::section<_Order>::subtype_htlm:
                    process_htlm<_Order>(library, segment.payload(), segment.payload_length());
                    break;
                /* unknown (2) */
                case detail::section<_Order>::subtype_hplm:
                    process_hplm<_Order>(library, segment.payload(), segment.payload_length());
                    break;
                /* album list (9) */
                case detail::section<_Order>::subtype_halm:
                    process_halm<_Order>(library, segment.payload(), segment.payload_length());
                    break;
                /* artist list (11) */
                case detail::section<_Order>::subtype_hilm:
                    process_hilm<_Order>(library, segment.payload(), segment.payload_length());
                    break;
                /* unknown (13) */
                case detail::section<_Order>::subtype_hghm:
                    process_hghm<_Order>(library, segment.payload(), segment.payload_length());
                    break;
                /* track data (14) */
                case detail::section<_Order>::subtype_htlm_alt:
                    process_htlm_alt<_Order>(library, segment.payload(), segment.payload_length());
                    break;
                /* unknown (16) */
                case 16:
                    process_hdfm<_Order>(segment.payload(), segment.payload_length());
                    break;
                
                default:
                    break;
                }

                return segment.data_length();
            }

            void process_data(hexed::Library &library, void const *data, size_t szData)
            {
                size_t offset = 0;

                while(offset < szData)
                {
                    detail::basic_segment<blessed::endian::native> segment(static_cast<char const *>(data) + offset, szData);
                    uint32_t data_length{};

                    spdlog::info("mnemonic {:08x}", segment.mnemonic());

                    switch(segment.mnemonic())
                    {
                    case ITL_HEADER_HDSM_LE: /* hdsm */
                        data_length = process_section<blessed::endian::little>(library, static_cast<char const *>(data) + offset, szData - offset);
                        break;
                    case ITL_HEADER_HDSM_BE:
                        data_length = process_section<blessed::endian::big>(library, static_cast<char const *>(data) + offset, szData - offset);
                        break;
                    default:
                        spdlog::info("unknown segment {:08x}", segment.mnemonic());
                        break;
                    }

                    offset += data_length;
                }
            }

            template<blessed::endian _Order>
            void process_envelope(hexed::Library &library)
            {
                detail::envelope<_Order> envelope(mapping_.data(), mapping_.size());

                uint32_t thing_length = 16777216 * 4;

                spdlog::info("segment size {}", envelope.header_length());
                spdlog::info("data size {}", envelope.data_length());

                unique_c_ptr<void> raw(malloc_unique(envelope.crypt_length()));
                unique_c_ptr<void> uncompressed(malloc_unique(thing_length));

                do_decrypt(raw.get(), envelope.crypt_length(), envelope.payload(), envelope.crypt_length());

                ::memcpy(const_cast<void *>(envelope.payload()), raw.get(), envelope.crypt_length());
                
                size_t szData = inflate(uncompressed.get(), thing_length, envelope.payload(), envelope.payload_length());

                spdlog::info("inflated size {0}", szData);

                process_data(library, uncompressed.get(), szData);
            }
        private:
            mapping mapping_;
        };
    }

    class ITLFormat : public LibraryFormat
    {

    };
}

#endif
