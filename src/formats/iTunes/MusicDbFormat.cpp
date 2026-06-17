#include <memory>
#include <pwd.h>

#include <formats/iTunes/MusicDbFormat.hpp>
#include <formats/iTunes/MusicDbFormatReader.hpp>
#include <hexed/LibraryFormatManager.hpp>
#include <CoreFoundation/CFPropertyList.h>

namespace hexed
{
    namespace itunes
    {
        template<typename _Type>
        struct cf_deleter
        {
            void operator()(_Type *p) const
            {
                ::CFRelease(p);
            }
        };

        std::string MusicDbLibraryFormat::getUserHomeFolder()
        {
            char const *homedir{};
            passwd const *pwuid{};

            if ((homedir = getenv("HOME")) != nullptr)
            {
                return std::string{homedir};
            }

            if ((pwuid = getpwuid(geteuid())) != nullptr)
            {
                return std::string{pwuid->pw_dir};
            }

            return "";
        };

        std::string MusicDbLibraryFormat::getDefaultPreferencesPath()
        {
            return getUserHomeFolder() + "/Library/Preferences/com.apple.Music.plist";
        }

        template<typename _Type>
        using unique_cf_ref = std::unique_ptr<_Type, cf_deleter<_Type>>;

        using unique_cf_string = unique_cf_ref<struct __CFString const>;
        using unique_cf_url = unique_cf_ref<struct __CFURL const>;
        using unique_cf_read_stream = unique_cf_ref<struct __CFReadStream>;
        using unique_cf_property_list = unique_cf_ref<void const>;

        std::string butts(CFStringRef str, CFStringEncoding encoding = kCFStringEncodingUTF8)
        {
            CFIndex length = CFStringGetLength(str);

            if (!length)
                return std::string{};

            auto ptr = CFStringGetCStringPtr(str, encoding);

            if (ptr)
                return std::string(ptr, length);

            std::string res;
            CFIndex offset{};

            res.reserve(CFStringGetMaximumSizeForEncoding(length, encoding));

            while (offset < length)
            {
                UInt8 buffer[1024];
                CFIndex bytes{};
                CFIndex consumed;

                consumed = CFStringGetBytes(str, CFRangeMake(offset, length - offset), encoding, '?', FALSE, buffer, sizeof(buffer), &bytes);

                if ((consumed == 0) || (bytes <= 0))
                    break;

                res.append(reinterpret_cast<char *>(buffer), bytes);

                offset += consumed;
            }

            return res;
        }

        std::shared_ptr<LibraryFormatReader> MusicDbLibraryFormat::createReader()
        {
            return std::make_shared<MusicDbFormatReader>();
        }

        static void dump_value(CFTypeRef v)
        {
            fprintf(stdout, "type: %lu\n", CFGetTypeID(v));

            fprintf(stdout, "%s\n", butts(CFCopyTypeIDDescription(CFGetTypeID(v))).c_str());

            if (CFGetTypeID(v) == CFStringGetTypeID())
            {
                fprintf(stdout, "%s\n", butts(static_cast<CFStringRef>(v)).c_str());
            }
        }

        static void dump_dict(CFDictionaryRef d)
        {
            auto count = CFDictionaryGetCount(d);

            CFTypeRef keys[count];
            CFTypeRef values[count];

            CFDictionaryGetKeysAndValues(d, keys, values);

            for (decltype(count) i = 0; i < count; ++i)
            {
                // dump_value(keys[i]);
                dump_value(values[i]);
            }
        }

        static void show_error(CFErrorRef e)
        {
            fprintf(stdout, "description: %s\n", butts(CFErrorCopyDescription(e)).c_str());
            fprintf(stdout, "reason: %s\n", butts(CFErrorCopyFailureReason(e)).c_str());
            dump_dict(CFErrorCopyUserInfo(e));
        }

        std::vector<std::string> MusicDbLibraryFormat::scan(void) const
        {
            ::CFAllocatorRef allocator = CFAllocatorGetDefault();
            ::CFPropertyListFormat fmt = kCFPropertyListOpenStepFormat;
            ::CFErrorRef error{};

            unique_cf_string path(CFStringCreateWithCString(allocator, getDefaultPreferencesPath().c_str(), kCFStringEncodingUTF8));
            unique_cf_url url(CFURLCreateWithFileSystemPath(allocator, path.get(), CFURLPathStyle::kCFURLPOSIXPathStyle, FALSE));
            unique_cf_read_stream stream(CFReadStreamCreateWithFile(allocator, url.get()));

            CFReadStreamOpen(stream.get());

            unique_cf_property_list propertyList(CFPropertyListCreateWithStream(allocator, stream.get(), 0, kCFPropertyListImmutable, &fmt, &error));

            unique_cf_string key(CFStringCreateWithCString(allocator, "library-url", kCFStringEncodingASCII));

            std::vector<std::string> result{};

            if (error)
                show_error(error);

            if (CFGetTypeID(propertyList.get()) == CFDictionaryGetTypeID())
            {
                CFDictionaryRef dict(static_cast<CFDictionaryRef>(propertyList.get()));
                CFTypeRef value{};
    
                if (CFDictionaryGetValueIfPresent(dict, key.get(), &value) == TRUE)
                {
                    if (CFGetTypeID(value) == CFStringGetTypeID())
                    {
                        
                        unique_cf_url uri(CFURLCreateWithString(allocator, static_cast<CFStringRef>(value), nullptr));
                        unique_cf_string path(CFURLCopyFileSystemPath(uri.get(), kCFURLPOSIXPathStyle));

                        result.emplace_back(butts(path.get()) + "/Library.musicdb");
                    }
                }
            }

            return result;
        }
    }
}


// $HOME/Music/Music/Music Library.musiclibrary/Library.musicdb

