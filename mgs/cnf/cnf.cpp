#include "cnf.h"

DataConfig::DataConfig(std::string filename) {
	this->filename = filename;
}

DataConfig::DataConfig(uint8_t* data, int size) {
	this->data = data;
	this->dataSize = size;
}

DataConfig::~DataConfig() {
	if (madeFromHD) delete[] cnf;
}

void DataConfig::setWorkDir(std::string& workdir) {
	this->workdir = &workdir;
}

void DataConfig::setHandler(CNFHandler* cnfHandler) {
	this->cnfHandler = cnfHandler;
}

const char* DataConfig::getRegionForID(uint32_t id) {
	for (int i = 0; i < 7; i++) {
		if (cnfRegion[i].id == id)
			return cnfRegion[i].name;
	}
	return "";
}

void DataConfig::readVram(std::vector<std::string> tokens) {
	for (int i = 1; i < tokens.size(); i++) {
		if (cnfHandler) cnfHandler->processVram(tokens[i], currentRegion, workdir);
	}
}

uint32_t DataConfig::getCNFSize() {
	return sizeof(DataCNF) + sizeof(DataCNFTag) * cnf->numTags;
}

uint32_t DataConfig::getCNFSizeHD() {
	return sizeof(DataCNFHD) + sizeof(DataCNFTagHD) * cnfHD->numTags;
}

void DataConfig::readSlot(std::vector<std::string> tokens) {
	std::string filename = tokens[1];
	int16_t pageNum = std::stoi(tokens[2]);
	int64_t offset  = std::stoi(tokens[3]);

	if (cnfHandler) cnfHandler->processSlot(filename, pageNum, offset, currentRegion, workdir);
	std::getline(textStream, tokens[0]); //endslot
}

void DataConfig::readFlag(std::string line) {
	std::stringstream lineStream(line);
	std::vector<std::string> tokens = { std::istream_iterator<std::string>{lineStream}, std::istream_iterator<std::string>{} };
	std::string flagName = tokens[0];

	if (flagName == "slot") readSlot(tokens); else
	if (flagName == "vram") readVram(tokens); else
	currentRegion = flagName;
}

void DataConfig::readFile(DataCNFTag tag, DataCNFTag nextTag) {
	int size = nextTag.offset < tag.offset ? 0 : nextTag.offset - tag.offset;
	uint32_t fileext = tag.id >> 24;
	uint32_t filehash = tag.id & 0xFFFFFF;
	std::string filename = formatStrcode(intToHexString(filehash)) + "." + getExtForID(fileext);
	if (cnfHandler) cnfHandler->processFile(filename, currentRegion, workdir, &data[dataPtr + tag.offset], size);
}

void DataConfig::readResident(DataCNFTag tag) {}

void DataConfig::readSlot(DataCNFTag tag) {}

void DataConfig::readFlag(DataCNFTag tag) {
	uint32_t flagID = tag.id & 0xFFFFFF;
	if (!flagID) return;
	currentRegion = getRegionForID(flagID);
	dataPtr += cnfHandler->getNextSectorOffset(dataPtr);
}

void DataConfig::readLine(DataCNFTag tag, DataCNFTag nextTag) {
	switch (tag.id >> 24) {
	case 0x7F:
		readFlag(tag);
		break;
	case 0x7E:
		readResident(tag);
		break;
	case 0x7D:
		readSlot(tag);
		break;
	case 0x0:
		break;
	default:
		readFile(tag, nextTag);
	}
}

void DataConfig::readLine(std::string line) {
	switch (line.at(0)) {
	case 0x2E:
		readFlag(&line[1]);
		break;
	case 0x3F:
		if (cnfHandler) cnfHandler->processAfp(&line[1], currentRegion, workdir);
		break;
	case 0x40:
		if (cnfHandler) cnfHandler->processQar(&line[1], currentRegion, workdir);
		break;
	default:
		if (cnfHandler) cnfHandler->processFile(line, currentRegion, workdir, NULL, 0);
	}
}

void DataConfig::cnfFromHD() {

	int size = sizeof(DataCNF) + sizeof(DataCNFTag) * cnfHD->numTags;
	cnf = (DataCNF*)new uint8_t[size];
	madeFromHD = true;

	cnf->numTags = cnfHD->numTags;
	for (int i = 0; i < cnfHD->numTags; i++) {
		cnf->tags[i].id = cnfHD->tags[i].id;
		cnf->tags[i].offset = cnfHD->tags[i].offset;
	}
}

void DataConfig::initBinaryStream() {
	cnf = (DataCNF*)data;
	dataPtr += getCNFSize();
}

void DataConfig::initBinaryStreamHD() {
	cnfHD = (DataCNFHD*)data;
	dataPtr += getCNFSizeHD();
	cnfFromHD();
}

void DataConfig::initTextStream() {
	std::string cnfTxt = std::string((char*)data, dataSize);
	textStream.str(cnfTxt);
}

void DataConfig::readBinary() {
	PLATFORM == PS3 ? initBinaryStreamHD() : initBinaryStream();

	for (int i = 0; i < cnf->numTags - 1; i++) {
		readLine(cnf->tags[i], cnf->tags[i+1]);
	}
}

void DataConfig::readASCII() {
	initTextStream();
	
	std::string line;
	while (std::getline(textStream, line)) {
		readLine(line);
	}
}

void DataConfig::read(CNFTYPE type) {
	type == BINARY ? readBinary() : readASCII();
}