#include "dar.h"

Dar::Dar(std::string filename) {
	this->filename = filename;
}

Dar::~Dar() {

}

int32_t Dar::getNumFiles() {
	return this->numFiles;
}

std::vector<uint8_t> Dar::getFile(int32_t i) {
	return this->files[i].data;
}

void Dar::open() {
	std::ifstream darDat;
	darDat.open(filename, std::ios::binary);
	darDat.read((char*)&this->numFiles, 4);

	for (int i = 0; i < this->numFiles; i++) {
		int align;
		DarEntry entry;

		std::getline(darDat, entry.filename, '\0');
		align = getAlignment(darDat.tellg(), 4);
		darDat.seekg(align, darDat.cur);

		darDat.read((char*)&entry.size, 4);
		align = getAlignment(darDat.tellg(), 16);
		darDat.seekg(align, darDat.cur);

		entry.data.resize(entry.size);
		darDat.read((char*)&entry.data[0], entry.size);
		darDat.seekg(1, darDat.cur);

		files.push_back(entry);
	}

	darDat.close();
}

void Dar::extract(std::string output) {
	updateDir("Dar", output);

	for (int i = 0; i < this->numFiles; i++) {
		writeDataToFile(&files[i].data[0], files[i].size, files[i].filename, output);
	}
}