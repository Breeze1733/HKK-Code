#ifndef SHOWMAP_H
#define SHOWMAP_H
#include "Speaker.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

void setConsoleColor(int color);
void showMap();
void updateMapData(const std::vector<Speaker>& speakers, int width, int length);

#endif // SHOWMAP_H