#include "ShowMap.h"
#include "MapCalculator.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <cmath>

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void setConsoleFontSquare(int fontWidth = 16, int fontHeight = 16) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    wcscpy_s(cfi.FaceName, L"Consolas");
    cfi.dwFontSize.X = fontWidth;
    cfi.dwFontSize.Y = fontHeight;
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

void setColorByDecibel(double dB, bool isSpeaker, const DecibelThreshold& thres) {
    if (isSpeaker) {
        setConsoleColor(13);
        return;
    }
    if (dB >= thres.over) {
        setConsoleColor(12);
    } else if (dB >= thres.good) {
        setConsoleColor(14);
    } else if (dB >= thres.low) {
        setConsoleColor(8);
    } else {
        setConsoleColor(7);
    }
}

void showMap(const MapCalculator& model, const DecibelThreshold& thres, int linesPerPage) {
    int width = model.getWidth();
    int length = model.getLength();
    if (width <= 0 || length <= 0) {
        std::cout << "\n尚未设置场地大小\n";
        return;
    }
    int lineCount = 0;
    for (int i = 0; i < length; ++i) {
        int lastColor = -1;
        std::string buffer;
        for (int j = 0; j < width; ++j) {
            bool isSpeaker = model.isSpeakerAt(j, i);
            double dB = model.getDecibelAt(j, i);
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

    DecibelThreshold thres(100.0, 93.0, 85.0);
    int linesPerPage = 500;
    bool firstShow = true;

    MapCalculator model;
    if (!model.loadData("output/data.txt")) {
        std::cout << "数据加载失败！\n";
        return 1;
    }

    while (true) {
        model.loadData("output/data.txt"); // 支持热加载
        showMap(model, thres, linesPerPage);
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