#ifndef MAP_CALCULATOR_H
#define MAP_CALCULATOR_H

#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include "DecibelThreshold.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>


/**
 * @brief 声压分布计算核心类
 */
class MapCalculator {
private:
    Field field;
    std::vector<Speaker> speakers;
public:
    MapCalculator();
    bool loadData(const std::string& path);
    double getDecibelAt(int x, int y) const;
    double getDecibelAt(int x, int y, double freqHz) const;
    bool isSpeakerAt(int x, int y) const;
    int getWidth() const { return field.getWidth(); }
    int getLength() const { return field.getLength(); }
};

#endif // MAP_CALCULATOR_H