#ifndef SHOWMENU_H
#define SHOWMENU_H

#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include <iostream>
#include <iomanip>
#include <vector>
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
void clearConsoleBelow(int);
void clearAboveLines(int);
void showTitle();
void showData();
void showMenu();
void setFieldSize();
void addSpeaker();
void setData();
void openMap();
void menuLogic();

#endif // SHOWMENU_H