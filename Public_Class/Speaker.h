#ifndef SPEAKER_H
#define SPEAKER_H

#include <cmath>

class Speaker {
private:
    int x;
    int y;
    int sensitivity;         // 灵敏度，整数，单位dB/W/m或dB/2.83V/m
    int impedance;           // 阻抗，整数，单位Ω，0表示dB/W/m
    int rmsPower;            // RMS功率，整数，单位W
    int coverageAngle;       // 覆盖角，整数，单位度，0为全指向
    int mainAxisOrientation; // 主轴朝向，整数，单位度
    int type;                // 音响类型，整数，1表示音响类型1，2表示音响类型2
public:
    Speaker()
        : x(0), y(0), sensitivity(0), impedance(0), rmsPower(0), coverageAngle(0), mainAxisOrientation(0), type(0) {}

    Speaker(int xPos, int yPos, int sens, int imp, int rms, int angle, int orientation)
        : x(xPos), y(yPos), sensitivity(sens), impedance(imp), rmsPower(rms),
          coverageAngle(angle), mainAxisOrientation(orientation), type(0) {}

    Speaker(int xPos, int yPos, int sens, int imp, int rms, int angle, int orientation, int type)
        : x(xPos), y(yPos), sensitivity(sens), impedance(imp), rmsPower(rms),
          coverageAngle(angle), mainAxisOrientation(orientation), type(type) {}
    
    // 坐标
    int getX() const { return x; }
    void setX(int xPos) { x = xPos; }
    int getY() const { return y; }
    void setY(int yPos) { y = yPos; }

    // 灵敏度
    int getSensitivity() const { return sensitivity; }
    void setSensitivity(int sens) { sensitivity = sens; }

    // 阻抗
    int getImpedance() const { return impedance; }
    void setImpedance(int imp) { impedance = imp; }

    // RMS功率
    int getRmsPower() const { return rmsPower; }
    void setRmsPower(int rms) { rmsPower = rms; }

    // 覆盖角
    int getCoverageAngle() const { return coverageAngle; }
    void setCoverageAngle(int angle) { coverageAngle = angle; }

    // 主轴朝向
    int getMainAxisOrientation() const { return mainAxisOrientation; }
    void setMainAxisOrientation(int orientation) { mainAxisOrientation = orientation; }

    // 获取以dB/W/m为基准的灵敏度（返回double，因有对数运算）
    double getSensitivity_dBWm() const {
        if (impedance == 0) {
            // 已是 dB/W/m
            return static_cast<double>(sensitivity);
        } else {
            // dB/2.83V/m 转 dB/W/m
            // dB(W/m) = dB(2.83V/m) - 10*log10(8/Z)
            return static_cast<double>(sensitivity) - 10.0 * std::log10(8.0 / impedance);
        }
    }

    // 音响类型
    int getType() const { return type; }
    void setType(int t) { type = t; }
};

#endif // SPEAKER_H