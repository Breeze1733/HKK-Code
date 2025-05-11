#ifndef MenuFunction_H
#define MenuFunction_H

#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include "CheckInput.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <windows.h> 
#include <fstream>
#include <direct.h>
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::endl;
using std::setw;
using std::left;
using std::right;
using std::setfill;
using std::to_string;
using std::ios;
using std::ifstream;
using std::ofstream;
using std::numeric_limits;
using std::streamsize;

void saveDataToFile(const string &);
void updateData();
void showMenu();
void setFieldSize();
void addSpeaker();
void modifySpeaker();
void deleteSpeaker();
void openMap();
void storeSolution();
void readSolution();
void exitProgram();
void invalidInput();

#endif // MenuFunction_H