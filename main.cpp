#include "Assembler.h"

using namespace std;

int main(int argc, const char * argv[])
{
	Assembler assembler;
	if (argc < 2) {
		cerr << "I need the name of your .asm file silly. Try again." << endl;
		return 0;
	}
	string filename = argv[1];
	int PC = assembler.runAssembler(filename);
	if (PC > 0) {
		assembler.runVM(PC);
	}
} 