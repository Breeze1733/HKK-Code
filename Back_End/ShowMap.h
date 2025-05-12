#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include "MapCalculator.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <windows.h>
#include <fstream>
using std::cout;
using std::cin;
using std::string;
using std::vector;

/**
 * @brief 分贝阈值结构体，封装分贝区间
 */
struct DecibelThreshold {
    double over;
    double good;
    double low;
    DecibelThreshold(double o = 100.0, double g = 80.0, double l = 65.0)
        : over(o), good(g), low(l) {}
};

void setConsoleColor(int);
void setConsoleFontSquare(int, int);
void getData();
void showMap(const MapCalculator&, const DecibelThreshold&, int linesPerPage);
void showMap(const MapCalculator&, const DecibelThreshold&, int linesPerPage, double freqHz, const std::string& freqLabel);
void updateMapData(const DecibelThreshold& thres);
void setColorByDecibel(double dB, bool isSpeaker, const DecibelThreshold& thres);
bool isSpeakerAt(int x, int y);
double sumDecibelAt(int x, int y);
void printUserTips(int linesPerPage);

