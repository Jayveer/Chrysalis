#pragma once
#include "../common/fileutil.h"

#include <string>
#include <fstream>

struct DarEntry {
	std::string filename;
	uint32_t size;
	std::vector<uint8_t> data;
};

struct DarHeader {
	int32_t numFiles;
};

class Dar {
public:
	Dar(std::string filename);
	~Dar();

	void open();
	int32_t getNumFiles();
	void extract(std::string output = "");
	std::vector<uint8_t> getFile(int32_t i);
private:
	int32_t numFiles;
	std::vector<DarEntry> files;
	std::string filename = "cache.dar";
};