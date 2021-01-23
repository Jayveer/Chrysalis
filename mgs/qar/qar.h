#pragma once
#include <string>

#include "../common/fileutil.h"

struct QarDataInfo {
	uint32_t fileInfo;
	uint32_t fileSize;
};

struct QarHeader {
	int32_t numFiles;
	std::vector<QarDataInfo> dataInfo;
	std::vector<std::string> filenames;
	uint32_t offset;
};

class Qar{
public:
	QarHeader header;
	std::vector<std::vector<uint8_t>> txpFiles;
	Qar(std::string filename);
	~Qar();

	void open();
	void extract(std::string output);
private:
	std::string filename = "cache.qar";
};