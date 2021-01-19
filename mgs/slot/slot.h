#pragma once
#include <time.h>
#include <fstream>

#include "../cnf/cnf.h"
#include "../common/util.h"
#include "../common/strcode.h"
#include "../common/fileutil.h"
#include "../common/ext_table.h"
#include "../common/game/game.h"
#include "../common/decryptor/decryptor.h"
#include "../3rdparty/zlib/wrapper/ZlibWrapper.h"

struct SlotHeader {
	uint32_t timestamp; //interchangable with strcode
	uint16_t version;
	uint16_t pageSize;  //multiplied by sector size
	uint16_t numPages;
	uint16_t unknownA;
	uint32_t unknownB;
};

struct SlotKeyHeader {
	uint32_t saltA;
	uint32_t saltB;
	uint32_t saltC;
};

struct SlotKeyEntry {
	uint32_t firstPage; //5 page + 3 unknown
	uint32_t lastPage;  //5 page + 3 unknown
	uint32_t hash;
};

struct SlotKeyEntryHD {
	uint32_t firstPage; //5 page + 3 unknownA
	uint32_t lastPage;  //5 page + 3 unknownB
	int32_t hash;
	uint32_t unknownA;
	uint32_t unknownB;
};

struct SlotCompressedHeader {
	uint16_t unknownA;
	uint16_t unknownB;
	uint32_t padding;
	uint32_t compressedSize;
	uint32_t decompressedSize;
	uint8_t compressedData[];
};

struct PageInfo {
	uint16_t pageID;
	unsigned int offset;
	unsigned int size;
};

class Slot : public CNFHandler {
protected:
	Decryptor decryptor;
	std::string filename;
	uint32_t sector = 0x800;

	void writeDataToFile(uint8_t* data, int size, const std::string& filename, std::string& output);
	uint8_t* _decryptPage(uint32_t key, uint32_t saltA, uint32_t saltB, uint32_t saltC, int offset, int size, bool makeKey = 0);
private:
	SlotHeader header;
	uint8_t* decompressPage(uint8_t* compressedPage, int& size);
	uint8_t* decryptPage(std::string keyFile, uint16_t numPage);
	void extractFiles(uint8_t* pageData, int pageSize, std::string& output);
	
	uint32_t getSectorSize();
	uint64_t getNextSectorOffset(uint64_t currentOffset);
	void processQar(std::string filename, std::string region, std::string* workDir);
	void processAfp(std::string filename, std::string region, std::string* workDir);
	void processVram(std::string filename, std::string region, std::string* workDir);
	void processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size);
	void processSlot(std::string filename, int16_t page, int64_t offset, std::string region, std::string* workDir);
public:
	~Slot();
	Slot(std::string filename);
	Slot(std::string filename, uint32_t sector);

	std::string getFilename();
	std::string getCreatedDate();

	void open();
	void extractAll(std::string keyFile, const std::string& output = "");
	void extract(std::string keyFile, uint16_t pageID, std::string output = "");
	
};