#ifndef HEXED_DETAIL_ENCRYPTION_HPP_
#define HEXED_DETAIL_ENCRYPTION_HPP_

#include <CommonCrypto/CommonCrypto.h>

namespace hexed
{
    namespace detail
    {
        static inline size_t aes128decrypt(void *dest, size_t szDest, void const *source, size_t szSource)
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

    }
}

#endif
