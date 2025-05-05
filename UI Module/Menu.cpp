#include "Menu.h"
#include <iostream>
#include <vector>
#include <string>
#include <windows.h> // 用于设置控制台颜色
using std::cout;
using std::cin;
using std::string;
using std::vector;

struct Speaker {
    string name;
    float power; // 功率
    float distance; // 距离
    float criticalDistance; // 临界距离
};

// 全局变量
vector<Speaker> speakers;
int fieldWidth = 0;  // 场地宽度
int fieldLength = 0; // 场地长度

// 设置控制台颜色
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 显示主菜单和分贝分布图
void showMenuAndMap() {
    // 显示菜单
    cout << "\n===== 音响分贝模拟系统 =====\n";
    cout << "1. 设置场地大小\n";
    cout << "2. 添加新的音响\n";
    cout << "3. 修改现有音响的参数\n";
    cout << "4. 删除音响\n";
    cout << "5. 退出程序\n";
    cout << "请选择操作: ";

    // 显示分贝分布图
    if (fieldWidth > 0 && fieldLength > 0) {
        cout << "\n分贝分布图 (场地大小: " << fieldWidth << " 米 x " << fieldLength << " 米):\n";
        for (int i = 0; i < fieldLength; ++i) {
            for (int j = 0; j < fieldWidth; ++j) {
                int decibel = (i + j) % 100; // 示例分贝值计算
                if (decibel < 30) {
                    setConsoleColor(2); // 绿色
                    cout << " ";
                } else if (decibel < 60) {
                    setConsoleColor(6); // 黄色
                    cout << ".";
                } else if (decibel < 90) {
                    setConsoleColor(4); // 红色
                    cout << "*";
                } else {
                    setConsoleColor(12); // 深红色
                    cout << "#";
                }
            }
            setConsoleColor(7); // 恢复默认颜色
            cout << "\n";
        }
    } else {
        cout << "\n尚未设置场地大小  无法显示分贝分布图。\n";
    }
}

// 设置场地大小
void setFieldSize() {
    cout << "请输入场地宽度（单位：米）: ";
    cin >> fieldWidth;
    cout << "请输入场地长度（单位：米）: ";
    cin >> fieldLength;

    if (fieldWidth > 0 && fieldLength > 0) {
        cout << "场地大小设置为 " << fieldWidth << " 米 x " << fieldLength << " 米。\n";
    } else {
        cout << "输入无效，请重新设置场地大小。\n";
        fieldWidth = 0;
        fieldLength = 0;
    }
}

// 添加新的音响
void addSpeaker() {
    Speaker newSpeaker;
    cout << "请输入音响名称: ";
    cin >> newSpeaker.name;
    cout << "请输入音响功率(W): ";
    cin >> newSpeaker.power;
    cout << "请输入音响距离(m): ";
    cin >> newSpeaker.distance;

    // 简单计算临界距离（示例公式）
    newSpeaker.criticalDistance = newSpeaker.power / (4 * 3.14 * newSpeaker.distance * newSpeaker.distance);
    speakers.push_back(newSpeaker);

    cout << "音响添加成功！\n";
}

// 主菜单逻辑
void menuLogic() {
    int choice;
    do {
        showMenuAndMap();
        cin >> choice;

        switch (choice) {
            case 1:
                setFieldSize();
                break;
            case 2:
                addSpeaker();
                break;
            case 3:
                cout << "修改音响功能尚未实现。\n";
                break;
            case 4:
                cout << "删除音响功能尚未实现。\n";
                break;
            case 5:
                cout << "退出程序。\n";
                break;
            default:
                cout << "无效的选择，请重新选择。\n";
        }
    } while (choice != 5);
}