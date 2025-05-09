#include "ShowMap.h"
#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include <sstream>
#include <windows.h>
#include <algorithm> // For std::min

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
        string name;
        int x, y, decibel;
        if (!(inFile >> name >> x >> y >> decibel)) {
            cout << "数据文件格式错误（音响参数）\n";
            break;
        }
        speakers.emplace_back(name, x, y, decibel);
    }

    inFile.close();
}

// 更新分贝分布图数据
void updateMapData(const DecibelThreshold& thres) {
    system("cls");
    showMap(thres);
}

// 显示分贝分布图
void showMap(const DecibelThreshold& thres) {
    int width = field.getWidth();
    int length = field.getLength();
    if (width > 0 && length > 0) {
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
        }
    } else {
        std::cout << "\n尚未设置场地大小\n";
    }
}

// // 根据场地尺寸设置控制台缓冲区和窗口大小，并最大化窗口
// void setupConsoleForField(int width, int length) {
//     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//     COORD bufferSize = {
//         static_cast<SHORT>(std::min(width, 1000)),
//         static_cast<SHORT>(std::min(length, 1000))
//     };
//     SetConsoleScreenBufferSize(hOut, bufferSize);

//     SHORT winRight = static_cast<SHORT>(bufferSize.X - 1);
//     SHORT winBottom = static_cast<SHORT>(bufferSize.Y - 1);
//     SMALL_RECT winRect = {0, 0, winRight, winBottom};
//     SetConsoleWindowInfo(hOut, TRUE, &winRect);

//     ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
// }

int main() {
    SetConsoleOutputCP(CP_UTF8);

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