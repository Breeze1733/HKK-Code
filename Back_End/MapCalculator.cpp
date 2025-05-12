#include "MapCalculator.h"
#include <fstream>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MapCalculator::MapCalculator() : field(0, 0) {}

bool MapCalculator::loadData(const std::string& path) {
    std::ifstream inFile(path);
    if (!inFile) return false;
    int width, length;
    if (!(inFile >> width >> length)) return false;
    field.setWidth(width);
    field.setLength(length);
    int speakerCount;
    if (!(inFile >> speakerCount)) return false;
    speakers.clear();
    for (int i = 0; i < speakerCount; ++i) {
        int x, y, sensitivity, impedance, ratedPower, coverageAngle, mainAxisOrientation;
        if (!(inFile >> x >> y >> sensitivity >> impedance >> ratedPower >> coverageAngle >> mainAxisOrientation))
            return false;
        speakers.emplace_back(x, y, sensitivity, impedance, ratedPower, coverageAngle, mainAxisOrientation);
    }
    return true;
}

bool MapCalculator::isSpeakerAt(int x, int y) const {
    for (const auto& sp : speakers) {
        if (sp.getX() == x && sp.getY() == y) return true;
    }
    return false;
}

double MapCalculator::getDecibelAt(int x, int y) const {
    double totalPower = 0.0;
    constexpr double minDbContribution = 1.0;
    constexpr double powerDivisor = 5.0;
    for (const auto& sp : speakers) {
        double dx = x - sp.getX();
        double dy = y - sp.getY();
        double r = sqrt(dx * dx + dy * dy);
        double avgPower = sp.getRatedPower() / powerDivisor;
        if (avgPower <= 0) continue;
        double di = 0.0;
        int coverage = sp.getCoverageAngle();
        // 8kHz频率下的背后声压级地板
        double minDb = -28.0; // 专业音箱8kHz极坐标主轴背后常见值
        if (coverage > 0) {
            double theta = atan2(-dy, dx) * 180.0 / M_PI;
            if (theta < 0) theta += 360.0;
            double delta = fabs(theta - sp.getMainAxisOrientation());
            if (delta > 180.0) delta = 360.0 - delta;
            double thetaRad = delta * M_PI / 180.0;
            double edge = coverage / 2.0;
            double n = std::log(0.5) / std::log(std::cos(edge * M_PI / 180.0));
            double cosTheta = std::cos(thetaRad);
            if (cosTheta < 1e-6) cosTheta = 1e-6;
            di = n * 10.0 * std::log10(cosTheta);
            if (di < minDb) di = minDb;
        }
        // 全指向时di=0
        double dB_1m = sp.getSensitivity_dBWm() + 10.0 * log10(avgPower) + di;
        double dB_at_point = (r < 1e-6) ? dB_1m : dB_1m - 20.0 * log10(r);
        if (dB_at_point < minDbContribution)
            continue;
        totalPower += pow(10.0, dB_at_point / 10.0);
    }
    if (totalPower < 1e-12) return 0.0;
    return 10.0 * log10(totalPower);
}