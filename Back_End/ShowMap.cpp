#include "ShowMap.h"
#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include <sstream>
#include <windows.h>
#include <algorithm> // For std::min
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>

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

// 设置控制台字体宽高比为正方形
void setConsoleFontSquare(int fontWidth = 16, int fontHeight = 16) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    wcscpy_s(cfi.FaceName, L"Consolas"); // 推荐等宽字体
    cfi.dwFontSize.X = fontWidth;
    cfi.dwFontSize.Y = fontHeight;
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// 需要迁移的数据
Field field(0, 0); // 初始化场地大小
vector<Speaker> speakers; // 存储音响对象的结构

// 分贝转颜色，isSpeaker为true时用音响颜色
void setColorByDecibel(double dB, bool isSpeaker, const DecibelThreshold& thres) {
    if (isSpeaker) {
        setConsoleColor(13); // 浅紫色，音响本体（更亮眼）
        return;
    }
    if (dB >= thres.over) {
        setConsoleColor(12); // 浅红色，过大
    } else if (dB >= thres.good) {
        setConsoleColor(14); // 浅黄色，适合
    } else if (dB >= thres.low) {
        setConsoleColor(8); // 灰色，过小
    } else {
        setConsoleColor(7); // 白色，极低
    }
}

// 判断(x, y)是否有音响
bool isSpeakerAt(int x, int y) {
    for (const auto& sp : speakers) {
        if (sp.getX() == x && sp.getY() == y) return true;
    }
    return false;
}

// 声音叠加：能量相加，最后转回分贝
double sumDecibelAt(int x, int y) {
    double totalPower = 0.0;
    static const double MIN_DB_CONTRIBUTION = 1.0;
    for (const auto& sp : speakers) {
        double dx = x - sp.getX();
        double dy = y - sp.getY();
        double r = sqrt(dx * dx + dy * dy);
        double dB = sp.getDecibel();
        double dB_at_r = (r < 1e-3) ? dB : dB - 20.0 * log10(r);
        if (dB_at_r < MIN_DB_CONTRIBUTION) continue; // 贡献极小可忽略
        totalPower += pow(10.0, dB_at_r / 10.0);
    }
    if (totalPower < 1e-6) return 0.0;
    return 10.0 * log10(totalPower);
}

void getData() {
    // 创建输入文件路径
    std::string inputPath = "output/data.txt";
    std::ifstream inFile(inputPath);

    if (!inFile) {
        cout << "无法读取文件: " << inputPath << "\n";
        return;
    }

    // 读取 Field 数据
    int width, length;
    if (!(inFile >> width >> length)) {
        cout << "数据文件格式错误（场地大小）\n";
        return;
    }
    field.setWidth(width);
    field.setLength(length);

    // 读取 Speaker 数据
    int speakerCount;
    if (!(inFile >> speakerCount)) {
        cout << "数据文件格式错误（音响数量）\n";
        return;
    }
    speakers.clear();
    for (int i = 0; i < speakerCount; ++i) {
        int x, y, decibel;
        inFile >> x >> y >> decibel;
        if (inFile.fail()) {
            cout << "数据文件格式错误（音响参数）\n";
            return;
        }
        speakers.emplace_back(x, y, decibel);
    }

    inFile.close();
}

// 更新分贝分布图数据
void updateMapData(const DecibelThreshold& thres) {
    // 不再清屏，直接输出
    showMap(thres);
}

// 显示分贝分布图
void showMap(const DecibelThreshold& thres) {
    int width = field.getWidth();
    int length = field.getLength();
    if (width > 0 && length > 0) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        int winLines = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        int linesPerPage = winLines - 2; // 留2行用于提示

        int lineCount = 0;

        for (int i = 0; i < length; ++i) {
            int lastColor = -1;
            std::string buffer;
            for (int j = 0; j < width; ++j) {
                bool isSpeaker = isSpeakerAt(j, i);
                double dB = sumDecibelAt(j, i);
                int color;
                if (isSpeaker) {
                    color = 13;
                } else if (dB >= thres.over) {
                    color = 12;
                } else if (dB >= thres.good) {
                    color = 14;
                } else if (dB >= thres.low) {
                    color = 8;
                } else {
                    color = 7;
                }
                if (color != lastColor && !buffer.empty()) {
                    setConsoleColor(lastColor);
                    std::cout << buffer;
                    buffer.clear();
                }
                buffer += "の";
                lastColor = color;
            }
            if (!buffer.empty()) {
                setConsoleColor(lastColor);
                std::cout << buffer;
            }
            setConsoleColor(7);
            std::cout << "\n";
            ++lineCount;

            // 分页判断
            if (lineCount >= linesPerPage) {
                setConsoleColor(14);
                std::cout << "按任意键继续..." << std::endl;
                _getch();
                system("cls");
                lineCount = 0;
            }
        }
    } else {
        std::cout << "\n尚未设置场地大小\n";
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setConsoleFontSquare(16, 16); // 新增：设置字符为正方形

    // 推荐阈值
    DecibelThreshold thres(100.0, 93.0, 85.0);
    cout << "输入 exit 后回车可退出，输入任意内容回车可刷新分布图。\n";
    while (true) {
        getData();

        // // 读取场地尺寸并设置控制台
        // int width = field.getWidth();
        // int length = field.getLength();
        // setupConsoleForField(width, length);

        updateMapData(thres);

        cout << "请输入指令（exit 或其他）：";
        string str;
        cin >> str;
        if (str == "exit") {
            break;
        }
    }
    return 0;
}