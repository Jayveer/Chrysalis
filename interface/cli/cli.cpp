#include "cli.h"

CLI::CLI(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;
}

CLI::~CLI() {
}

void CLI::processArgs() {
	processCommands();
	processFile();
}

void CLI::processCommands() {
	while (currentArg < 3 && isCommand(argv[currentArg])) {
		setCommand(argv[currentArg]);
		currentArg++;
	}
}

void CLI::processSlot() {
	uint32_t sector = PLATFORM == PS3 ? 0x1000 : 0x800;
	std::string input = argv[currentArg];
	std::string output = "";
	int pageID = -1;
	currentArg++;

	if (currentArg <= argc -1 ) {
		if (isNumber(argv[currentArg]))
			pageID = argToNumber(argv[currentArg]);
		else
			output = argv[currentArg];
		currentArg++;
	}

	if (currentArg == argc - 1) output = argv[currentArg];

	if (!verifyFile(input)) return;

	Slot slot(input, sector);
	slot.open();

	resetDir(input);
	updateDir("SLOT.KEY", input);

	if (!verifyFile(input)) return;

	printf("Extracting Slot...");
	pageID == -1 ? slot.extractAll(input, output) : slot.extract(input, pageID, output);
}

void CLI::processStage() {
	std::string input = argv[currentArg];
	std::string stageName = "";
	std::string output = "";
	currentArg++;

	if (currentArg <= argc - 1) {
		std::string argString = argv[currentArg];
		isDirectory(argString) ? output = argString : stageName = argString;
		currentArg++;
	}

	if (currentArg == argc - 1) output = argv[currentArg];

	if (!verifyFile(input)) return;

	Stage stage(input);
	stage.open();

	printf("Extracting Stage...");
	stageName == "" ? stage.extractAll(output) : stage.extract(stageName, output);
}

bool CLI::verifyFile(std::string& file) {
	if (!fileExists(file)) {
		printf("couldn't find %s\n", file.c_str());
		printUsage();
		return false;
	}

	return true;
}

void CLI::processFile() {
	std::string filepath = argv[currentArg];
	if (getCurrentDir(filepath) == "SLOT.DAT") processSlot();
	if (getCurrentDir(filepath) == "STAGEDAT.PDT") processStage();
}

void CLI::setCommand(char* arg) {
	if (!strcmp(arg, "-extract") || !strcmp(arg, "-e")) {
		command = EXTRACT;
		return;
	}

	if (!strcmp(arg, "-PS3") || !strcmp(arg, "-ps3")) {
		PLATFORM = PS3;
		return;
	}

	if (!strcmp(arg, "-PSP") || !strcmp(arg, "-psp")) {
		PLATFORM = PSP;
		return;
	}

	printf("command not recogized");
}

void CLI::run(std::string programName, std::string version) {
	printf("Running %s v%s: Visit https://github.com/Jayveer/Chrysalis for updates:\n", programName.c_str(), version.c_str());
	if (!checkInput()) return;
	processArgs();
}

bool CLI::checkInput() {
	if (argc < 2 || argc > 6) {
		printUsage();
		return false;
	}

	return true;
}

int CLI::argToNumber(char* arg) {
	int num;
	sscanf_s(arg, "%d", &num);
	return num;
}

bool CLI::isNumber(char* arg) {
	for (int i = 0; arg[i] != 0; i++) {
		if (!isdigit(arg[i]))
			return false;
	}
	return true;
}

bool CLI::isCommand(char* arg) {
	return arg[0] == 0x2D;
}

void CLI::printUsage() {
	printf(this->USAGE_MESSAGE);
}

void CLI::exit() {
	printf(this->EXIT_MESSAGE);
}