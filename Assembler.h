#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <array>

#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

const int SIZE = 1000000;
const int THREAD_SIZE = 50000;
// template<class T>
class Assembler {
	const static std::map<std::string, int> OPCODES;
	const static std::set<std::string> TYPES;
	struct Instruction {
        int opCode;
        int opd1;
        int opd2;
    };
	std::map<std::string, int> symb_tbl;
	char mem[SIZE];
	int reg[13];
	std::array<int,5> threads;
	int lineNum = 0, counter = 17;
    bool pass1 = true, finished = false;

	void saveCharToMem(char, int );
	void saveIntToMem(int, int );
	void saveToTable(std::string, int );
	void saveInstructToMem(std::vector<std::string> input);
	int findNextThread (int);
	bool threadsRunning();

public:
	int runAssembler(std::string);
	void runVM(int);
};
#endif
