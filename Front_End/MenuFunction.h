#ifndef MenuFunction_H
#define MenuFunction_H

#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include "ManageData.h"
#include "CheckInput.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <windows.h>
#include <direct.h>
using std::cin;
using std::cout;
using std::ios;
using std::string;
using std::vector;
using std::streamsize;
using std::numeric_limits;
using std::setw;
using std::left;
using std::right;
using std::to_string;

void setFieldSize(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);
void addSpeakerType(vector<vector<int>> &type);
void deleteSpeakerType(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);
void addSpeaker(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);
void adjustSpeaker(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);
void deleteSpeaker(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);
void openMap(Field &field, vector<Speaker> &speakers);
void storeSolution(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);
void readSolution(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);
void exitProgram();
void invalidChoice();

#endif // MenuFunction_H