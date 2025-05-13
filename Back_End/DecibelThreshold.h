#ifndef DECIBEL_THRESHOLD_H
#define DECIBEL_THRESHOLD_H

/**
 * @brief 分贝阈值结构体，封装分贝区间
 */
struct DecibelThreshold {
    double over;
    double good;
    double low;
    DecibelThreshold(double o = 100.0, double g = 85.0, double l = 70.0)
        : over(o), good(g), low(l) {}
};

#endif // DECIBEL_THRESHOLD_H