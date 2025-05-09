#include "ShowMap.h"
#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
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
        setConsoleColor(4); // 红色，音响本体
        return;
    }
    if (dB >= thres.over) {
        setConsoleColor(12); // 浅红色，过大
    } else if (dB >= thres.good) {
        setConsoleColor(6); // 黄色，适合
    } else if (dB >= thres.low) {
        setConsoleColor(2); // 绿色，过小
    } else {
        setConsoleColor(8); // 灰色，极低
    }
}

// 判断(x, y)是否有音响
bool isSpeakerAt(int x, int y) {
    for (auto& sp : speakers) {
        if (sp.getX() == x && sp.getY() == y) return true;
    }
    return false;
}

// 声音叠加：能量相加，最后转回分贝
double sumDecibelAt(int x, int y) {
    double totalPower = 0.0;
    for (auto& sp : speakers) {
        double dx = x - sp.getX();
        double dy = y - sp.getY();
        double r = sqrt(dx * dx + dy * dy);
        double dB = sp.getDecibel();
        if (r < 1e-3) {
            totalPower += pow(10.0, dB / 10.0);
        } else {
            double dB_at_r = dB - 20.0 * log10(r);
            totalPower += pow(10.0, dB_at_r / 10.0);
        }
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
    inFile >> width >> length;
    field.setWidth(width);
    field.setLength(length);

    // 读取 Speaker 数据
    int speakerCount;
    inFile >> speakerCount;
    speakers.clear();
    for (int i = 0; i < speakerCount; ++i) {
        string name;
        int x, y, decibel;
        inFile >> name >> x >> y >> decibel;
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
            for (int j = 0; j < width; ++j) {
                bool isSpeaker = isSpeakerAt(j, i);
                double dB = sumDecibelAt(j, i);
                setColorByDecibel(dB, isSpeaker, thres);
                cout << "の";
            }
            setConsoleColor(7); // 恢复默认颜色
            cout << "\n";
        }
    } else {
        cout << "\n尚未设置场地大小\n";
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    // 阈值可在此处自定义
    DecibelThreshold thres(120.0, 100.0, 80.0);
    while (true) {
        getData();
        updateMapData(thres);
        string str;
        cin >> str;
        if (str == "exit") {
            break;
        }
    }
    return 0;
}