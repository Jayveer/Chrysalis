#pragma once
#include <vector>
#include <string>
#include <sstream>

#include "../common/util.h"
#include "../common/ext_table.h"
#include "../common/game/game.h"

struct DataCNFTag {
	uint32_t id;
	uint32_t offset;
};

struct DataCNFTagHD {
	uint32_t id;
	uint32_t pad;
	uint32_t offset;
	uint32_t pad2;
};

struct DataCNF {
	uint32_t  numTags;
	DataCNFTag tags[];
};

struct DataCNFHD {
	uint32_t  numTags;
	uint32_t  pad;
	DataCNFTagHD tags[];
};


enum CNFTYPE {
	ASCII,
	BINARY
};

struct CNFREGION {
	const char* name;
	int id;
};

class CNFHandler {
public:
	virtual uint32_t getSectorSize() = 0;
	virtual uint64_t getNextSectorOffset(uint64_t currentOffset) = 0;
	virtual void processQar(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processAfp(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processVram(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size) = 0;
	virtual void processSlot(std::string filename, int16_t page, int64_t offset, std::string region, std::string* workDir) = 0;
};

class DataConfig {
public:
	DataConfig(std::string filename);
	DataConfig(uint8_t* data, int size);

	~DataConfig();
	int64_t getMaxPageSize();
	void read(CNFTYPE type = ASCII);
	void setWorkDir(std::string& workdir);
	void setHandler(CNFHandler* cnfHandler);
	const char* getRegionForID(uint32_t id);
private:
	int dataSize = 0;
	DataCNF* cnf = {};
	uint8_t* data = {};
	std::string filename;
	uint64_t dataPtr = 0;
	DataCNFHD* cnfHD = {};
	uint8_t* pageData = {};
	bool madeFromHD = false;
	std::string* workdir = {};
	CNFHandler* cnfHandler = {};
	std::string currentRegion;
	std::stringstream textStream;

	void readASCII();
	void cnfFromHD();
	void readBinary();
	uint32_t getCNFSize();
	void initTextStream();
	uint32_t getCNFSizeHD();
	void initBinaryStream();
	void initBinaryStreamHD();
	void readFlag(DataCNFTag tag);
	void readSlot(DataCNFTag tag);
	void readLine(std::string line);
	void readFlag(std::string line);
	void readResident(DataCNFTag tag);
	void readVram(std::vector<std::string> tokens);
	void readSlot(std::vector<std::string> tokens);
	void readLine(DataCNFTag tag, DataCNFTag nextTag);
	void readFile(DataCNFTag tag, DataCNFTag nextTag);

	const CNFREGION cnfRegion[7] = {
		{ "end",		 0x00000000	},
		{ "nocache",	 0x00010000	},
		{ "cache",		 0x00000002	},
		{ "resident",	 0x00000003 },
		{ "delayload",	 0x00000004	},
		{ "delayload_w", 0x00000005	},
		{ "sound",		 0x00000010 }
	};
};