#ifndef SHOWMENU_H
#define SHOWMENU_H

#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
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
using std::setfill;
using std::to_string;
void clearConsoleBelow(int);
void clearAboveLines(int);
void setData();
void updateData();
void showTitle();
void showData();
void showCommandMenu();
void setFieldSize();
void addSpeaker();
void modifySpeaker();
void deleteSpeaker();
void openMap();
void menuLogic();

#endif // SHOWMENU_H