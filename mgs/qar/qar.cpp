#include "qar.h"

Qar::Qar(std::string filename) {
	this->filename = filename;
}

Qar::~Qar() {

}

void Qar::open() {
	std::ifstream qarDat;
	qarDat.open(filename, std::ios::binary);

	qarDat.seekg(-4, qarDat.end);
	qarDat.read((char*)&header.offset, 4);

	qarDat.seekg(header.offset, qarDat.beg);
	qarDat.read((char*)&header.numFiles, 4);

	header.dataInfo.resize(header.numFiles);
	qarDat.read((char*)&header.dataInfo[0], header.numFiles * sizeof(QarDataInfo));

	header.filenames.resize(header.numFiles);
	for (int i = 0; i < header.numFiles; i++) {
		std::getline(qarDat, header.filenames[i], '\0');
	}

	qarDat.seekg(0, qarDat.beg);
	txpFiles.resize(header.numFiles);

	for (int i = 0; i < header.numFiles; i++) {
		int size = header.dataInfo[i].fileSize;
		txpFiles[i].resize(size);
		qarDat.read((char*)&txpFiles[i][0], size);

		int align = getAlignment(qarDat.tellg(), 0x80);
		qarDat.seekg(align, qarDat.cur);
	}

	qarDat.close();
}

void Qar::extract(std::string output) {
	updateDir("Qar", output);

	for (int i = 0; i < header.numFiles; i++) {
		writeDataToFile(&txpFiles[i][0], header.dataInfo[i].fileSize, header.filenames[i], output);
	}
}
