#include "ShowMap.h"
#include "ShowMenu.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <windows.h> 
using std::cout;
using std::cin;
using std::string;
using std::vector;

/* 
关于控制台颜色,其中：
0 = 黑色       8 = 灰色
1 = 蓝色       9 = 浅蓝色
2 = 绿色       10 = 浅绿色
3 = 青色       11 = 浅青色
4 = 红色       12 = 浅红色
5 = 紫色       13 = 浅紫色
6 = 黄色       14 = 浅黄色
7 = 白色       15 = 亮白色
*/
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 辅助函数：设置光标位置
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, position);
}

// 需要迁移的数据

extern vector<Speaker> speakers;
extern int fieldWidth;
extern int fieldLength;

// 相关运算接口
void changeColor(int x,int y){
    int decibel = (x + y) % 100;
    if (decibel < 30) {
        setConsoleColor(2);
    } else if (decibel < 60) {
        setConsoleColor(6);
    } else if (decibel < 90) {
        setConsoleColor(12);
    } else {
        setConsoleColor(4);
    }
}

// 全局变量
static vector<Speaker> mapSpeakers;
static int mapFieldWidth = 0;
static int mapFieldLength = 0;

// 更新分贝分布图数据
void updateMapData(const vector<Speaker>& speakers, int width, int length) {
    mapSpeakers = speakers;
    mapFieldWidth = width;
    mapFieldLength = length;

    // 清空控制台并重新绘制分贝分布图
    system("cls");
    showMap();
}

// 显示分贝分布图
void showMap() {
    if (mapFieldWidth > 0 && mapFieldLength > 0) {
        cout << "\n分贝分布模拟图 (每个#表示一平方米):\n";
        for (int i = 0; i < mapFieldLength; ++i) {
            for (int j = 0; j < mapFieldWidth; ++j) {
                // 简单的颜色逻辑，后续可根据 Speaker 数据调整
                setConsoleColor((i + j) % 15 + 1);
                cout << "#";
            }
            setConsoleColor(7); // 恢复默认颜色
            cout << "\n";
        }
    } else {
        cout << "\n尚未设置场地大小\n";
    }
}