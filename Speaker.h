#ifndef SPEAKER_H
#define SPEAKER_H

#include <string>
using std::string;

class Speaker {
    private:
        string name; // 音响名称
        int x;       // 音响在场地中的x坐标
        int y;       // 音响在场地中的y坐标
        int decibel; // 音响的分贝值
    public:
        Speaker() : name(""), x(0), y(0), decibel(0) {} // 默认构造函数
        Speaker(string n, int xPos, int yPos, int db) : name(n), x(xPos), y(yPos), decibel(db) {} // 带参数的构造函数
        string getName() { return name; } // 获取音响名称
        void setName(string n) { name = n; } // 设置音响名称
        int getX() { return x; } // 获取x坐标
        void setX(int xPos) { x = xPos; } // 设置x坐标
        int getY() { return y; } // 获取y坐标
        void setY(int yPos) { y = yPos; } // 设置y坐标
        int getDecibel() { return decibel; } // 获取分贝值
        void setDecibel(int db) { decibel = db; } // 设置分贝值
};

#endif // SPEAKER_H