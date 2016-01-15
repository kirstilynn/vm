#include "Assembler.h"

#include <iostream>
#include <fstream>
#include <string>  
#include <sstream> 
#include <vector>
#include <map>
#include <set>
#include <array>
#include <cassert>
#include <algorithm>
#include <cstdint>
#include <stdlib.h> 

using namespace std;

const map<string, int> Assembler::OPCODES = {
	{"ADD", 13}, {"SUB", 15}, {"MUL", 16}, {"DIV", 17}, 
	{"LDR", 10}, {"LDB", 12}, {"MOV", 7}, {"TRP", 0}, 
	{"AND", 18}, {"OR", 19}, {"JMP", 1}, {"JMR", 2},
    {"BNZ", 3}, {"BGT", 4}, {"BLT", 5}, {"BRZ", 6},
    {"CMP", 20}, {"LDA", 8}, {"ADI", 14}, {"STR", 9},
    {"STB", 11}, {"JMR", 2}, {"RUN", 30}, {"END", 31},
    {"BLK", 32}, {"LCK", 33}, {"ULK", 34}
};
// const set<string> Assembler::REG_LABEL = {"BNZ", "BGT", "BLT", "BRZ", "LDA",
//     "STR", "LDR", "STB", "LDB"
// }; 
const set<string> Assembler::TYPES = {".BYT", ".INT"};

string getFileExtension(const string& filename) {
    if(filename.find_last_of(".") != string::npos)
        return filename.substr(filename.find_last_of(".")+1);
    return "";
}
void Assembler::saveCharToMem(char value, int counter) {
	memcpy(mem+counter, &value, 1);
}
void Assembler::saveIntToMem(int value, int counter) {
    memcpy(mem+counter, &value, 4);
}
void Assembler::saveToTable(string label, int counter) {
    if(symb_tbl.find(label) != symb_tbl.end()) //label already exists, warn user
        cerr << "warning: Label '" + label + "' already defined" <<"\n";
    symb_tbl.insert(pair<string, int>(label, counter)); //save mem location of directive into symbol table
}
void Assembler::saveInstructToMem(vector<string> input) {
    Instruction instr;
    instr.opCode = OPCODES.at(input[0]);
    saveIntToMem(instr.opCode, counter);
    //OPERAND 1
    if(input.size() > 1) {
        if(input[1].at(0) == 'R' && input[1].length() > 1) { //OPCODE REGISTER 
            if(isdigit(input[1].at(1))) {
                instr.opd1 = stoi(input[1].substr(1));
                saveIntToMem(instr.opd1, counter+4);
            }
            else { //LABEL THAT STARTS WITH R (ex. RESET)
                instr.opd1 = symb_tbl.at(input[1]);
                saveIntToMem(instr.opd1, counter+4);
            }
        }
        else if(isdigit(input[1].at(0))) { //OPCODE IMM
            instr.opd1 = atoi(input[1].c_str());
            saveIntToMem(instr.opd1, counter+4);
        }
        else {  //OPCODE LABEL
            //cout << "op label" << endl;
            if(symb_tbl.find(input[1]) != symb_tbl.end()) {
                instr.opd1 = symb_tbl.at(input[1]);
                saveIntToMem(instr.opd1, counter+4);
            }
            else {
                if (input[1].compare("SL")==0) {
                    instr.opd1 = 9;
                    saveIntToMem(instr.opd1, counter+4);
                }
                else if (input[1].compare("SP")==0) {
                    instr.opd1 = 10;
                    saveIntToMem(instr.opd1, counter+4);
                }
                else if (input[1].compare("FP")==0) {
                    instr.opd1 = 11; 
                    saveIntToMem(instr.opd1, counter+4);
                }
                else if (input[1].compare("SB")==0) {
                    instr.opd1 = 12; 
                    saveIntToMem(instr.opd1, counter+4);
                }
                else {
                    throw runtime_error(to_string(lineNum) + ": Label'" + input[1] + "' not defined");
                }
            }
        }
    }

    
    //OPERAND 2
    if (input.size() == 3) { //OPCODE OPD1 OPD2
        if(input[2].at(0) == 'R' && input[2].length() > 1) { //OPCODE REGISTER REGISTER
            instr.opd2 = stoi(input[2].substr(1));
            saveIntToMem(instr.opd2, counter+8);
            if(instr.opCode==9) {
                instr.opCode = 21;
                saveIntToMem(instr.opCode, counter);
            }
            if(instr.opCode==10) {
                instr.opCode = 22;
                saveIntToMem(instr.opCode, counter);
            }
            if(instr.opCode==11) {
                instr.opCode = 23;
                saveIntToMem(instr.opCode, counter);
            }
            if(instr.opCode==12) {
                instr.opCode = 24;
                saveIntToMem(instr.opCode, counter);
            }
            // cout << ' ' << instr.opd2 << endl;
        }
        else if(isdigit(input[2].at(0)) || (input[2].at(0) == '-')) { //OPCODE OPD1 IMM
            instr.opd2 = atoi(input[2].c_str());
            saveIntToMem(instr.opd2, counter+8);
        }
        else { //OPCODE REGISTER LABEL
            if(symb_tbl.find(input[2]) != symb_tbl.end()) {
                instr.opd2 = symb_tbl.at(input[2]);
                saveIntToMem(instr.opd2, counter+8);
            }
            else {
                if (input[2].compare("SL")==0) {
                    instr.opd2 = 9;
                    saveIntToMem(instr.opd2, counter+8);
                }
                else if (input[2].compare("SP")==0) {
                    instr.opd2 = 10;
                    saveIntToMem(instr.opd2, counter+8);
                }
                else if (input[2].compare("FP")==0) {
                    instr.opd2 = 11; 
                    saveIntToMem(instr.opd2, counter+8);
                }
                else if (input[2].compare("SB")==0) {
                    instr.opd2 = 12; 
                    saveIntToMem(instr.opd2, counter+8);
                }
                else {
                    throw runtime_error(to_string(lineNum) + ": Label'" + input[2] + "' not defined");
                }



                if(instr.opCode==9) {
                    instr.opCode = 21;
                    saveIntToMem(instr.opCode, counter);
                }
                if(instr.opCode==10) {
                    instr.opCode = 22;
                    saveIntToMem(instr.opCode, counter);
                }
                if(instr.opCode==11) {
                    instr.opCode = 23;
                    saveIntToMem(instr.opCode, counter);
                }
                if(instr.opCode==12) {
                    instr.opCode = 24;
                    saveIntToMem(instr.opCode, counter);
                }

            }
        }
    }
}
int Assembler::runAssembler(string filename) {
	char charVal;
	int intVal;
	int PC = -1;
	try {
		if (getFileExtension(filename) != "asm")
			throw runtime_error("What am I supposed to do with that? I need a .asm file");
        while(!finished) {
            ifstream file(filename);
            if (file.is_open()) {
                string input, comment;
                while(getline(file, input)) {
                    lineNum++;
                    stringstream iss(input);
                    getline(iss, input, ';'); //parse out ;comment sections 
                    //tokenize lines
                    stringstream strstr(input);
                    istream_iterator<string> it(strstr);
                    istream_iterator<string> end;
                    vector<string> line(it, end);
                    
                    //passes
                    if (line.size() < 1) {
                        if(line.size() != 0)
                            throw runtime_error(to_string(lineNum) + ": Incorect syntax '" + input + "'");
                    }
                    else if(TYPES.find(line[0])!=TYPES.end()) { // DATA with no label (ARRAY)
                            if (line.size() == 2) { //correct syntax
                                if (line[0].compare(".BYT") == 0) { //BYT
                                    if(!pass1) {
                                        if(line[1].compare("\'\\n\'") == 0 ) { //NEWLINE CHAR
                                            charVal = '\n';
                                            saveCharToMem(charVal, counter);
                                        }
                                        else {
                                            charVal = line[1].at(1);
                                            saveCharToMem(charVal, counter);
                                        }
                                    }
                                    counter+=1;
                                }
                                else if (line[0].compare(".INT") == 0) { //INT
                                    if(!pass1) {
                                        intVal = atoi(line[1].c_str());
                                        //cout << "debug: " << intVal;
                                        saveIntToMem(intVal, counter);
                                       // cout << "debug: " << intVal << ", " << counter << endl;
                                    }
                                    counter +=4;
                                }
                                else {
                                    assert("Unhandeled type.");
                                }
                            }
                            else if (line.size() == 3 && line[1].compare("'")==0 && line[2].compare("'")==0) { //data is a space ex. ' '
                                 if(!pass1) {
                                    charVal = ' ';
                                    saveCharToMem(charVal, counter);
                                 }
                                counter +=1;
                            }
                            else {
                                throw runtime_error(to_string(lineNum) + ": Incorect syntax '" + input + "'");
                            }
                    }
                    else if (OPCODES.find(line[0])!=OPCODES.end()) { // instruction with no label
                        if(pass1) {
                        	if(PC < 0)
                        		PC = counter;
                        }
                        else {
                            //convert to byte code then save into memory
                            saveInstructToMem(line);
                        }
                        counter +=12;
                    }
                    else if(OPCODES.find(line[1])!=OPCODES.end() || TYPES.find(line[1])!=TYPES.end()) { //it better be a label, meaning the next token should be type or opcode
                        if (TYPES.find(line[1])!=TYPES.end()) { //Loading data into memory
                            if (line.size() == 3) { //correct syntax
                                if (line[1].compare(".BYT") == 0) { //BYT
                                    if(pass1) {
                                    	saveToTable(line[0], counter);
                                    }
                                    else {
                                        if(line[2].compare("\'\\n\'") == 0 ) { //NEWLINE CHAR
                                        	charVal = '\n';
                                            saveCharToMem(charVal, counter);
                                        }
                                        else {
                                        	charVal = line[2].at(1);
                                            saveCharToMem(charVal, counter);
                                        }
                                    }
                                    counter+=1;
                                }
                                else if (line[1].compare(".INT") == 0) { //INT
                                    if(pass1) {
                                    	saveToTable(line[0], counter);
                                    }
                                    else {
                                    	intVal = atoi(line[2].c_str());
                                    	//cout << "debug: " << intVal;
                                        saveIntToMem(intVal, counter);
                                        memcpy(&intVal, mem+counter, 4);
                                       // cout << "debug: " << intVal << ", " << counter << endl;
                                    }
                                    counter +=4;
                                }
                                else {
                                    assert("Unhandeled type.");
                                }
                            }
                            else if (line.size() == 4 && line[2].compare("'")==0 && line[3].compare("'")==0) { //data is a space ex. ' '
                                 if(pass1) {
                                    saveToTable(line[0], counter);
                                 }
                                 else {
                                 	charVal = ' ';
                                 	saveCharToMem(charVal, counter);
                                 }
                                counter +=1;
                            }
                            else {
                                throw runtime_error(to_string(lineNum) + ": Incorect syntax '" + input + "'");
                            }
                        }
                        else if (OPCODES.find(line[1])!=OPCODES.end()) { //instruction set with label
                            if(pass1) {
                            	if(PC < 0)
                        			PC = counter;
                            	saveToTable(line[0], counter);
                                //cout << "SAVED: " << line[0] << "at "<< counter << endl;
                             }
                            else {
                               
                                line.erase (line.begin()); //remove label
                                saveInstructToMem(line);
                            }
                            counter +=12;
                        }
                        else
                            throw runtime_error(to_string(lineNum) + ": Error in code '" + line[1] + "'" );
                    }
                    else {
                        throw runtime_error(to_string(lineNum) + ": Error in line code '" + line[1] + "'");
                    }

                }
            }
            else
                throw runtime_error("I can't open that file.");
            file.close();
            lineNum = 0;
           
            if(pass1) {
                pass1 = false;
                counter = 17;
            }
            else 
                finished = true;
        }
		return PC;
	}
	catch (exception& e) {
		cerr << e.what() << '\n';
		return -1;
	}
}

int Assembler::findNextThread (int current) {
    for (int i = current+1; i < threads.size(); i++) {
        if (threads[i] != -1)
            return i;
    }
    return 0;
}
bool Assembler::threadsRunning() {
    for (int i = 1; i < threads.size(); i++) {
        if (threads[i] != -1)
            return true;
    }
    return false;
}

void Assembler::runVM(int PC) {
	bool running = true;
	int intVal, currentThread = 0, base = (sizeof(mem) / sizeof(mem[0]))-5;
	char charVal;
	string opCode = "";
	Instruction instruction;
	reg[8] = PC;
    reg[9] = (sizeof(mem) / sizeof(mem[0]))-THREAD_SIZE;
    reg[10] = (sizeof(mem) / sizeof(mem[0]))-57; //point to first word, make room for reg
    reg[11] = 0;
    reg[12] = (sizeof(mem) / sizeof(mem[0]))-57; //point to first word, make room for reg
    threads.fill(-1); // deactivate all threads
    threads[0] = 0; //main thread is running
    currentThread = 0; //set current thread to main id

	while(running) {
        //cout << "reg: " << base-(currentThread*THREAD_SIZE)-48 << endl;
        memcpy(mem+base-(currentThread*THREAD_SIZE)-48, reg, 52); // save current state of registers
        currentThread = findNextThread(currentThread);
        //ßcout << reg[8] << endl;
        memcpy(reg, mem+base-(currentThread*THREAD_SIZE)-48, 52); // copy in new reg data
        //cout << "thread: " << currentThread << endl;
		memcpy(&intVal, mem+reg[8], 4);
		instruction.opCode = intVal;
		memcpy(&intVal, mem+reg[8]+4, 4);
		instruction.opd1 = intVal;
		memcpy(&intVal, mem+reg[8]+8, 4);
		instruction.opd2 = intVal;

		for (map<string,int>::const_iterator it = OPCODES.begin(); it != OPCODES.end(); ++it) {
            //cout << it->second  << " == " << instruction.opCode << " = " << (it->second == instruction.opCode) << endl;
        	if(it->second == instruction.opCode) {
             	opCode = it->first;
            }
    	}
        if (instruction.opCode == 21)
            opCode = "STR";
        if (instruction.opCode == 22)
            opCode = "LDR";
        if (instruction.opCode == 23)
            opCode = "STB";
        if (instruction.opCode == 24)
            opCode = "LDB";

        //cout << opCode << ": " << instruction.opCode << ' ' << instruction.opd1 << ' ' << instruction.opd2 << endl;
        assert(opCode.compare("") || 
            instruction.opCode == 21 || 
            instruction.opCode == 22  || 
            instruction.opCode == 23 ||
            instruction.opCode == 24); //There should be no undefined opcodes, the passes should have caught that.

		switch(instruction.opCode) {
			case 0: 	//TRP
				if(instruction.opd1 == 0) {
                  //  cout << "in here" << endl;
					running = false;
                    break;
                   // cout << "stop" << endl;
                }
				else if (instruction.opd1 == 1) {
					cout << reg[0];
					reg[8] +=12;
				}
                else if (instruction.opd1 == 2) {
                    cin >> intVal;
                    reg[0] = intVal;
                    reg[8] +=12;
                }
				else if (instruction.opd1 == 3) {
					memcpy(&charVal, reg, 1);
					cout << charVal;
					reg[8] +=12;
				}
                else if(instruction.opd1 == 4) {
                    charVal = getchar();
                    memcpy(reg, &charVal, 1);
                    reg[8] +=12;
                }
                else {
                    assert("unhandled trap");
                }
				break;
            case 1:     //JMP
                reg[8] = instruction.opd1;
                break;
            case 2:     //JMR
                reg[8] = reg[instruction.opd1];
                break;
            case 3:     //BNZ
                if(reg[instruction.opd1] != 0)
                    reg[8] = instruction.opd2;
                else
                   reg[8] += 12; 
                break;
            case 4:     //BGT
                if(reg[instruction.opd1] > 0)
                    reg[8] = instruction.opd2;
                else
                    reg[8] += 12;
                break;
            case 5:     //BLT
                if(reg[instruction.opd1] < 0)
                    reg[8] = instruction.opd2;
                else
                    reg[8] += 12;
                break;
            case 6:     //BRZ
                if(reg[instruction.opd1] == 0)
                    reg[8] = instruction.opd2;
                else
                    reg[8] += 12;
                break;
			case 7:		//MOV
                memcpy(&intVal, reg+(instruction.opd2), 4);
                memcpy(reg+(instruction.opd1), &intVal, 4);
				reg[8] += 12;
				break;
            case 8:     //LDA
                reg[instruction.opd1] = instruction.opd2;
                reg[8] += 12;
                break;
            case 9:     //STR
                memcpy(mem+instruction.opd2, &reg[instruction.opd1], 4);
                reg[8] += 12;
                break;
			case 10: 	//LDR
                memcpy(&intVal, mem+instruction.opd2, 4); //grab int from memory
                reg[instruction.opd1] = intVal;
				reg[8] +=12;
				break;
            case 11:    //STB
                memcpy(&charVal, reg+(instruction.opd1), 1);
                memcpy(mem+instruction.opd2, &charVal, 1);
                reg[8] += 12;
                break;
			case 12:	//LDB
                reg[instruction.opd1] = 0; //zero out all the bytes before storing char byte
                memcpy(&charVal, mem+instruction.opd2, 1); // load char from memory 
                memcpy(reg+instruction.opd1, &charVal, 1); //load char into first byte of reg 
				reg[8] +=12;
				break;
			case 13:	//ADD
				reg[instruction.opd1] = reg[instruction.opd1]+reg[instruction.opd2];
				reg[8] +=12;
				break;
            case 14:    //ADI
                reg[instruction.opd1] += instruction.opd2;
                reg[8] +=12;
                break;
			case 15:	//SUB
               // cout << reg[instruction.opd1] << " - " << reg[instruction.opd2] << " = " << reg[instruction.opd1]-reg[instruction.opd2] << endl;
				reg[instruction.opd1] = reg[instruction.opd1]-reg[instruction.opd2];
				reg[8] +=12;
				break;
			case 16:	//MULT
				reg[instruction.opd1] = reg[instruction.opd1]*reg[instruction.opd2];
				reg[8] +=12;
				break;
			case 17:	//DIV
				reg[instruction.opd1] = reg[instruction.opd1]/reg[instruction.opd2];
				reg[8] +=12;
				break;
            case 18:    //AND
                reg[instruction.opd1] = reg[instruction.opd1] & reg[instruction.opd2];
                reg[8] +=12;
                break;
            case 19:    //OR
                reg[instruction.opd1] = reg[instruction.opd1] | reg[instruction.opd2];
                reg[8] +=12;
                break;
            case 20:    //CMP
                if(reg[instruction.opd1] == reg[instruction.opd2])
                    reg[instruction.opd1] = 0;
                else if (reg[instruction.opd1] > reg[instruction.opd2])
                    reg[instruction.opd1] = 1;
                else
                    reg[instruction.opd1] = -1;
                reg[8] +=12;
                break;
            case 21:    //STR Register Indirect
                memcpy(mem+reg[instruction.opd2], &reg[instruction.opd1], 4);
                reg[8] +=12;
                break;
            case 22:    //LDR Register Indirect
                memcpy(&intVal, mem+reg[instruction.opd2], 4); //grab int from memory
                reg[instruction.opd1] = intVal;
                reg[8] +=12;
                break;
            case 23:    //STB Register Indirect
                memcpy(&charVal, reg+(instruction.opd1), 1);
                memcpy(mem+reg[instruction.opd2], &charVal, 1);
                reg[8] +=12;
                break;
            case 24:    //LDB Register Indirect
                memcpy(&charVal, mem+reg[instruction.opd2], 1); // load char from memory
                memcpy(reg+(instruction.opd1), &charVal, 1); //load char into first byte of reg
                reg[8] +=12;
                break;
            case 30:    //RUN
                for (int i=1; i < threads.size(); i++){
                    if(threads[i] == -1) {
                        reg[instruction.opd1] = i;
                        threads[i] = i;
                        reg[8] +=12;
                        memcpy(mem+base-(currentThread*THREAD_SIZE)-48, reg , 52); //copy registers into mem
                        currentThread = i;
                        reg[8] = instruction.opd2;
                        reg[9] = base - (i * THREAD_SIZE) - THREAD_SIZE;  
                        reg[10] = base - (i * THREAD_SIZE) - 52;
                        reg[11] = 0;
                        reg[12] = base - (i * THREAD_SIZE) - 52;  
                        memcpy(mem+base-(i*THREAD_SIZE), reg , 52); //copy registers into mem

                        break;
                    }
                }
                break;
            case 31:   //END
                threads[currentThread] = -1;
                reg[8] +=12;
                break;
            case 32:    //BLK
                if(currentThread==0 && !threadsRunning()){
                    reg[8] +=12;
                }
                break;
            case 33:    //LCK
                memcpy(&intVal, mem+instruction.opd1, 4);
                //cout <<"val: " << intVal << endl; 
                if(intVal == -1) { //if thread is unlocked
                    intVal = currentThread;
                    memcpy(mem+instruction.opd1, &intVal, 4); 
                    reg[8] +=12;
                }
                break;
            case 34:    //ULK
                memcpy(&intVal, mem+instruction.opd1, 4); 
                if(intVal == currentThread) { //if thread that locked is the same as current
                    intVal = -1;
                    memcpy(mem+instruction.opd1, &intVal, 4); 
                }
                reg[8] +=12;
                break;
			default:
				assert("unhandled opcode. Handle it gurl.");
				break;
		}
	}
    //cout << "finished" << endl;
}




