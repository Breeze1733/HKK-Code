#include "ShowMap.h"
#include "MapCalculator.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <cmath>

// 设置控制台文本颜色
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 设置控制台字体为正方形
void setConsoleFontSquare(int fontWidth = 16, int fontHeight = 16) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    wcscpy_s(cfi.FaceName, L"Consolas");
    cfi.dwFontSize.X = fontWidth;
    cfi.dwFontSize.Y = fontHeight;
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// 根据分贝设置颜色
void setColorByDecibel(double dB, bool isSpeaker, const DecibelThreshold& thres) {
    if (isSpeaker) {
        setConsoleColor(13); // 浅紫
        return;
    }
    if (dB >= thres.over) {
        setConsoleColor(12); // 浅红
    } else if (dB >= thres.good) {
        setConsoleColor(14); // 浅黄
    } else if (dB >= thres.low) {
        setConsoleColor(8);  // 灰
    } else {
        setConsoleColor(7);  // 白
    }
}

// 显示声压分布图
void showMap(const MapCalculator& model, const DecibelThreshold& thres, double freqHz, const std::string& freqLabel) {
    int width = model.getWidth();
    int length = model.getLength();
    if (width <= 0 || length <= 0) {
        std::cout << "\n尚未设置场地大小\n";
        return;
    }
    std::cout << "当前频率：" << freqLabel << "\n";
    for (int i = 0; i < length; ++i) {
        int lastColor = -1;
        std::string buffer;
        for (int j = 0; j < width; ++j) {
            bool isSpeaker = model.isSpeakerAt(j, i);
            double dB = model.getDecibelAt(j, i, freqHz);
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
    }
    setConsoleColor(7);
}

void printUserTips(int linesPerPage) {
    std::cout << "【建议】为获得最佳体验，请将命令行窗口最大化（全屏），以显示更多内容。\n";
    std::cout << "【提示】分贝分布图每" << linesPerPage << "行会暂停，需按任意键继续查看下一页。\n";
    std::cout << "输入 exit 后回车可退出，输入任意内容回车可刷新分布图。\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setConsoleFontSquare(16, 16);

    setConsoleColor(14); // 亮黄色
    std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                               【重要提示】                               ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ 本程序将依次展示三个频率下的声压分布图：                                 ║\n";
    std::cout << "║   1. 低频（500Hz）                                                       ║\n";
    std::cout << "║   2. 中频（1kHz）                                                        ║\n";
    std::cout << "║   3. 高频（8kHz）                                                        ║\n";
    std::cout << "║                                                                          ║\n";
    std::cout << "║ 建议：请将命令行窗口调大，并将字体缩小以获得最佳体验。                   ║\n";
    std::cout << "║                                                                          ║\n";
    std::cout << "║ 每按一次任意键切换到下一张图。                                           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
    std::cout << "按任意键开始...\n";
    setConsoleColor(7);
    _getch();
    system("cls");

    DecibelThreshold thres(100.0, 93.0, 85.0);

    MapCalculator model;
    if (!model.loadData("output/data.txt")) {
        std::cout << "【错误】数据加载失败！\n";
        return 1;
    }

    struct FreqInfo { double freq; std::string label; };
    std::vector<FreqInfo> freqs = {
        {500,  "低频（500Hz）"},
        {1000, "中频（1kHz）"},
        {8000, "高频（8kHz）"}
    };

    for (size_t i = 0; i < freqs.size(); ++i) {
        showMap(model, thres, freqs[i].freq, freqs[i].label);
        setConsoleColor(14);
        if (i != freqs.size() - 1) {
            std::cout << "按任意键切换到下一张图...\n";
            _getch();
            system("cls");
        } else {
            std::cout << "按任意键退出\n";
            _getch();
        }
    }
    setConsoleColor(7);
    return 0;
}