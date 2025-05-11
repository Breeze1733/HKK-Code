#include "ShowMap.h"
#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include <sstream>
#include <windows.h>
#include <algorithm>
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>

// 控制台颜色说明：0=黑,1=蓝,2=绿,3=青,4=红,5=紫,6=黄,7=白,8=灰,9=浅蓝,10=浅绿,11=浅青,12=浅红,13=浅紫,14=浅黄,15=亮白

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

// 全局数据
Field field(0, 0);
std::vector<Speaker> speakers;

// 分贝转颜色，isSpeaker为true时用音响颜色
void setColorByDecibel(double dB, bool isSpeaker, const DecibelThreshold& thres) {
    if (isSpeaker) {
        setConsoleColor(13); // 浅紫色
        return;
    }
    if (dB >= thres.over) {
        setConsoleColor(12); // 浅红色
    } else if (dB >= thres.good) {
        setConsoleColor(14); // 浅黄色
    } else if (dB >= thres.low) {
        setConsoleColor(8); // 灰色
    } else {
        setConsoleColor(7); // 白色
    }
}

// 判断(x, y)是否有音响
bool isSpeakerAt(int x, int y) {
    for (const auto& sp : speakers) {
        if (sp.getX() == x && sp.getY() == y) return true;
    }
    return false;
}

// 声音叠加
double sumDecibelAt(int x, int y) {
    double totalPower = 0.0;
    constexpr double minDbContribution = 1.0; // 小于此分贝则不计入
    constexpr double powerDivisor = 5.0;      // 经验值：额定功率/5为平均工作功率

    for (const auto& sp : speakers) {
        double dx = x - sp.getX();
        double dy = y - sp.getY();
        double r = sqrt(dx * dx + dy * dy);

        double avgPower = sp.getRatedPower() / powerDivisor;
        if (avgPower <= 0) continue;

        // 用自动换算后的灵敏度
        double dB_1m = sp.getSensitivity_dBWm() + 10.0 * log10(avgPower);
        double dB_at_point = (r < 1e-6) ? dB_1m : dB_1m - 20.0 * log10(r);

        if (dB_at_point < minDbContribution)
            continue;
        totalPower += pow(10.0, dB_at_point / 10.0);
    }
    if (totalPower < 1e-12) return 0.0;
    return 10.0 * log10(totalPower);
}

// 读取数据
void getData() {
    std::string inputPath = "output/data.txt";
    std::ifstream inFile(inputPath);

    if (!inFile) {
        std::cout << "无法读取文件: " << inputPath << "\n";
        return;
    }

    int width, length;
    if (!(inFile >> width >> length)) {
        std::cout << "数据文件格式错误（场地大小）\n";
        return;
    }
    field.setWidth(width);
    field.setLength(length);

    int speakerCount;
    if (!(inFile >> speakerCount)) {
        std::cout << "数据文件格式错误（音响数量）\n";
        return;
    }
    speakers.clear();
    for (int i = 0; i < speakerCount; ++i) {
        int x, y, sensitivity, impedance, ratedPower;
        if (!(inFile >> x >> y >> sensitivity >> impedance >> ratedPower)) {
            cout << "数据文件格式错误（音响参数）\n";
            return;
        }
        speakers.emplace_back(x, y, sensitivity, impedance, ratedPower);
    }
    inFile.close();
}

// 显示分贝分布图(带分页)
void showMap(const DecibelThreshold& thres, int linesPerPage) {
    int width = field.getWidth();
    int length = field.getLength();
    if (width <= 0 || length <= 0) {
        std::cout << "\n尚未设置场地大小\n";
        return;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int lineCount = 0;

    for (int i = 0; i < length; ++i) {
        int lastColor = -1;
        std::string buffer;
        for (int j = 0; j < width; ++j) {
            bool isSpeaker = isSpeakerAt(j, i);
            double dB = sumDecibelAt(j, i);
            int color = isSpeaker ? 13
                        : (dB >= thres.over ? 12
                        : (dB >= thres.good ? 14
                        : (dB >= thres.low ? 8 : 7)));
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

        if (linesPerPage > 0 && lineCount >= linesPerPage) {
            setConsoleColor(14);
            std::cout << "按任意键继续..." << std::endl;
            _getch();
            system("cls");
            lineCount = 0;
        }
    }
    setConsoleColor(7); // 恢复默认颜色
}

void updateMapData(const DecibelThreshold& thres, int linesPerPage) {
    showMap(thres, linesPerPage);
}

// 用户提示，仅首次显示
void printUserTips(int linesPerPage) {
    std::cout << "【建议】为获得最佳体验，请将命令行窗口最大化（全屏），以显示更多内容。\n";
    std::cout << "【提示】分贝分布图每" << linesPerPage << "行会暂停，需按任意键继续查看下一页。\n";
    std::cout << "输入 exit 后回车可退出，输入任意内容回车可刷新分布图。\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setConsoleFontSquare(16, 16);

    DecibelThreshold thres(100.0, 93.0, 85.0);
    int linesPerPage = 500;
    bool firstShow = true;

    while (true) {
        getData();
        updateMapData(thres, linesPerPage);
        if (firstShow) {
            printUserTips(linesPerPage);
            firstShow = false;
        }
        std::cout << "请输入指令（exit 或其他）：";
        std::string str;
        std::cin >> str;
        if (str == "exit") break;
    }
    return 0;
}