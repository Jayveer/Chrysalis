#include "decryptor.h"

uint32_t Decryptor::makePageKeyA(uint32_t salt) {
   uint32_t fileKeyX = salt ^ 0x00006576;
   fileKeyX <<= 0x10;
   return fileKeyX |= salt;
}

uint32_t Decryptor::makePageKeyB(uint32_t salt, uint32_t salt2) {
    return salt * salt2;
}

uint32_t Decryptor::genericDecode(uint32_t key, uint32_t saltA, uint32_t saltB, uint32_t saltC, int offset, int size, uint8_t* src, bool makeKey) {
    uint32_t pageKey = saltA ^ saltB;
    if (makeKey) key = makePageKeyA(pageKey);
    uint32_t pageKeyB = makePageKeyB(pageKey, saltC);
    return decodeBuffer(key, pageKeyB, offset, size, src);
}

uint32_t Decryptor::decodeBuffer(uint32_t keyA, uint32_t keyB, unsigned int offset, unsigned int size, uint8_t* src) {
    uint32_t* srcP = (uint32_t*)src;
    size /= 4;

    for (int i = offset; i < size; i++) {
        uint32_t interval = keyA * KEY;
        srcP[i] ^= keyA;
        keyA = interval + keyB;
    }
    return keyA;
}