#include "slot.h"

Slot::Slot(std::string filename) {
	this->filename = filename;
}

Slot::Slot(std::string filename, uint32_t sector) {
	this->filename = filename;
	this->sector = sector;
}

Slot::~Slot() {

}

std::string Slot::getFilename() {
	return this->filename;
}

std::string Slot::getCreatedDate() {
	std::string createdDate;
	createdDate.resize(20);
	time_t rawtime = (time_t)this->header.timestamp;
	tm ptm;
	gmtime_s(&ptm, &rawtime);

	sprintf_s(&createdDate[0], 
			  20,
			  "%02d/%02d/%04d %02d:%02d:%02d", 
			  ptm.tm_mday, 
			  ptm.tm_mon, 
			  (1900 + ptm.tm_year), 
			  ptm.tm_hour, 
			  ptm.tm_min, 
			  ptm.tm_sec );

	return createdDate;
}

void Slot::open() {
	std::ifstream slotDat;
	slotDat.open(filename, std::ios::binary);
	slotDat.read((char*)&this->header, sizeof(SlotHeader));
	slotDat.close();
}

uint8_t* Slot::decompressPage(uint8_t* compressedPage, int& size) {
	ZlibWrapper zlib;
	SlotCompressedHeader* compressedHeader = (SlotCompressedHeader*)compressedPage;
	uint8_t* decompressedPage = new uint8_t[compressedHeader->decompressedSize];

	zlib.zlibInflate((char*)compressedHeader->compressedData,
		(char*)decompressedPage, compressedHeader->compressedSize,
		compressedHeader->decompressedSize,
		&size,
		1);

	return decompressedPage;
}

uint8_t* Slot::_decryptPage(uint32_t key, uint32_t saltA, uint32_t saltB, uint32_t saltC, int offset, int size, bool makeKey) {
	std::ifstream slotDat;
	slotDat.open(filename, std::ios::binary);

	slotDat.seekg(offset, SEEK_SET);
	uint8_t* dataBuffer = new uint8_t[size];
	slotDat.read((char*)dataBuffer, size);
	uint32_t keyA = decryptor.genericDecode(key, saltA, saltB, saltC, 0, size, dataBuffer, makeKey);

	slotDat.close();
	return dataBuffer;
}

void Slot::writeDataToFile(uint8_t* data, int size, const std::string& filename, std::string& output) {
	if (!std::filesystem::exists(output))
		std::filesystem::create_directories(output);

	updateDir(filename, output);
		std::ofstream ofs(output, std::ofstream::binary);
		ofs.write((char*)data, size);
		ofs.close();
	resetDir(output);
}

uint8_t* Slot::decryptPage(std::string keyFile, uint16_t pageID) {
	std::ifstream slotKey;
	slotKey.open(keyFile, std::ios::binary);

	SlotKeyHeader keyHeader;
	slotKey.read((char*)&keyHeader, 12);

	unsigned int stride = PLATFORM == PS3 ? 0x14 : 0x0C;

	unsigned int offset = (pageID * stride);
	slotKey.seekg(offset, SEEK_CUR);

	SlotKeyEntry keyEntry;
	slotKey.read((char*)&keyEntry, 0x0C);

	unsigned int start = (keyEntry.firstPage & 0xFFFFF) * sector;
	unsigned int end =   (keyEntry.lastPage  & 0xFFFFF) * sector;
	unsigned int size = end - start;

	uint8_t* decryptedPage = _decryptPage(0, keyHeader.saltA, keyHeader.saltB, keyHeader.saltC, start, size, true);
	
	return decryptedPage;
}

void Slot::extractFiles(uint8_t* pageData, int pageSize, std::string& output) {
	DataConfig cnf = DataConfig(pageData, pageSize);
	cnf.setHandler(this);
	cnf.setWorkDir(output);
	cnf.read(BINARY);
}

void Slot::extractAll(std::string keyFile, const std::string& output) { 
	for (int i = 0; i < header.numPages; i++) {
		extract(keyFile, i, output);
	}
}

void Slot::extract(std::string keyFile, uint16_t pageID, std::string output) {
	updateDir("slot", output);
	updateDir(std::to_string(pageID), output);

	int pageSize;
	uint8_t* decryptedPage = decryptPage(keyFile, pageID);
	uint8_t* decompressedPage = decompressPage(decryptedPage, pageSize);
	delete[] decryptedPage;

	extractFiles(decompressedPage, pageSize, output);
	delete[] decompressedPage;
}

/* CNF */

uint64_t Slot::getNextSectorOffset(uint64_t currentOffset) {
	return getAlignment(currentOffset, sector);
}

uint32_t Slot::getSectorSize() {
	return this->sector;
}

void Slot::processQar(std::string filename, std::string region, std::string* workDir) {
	int dummy = 0;
}

void Slot::processSlot(std::string filename, int16_t page, int64_t offset, std::string region, std::string* workDir) {
	int dummy = 0;
}

void Slot::processVram(std::string filename, std::string region, std::string* workDir) {
	int dummy = 0;
}

void Slot::processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size) {
	writeDataToFile(fileData, size, filename, *workDir);
}

void Slot::processAfp(std::string filename, std::string region, std::string* workDir) {
	int dummy = 0;
}