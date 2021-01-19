#include "interface/cli/cli.h"

int main(int argc, char** argv) {
	CLI cli = CLI(argc, argv);
	cli.run("Chrysalis", "1.0");
}