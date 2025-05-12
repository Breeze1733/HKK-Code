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
        int x, y, sensitivity, impedance, rmsPower, coverageAngle, mainAxisOrientation;
        if (!(inFile >> x >> y >> sensitivity >> impedance >> rmsPower >> coverageAngle >> mainAxisOrientation))
            return false;
        speakers.emplace_back(x, y, sensitivity, impedance, rmsPower, coverageAngle, mainAxisOrientation);
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
    constexpr double minDbContribution = 5.0;
    constexpr double minDistance = 1.0;

    for (const auto& sp : speakers) {
        double dx = x - sp.getX();
        double dy = y - sp.getY();
        double r = std::max(minDistance, sqrt(dx * dx + dy * dy));

        double rmsPower = sp.getRmsPower();
        if (rmsPower <= 0) continue;

        double pointAngle = atan2(dy, dx) * 180.0 / M_PI;
        if (pointAngle < 0) pointAngle += 360.0;
        double axisAngle = sp.getMainAxisOrientation();
        double delta = pointAngle + axisAngle; // 逆时针为正（数学极坐标）
        while (delta > 180.0) delta -= 360.0;
        while (delta < -180.0) delta += 360.0;
        double absDelta = std::abs(delta);

        // 方向性建模
        double di = 0.0;
        int coverage = sp.getCoverageAngle();
        if (coverage > 0) {
            double theta6dB = coverage / 2.0;
            double n = log(0.5) / log(cos(theta6dB * M_PI / 180.0));
            double thetaRad = absDelta * M_PI / 180.0;
            double mainLobe = pow(std::max(cos(thetaRad), 0.0), n);
            di = (mainLobe > 0) ? 20.0 * log10(mainLobe) : -40.0; // 最小-40dB
        } else {
            di = 0.0;
        }

        double dB_1m = sp.getSensitivity_dBWm() + 10.0 * log10(rmsPower) + di;
        double dB_at_point = dB_1m - 20.0 * log10(r);

        if (dB_at_point < minDbContribution) continue;
        totalPower += pow(10.0, dB_at_point / 10.0);
    }
    if (totalPower < 1e-12) return 0.0;
    return 10.0 * log10(totalPower);
}

double MapCalculator::getDecibelAt(int x, int y, double freqHz) const {
    double totalPower = 0.0;
    constexpr double minDbContribution = 1.0;
    constexpr double minDistance = 1.0;

    for (const auto& sp : speakers) {
        double dx = x - sp.getX();
        double dy = y - sp.getY();
        double r = std::max(minDistance, sqrt(dx * dx + dy * dy));
        double rmsPower = sp.getRmsPower();
        if (rmsPower <= 0) continue;

        int coverage1k = sp.getCoverageAngle();
        bool isOmni = (coverage1k <= 0 || coverage1k >= 360);

        double dB_at_point = 0.0;
        if (isOmni) {
            // 全指向音箱，不考虑主轴、coverage，只考虑距离、功率、频率损失
            double freqLoss = 0.0;
            if (freqHz > 0) {
                // 可查表或简化：空气吸收，单位dB/m
                // 推荐数据：1kHz: 0.003, 4kHz: 0.028, 8kHz: 0.093 dB/m
                double alpha = 0.0;
                if (freqHz >= 8000) alpha = 0.093;
                else if (freqHz >= 4000) alpha = 0.028;
                else if (freqHz >= 1000) alpha = 0.003;
                else alpha = 0.0; // 500Hz及以下忽略空气吸收
                freqLoss = alpha * r;
            }
            double dB_1m = sp.getSensitivity_dBWm() + 10.0 * log10(rmsPower);
            dB_at_point = dB_1m - 20.0 * log10(r) - freqLoss;
        } else {
            // 定向音箱（原有方向性模型部分）
            double coverage = coverage1k;
            if (freqHz > 500) {
                coverage = coverage1k * sqrt(1000.0 / freqHz);
                coverage = std::clamp(coverage, 10.0, 180.0);
            }

            double pointAngle = atan2(dy, dx) * 180.0 / M_PI;
            if (pointAngle < 0) pointAngle += 360.0;
            double axisAngle = sp.getMainAxisOrientation();
            double delta = pointAngle + axisAngle; // 逆时针为正
            while (delta > 180.0) delta -= 360.0;
            while (delta < -180.0) delta += 360.0;
            double absDelta = std::abs(delta);

            double di = 0.0;
            if (freqHz <= 500) {
                di = 0.0;
            } else {
                double theta6dB = coverage / 2.0;
                double n = log(0.5) / log(cos(theta6dB * M_PI / 180.0));
                double thetaRad = absDelta * M_PI / 180.0;
                double mainLobe = pow(std::max(cos(thetaRad), 0.0), n);
                di = (mainLobe > 0) ? 20.0 * log10(mainLobe) : -40.0;
            }

            double dB_1m = sp.getSensitivity_dBWm() + 10.0 * log10(rmsPower) + di;
            dB_at_point = dB_1m - 20.0 * log10(r);
        }
        if (dB_at_point < minDbContribution) continue;
        totalPower += pow(10.0, dB_at_point / 10.0);
    }
    if (totalPower < 1e-12) return 0.0;
    return 10.0 * log10(totalPower);
}