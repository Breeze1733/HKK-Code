#ifndef CheckInput_H
#define CheckInput_H
// 是用来检测输入是否合法

#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::string;
void clearConsoleBelow(const int & linesToKeep);
void clearAboveLines(const int & linesToClear);
int getInput(const int & min_value, const int & max_value);
int askQuestion(const int & min_value, const int & max_value, const string & question);

#endif // CheckInput_H