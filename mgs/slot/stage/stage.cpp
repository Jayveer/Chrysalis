#include "stage.h"

Stage::Stage(std::string filename) : Slot(filename) {
}

Stage::Stage(std::string filename, uint32_t sector) : Slot(filename, sector) {
}

Stage::~Stage() {

}

uint8_t* Stage::decompressFile(uint8_t* compressedFile, uint16_t pageID, int& size) {
	ZlibWrapper zlib;
	StageCompressedHeader* compressedHeader = (StageCompressedHeader*)compressedFile;
	uint8_t* decompressedPage = new uint8_t[compressedHeader->decompressedSize];

	zlib.zlibInflate((char*)compressedHeader->compressedData,
		(char*)decompressedPage, table[pageID].size - 4,
		compressedHeader->decompressedSize,
		&size,
		1);

	return decompressedPage;
}

uint8_t* Stage::decryptFile(uint16_t pageID) {
	return _decryptPage(0, this->header.saltA, this->header.saltB, this->header.saltC, table[pageID].offset, table[pageID].size, true);
}

void Stage::open() {
	int size = 0x14;
	std::ifstream stageDat;
	stageDat.open(filename, std::ios::binary);

	stageDat.read((char*)&this->header, sizeof(StageHeader));
	uint32_t keyA = decryptor.genericDecode(0, this->header.saltA, this->header.saltB, this->header.saltC, 0, size, (uint8_t*)&header.unknownA, true);

	table.resize(header.numPages);
	size = sizeof(StageTable) * header.numPages;
	stageDat.read((char*)&this->table[0], size);
	keyA = decryptor.genericDecode(keyA, this->header.saltA, this->header.saltB, this->header.saltC, 0, size, (uint8_t*)&table[0]);

	lookup.resize(header.numPages);
	size = sizeof(StageLookup) * header.numPages;
	stageDat.read((char*)&this->lookup[0], size);
	keyA = decryptor.genericDecode(keyA, this->header.saltA, this->header.saltB, this->header.saltC, 0, size, (uint8_t*)&lookup[0]);

	stageDat.close();
}

uint32_t Stage::findIndexForHash(int32_t hash) {
	uint32_t index = 0;

	do {
		if (lookup[index].key == hash)
			return index;
		index = lookup[index].key < hash ? lookup[index].nextOffsetLT : lookup[index].nextOffsetGT;
		index /= 0x10;
	} while (index);

	return -1;
}

uint8_t* Stage::extractFile(const std::string& fileName, std::string stageName, int& size) {
	uint32_t fileHash = fileStrcode(fileName.c_str());
	uint32_t folderHash  = strcode(stageName.c_str());

	uint32_t hash = lookupHash(fileHash, folderHash);
	uint32_t fileIdx = findIndexForHash(hash);
	if (fileIdx == -1) return 0;

	uint8_t* decryptedFile = decryptFile(fileIdx);
	uint8_t* decompressedFile = decompressFile(decryptedFile, fileIdx, size);
	delete[] decryptedFile;

	return decompressedFile;
}

void Stage::extract(const std::string& stageName, std::string output) {
	updateDir("stage", output);
	updateDir(stageName, output);
		
	int size = 0;
	uint8_t* cnfData = extractFile("data.cnf", stageName, size);
	if (!cnfData) return;

	DataConfig cnf = DataConfig(cnfData, size);
	cnf.setHandler(this);
	cnf.setWorkDir(output);
	cnf.read();

	writeDataToFile(cnfData, size, "data.cnf", output);
	delete[] cnfData;
}

void Stage::extract(uint16_t fileIdx, std::string output) {
	updateDir("stage", output);
	if (fileIdx > header.numPages) return;

	int size = 0;
	uint8_t* decryptedFile = decryptFile(fileIdx);
	uint8_t* decompressedFile = decompressFile(decryptedFile, fileIdx, size);
	delete[] decryptedFile;
	writeDataToFile(decompressedFile, size, std::to_string(fileIdx), output);
}

void Stage::extractToDir(const std::string& filename, const std::string& stageName, std::string& output) {
	int size = 0;
	uint8_t* data = extractFile(filename, stageName, size);
	if (!data) return;
	writeDataToFile(data, size, filename, output);
	delete[] data;
}

int Stage::getNumStages() {
	return PLATFORM == PS3 ? numStagesHD : numStages;
}

void Stage::extractAll(std::string output) {
	for (int i = 0; i < getNumStages(); i++) {
		extract(stages[i], output);
	}
}

void Stage::extractAllIdx(std::string output) {
	for (int i = 0; i < header.numPages; i++) {
		extract(i, output);
	}
}

/* CNF Callbacks */

void Stage::processQar(std::string filename, std::string region, std::string* workDir) {
	std::string stageName = getCurrentDir(*workDir);
	extractToDir(filename, stageName, *workDir);
}


void Stage::processVram(std::string filename, std::string region, std::string* workDir) {
	std::string stageName = getCurrentDir(*workDir);
	extractToDir(filename, stageName, *workDir);
}

void Stage::processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size) {
	std::string stageName = getCurrentDir(*workDir);
	extractToDir(filename, stageName, *workDir);
}

void Stage::processAfp(std::string filename, std::string region, std::string* workDir) {
	std::string stageName = getCurrentDir(*workDir);
	extractToDir(filename, stageName, *workDir);
}

void Stage::processSlot(std::string filename, int16_t page, int64_t offset, std::string region, std::string* workDir) {}