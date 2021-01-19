#pragma once
#include "../slot.h"

extern int numStages;
extern int numStagesHD;
extern const char* stages[93];

struct StageHeader {
	uint32_t saltA;
	uint32_t saltB;
	uint32_t saltC;
	uint32_t unknownA;
	uint32_t unknownB;
	uint32_t unknownC;
	uint16_t numPages;
	uint16_t unknownD;
	uint32_t lookupOffset;
};

struct StageTable {
	uint32_t size;
	uint32_t key;
	uint32_t offset;
};

struct StageLookup {
	int32_t key;
	uint32_t index;
	uint32_t nextOffsetLT;
	uint32_t nextOffsetGT;
};

struct StageCompressedHeader {
	uint32_t decompressedSize;
	uint8_t compressedData[];
};

class Stage: public Slot {
public:

	Stage(std::string filename);
	Stage(std::string filename, uint32_t sector);
	~Stage();

	void open(); 
	int getNumStages();
	void extractAll(std::string output = "");
	void extractAllIdx(std::string output = "");
	void extract(uint16_t fileIdx, std::string output = "");
	void extract(const std::string& stageName, std::string output = "");
private:
	StageHeader header = {};
	std::vector<StageTable> table;
	std::vector<StageLookup> lookup;

	uint8_t* decryptFile(uint16_t pageID);
	uint32_t findIndexForHash(int32_t hash);
	uint8_t* decompressFile(uint8_t* compressedPage, uint16_t pageID, int& size);
	uint8_t* extractFile(const std::string& fileName, std::string stageName, int& size);
	void extractToDir(const std::string& filename, const std::string& stageName, std::string& workDir);

	void processQar(std::string filename, std::string region, std::string* workDir);
	void processAfp(std::string filename, std::string region, std::string* workDir);
	void processVram(std::string filename, std::string region, std::string* workDir);
	void processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size);
	void processSlot(std::string filename, int16_t page, int64_t offset, std::string region, std::string* workDir);
};