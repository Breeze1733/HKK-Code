#ifndef SystemEntrance_H
#define SystemEntrance_H

#include "MenuFunction.h"

void showTitle();
void showData(Field &, vector<Speaker> &, vector<vector<int>> &);
void showType(vector<vector<int>> &);
void showDetail(Field &, vector<Speaker> &);
void showCommandMenu(Field &, vector<Speaker> &);
void menuLogic(Field &, vector<Speaker> &, vector<vector<int>> &);

#endif // SystemEntrance_H 