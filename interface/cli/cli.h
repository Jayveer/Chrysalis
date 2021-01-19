#pragma once
#include <filesystem>

#include "../../mgs/slot/stage/stage.h";

typedef enum CLI_COMMAND {
	EXTRACT,

} CLI_COMMAND;

class CLI {
public:
	CLI(int argc, char** argv);
	~CLI();

	void run(std::string programName, std::string version);
	void exit();
private:
	int argc;
	char** argv;
	int currentArg = 1;
	CLI_COMMAND command = EXTRACT;

	void printUsage();
	bool checkInput();
	void processArgs();
	void processFile();
	void processSlot();
	void processStage();
	void processCommands();
	bool isNumber(char* arg);
	bool isCommand(char* arg);
	void setCommand(char* arg);
	int argToNumber(char* arg);
	bool verifyFile(std::string &file);
	
	const char* EXIT_MESSAGE = "Exiting\n";
	const char* USAGE_MESSAGE = "Usage:\t Chrysalis.exe <options> SLOT.DAT  [PAGEID] [OUTPUTDIRECTORY] \n"
									  "\t Chrysalis.exe <options> STAGEDAT.PDT  [STAGENAME] [OUTPUTDIRECTORY] \n"
									  "\t options: -PSP/-psp to extract PSP game files (default value) \n"
									  "\t\t  -PS3/-ps3 to extract PS3 game files \n"
									  "\t\t  -extract/-e to extract (default value) redundant for now \n\n"
		"Options are not required and will revert to defaults if not supplied\n"
		"If PAGEID or STAGENAME aren't provided all files will be extracted\n"
		"IF OUTPUTDIRECTORY is not provided files will be extracted to wherever the tool is run from\n"
		"SLOT.KEY is required to be in the same directory as SLOT.DAT\n";

};