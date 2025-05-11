#ifndef ManageData_H
#define ManageData_H

#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include "CheckInput.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

void saveDataToFile(const string & outputPath, Field &field, vector<Speaker> &speakers);
void storeDataToFile(const string & outputPath, Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);
void getDataFromFile(const string & inputPath, Field &field, vector<Speaker> &speakers, vector<vector<int>> &type);

#endif // ManageData_H