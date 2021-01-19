#pragma once
#include <inttypes.h>

class Decryptor {
private:
	const uint32_t KEY = 0x02E90EDD;
public:
	uint32_t makePageKeyA(uint32_t salt);
	uint32_t makeFolderKey(uint32_t folderHash);
	uint32_t makePageKeyB(uint32_t salt, uint32_t salt2);
	uint32_t decodeBuffer(uint32_t keyA, uint32_t keyB, unsigned int offset, unsigned int size, uint8_t* src);
	uint32_t genericDecode(uint32_t key, uint32_t saltA, uint32_t saltB, uint32_t saltC, int offset, int size, uint8_t* src, bool makeKey = false);
};