#include "Menu.h"
#include <iostream>
#include <iomanip>
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


/* 
设置控制台颜色,其中：
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

// 显示主菜单和分贝分布图
void showMenuAndMap() {
    // 清除分贝分布图区域
    setCursorPosition(0, 10); // 假设分贝分布图从第10行开始
    for (int i = 0; i < fieldLength + 5; ++i) { // 额外清理几行以防止残留
        cout << std::string(100, ' ') << "\n"; // 假设控制台宽度为100字符
    }

    // 重置光标到分贝分布图的起始位置
    setCursorPosition(0, 10);

    // 显示主菜单
    setCursorPosition(0, 0); // 主菜单从顶部开始
    cout << "\n========= 音响分贝模拟系统 =========\n";
    cout << std::left << std::setw(50) << "1. 设置场地大小" ;
    cout << std::left << std::setw(50) << "2. 添加新的音响" << "\n";
    cout << std::left << std::setw(50) << "3. 修改现有音响的参数";
    cout << std::left << std::setw(50) << "4. 删除音响" << "\n";
    cout << "5. 退出程序\n";

    // 显示分贝分布图
    if (fieldWidth > 0 && fieldLength > 0) {
        cout << "\n" << "分贝分布模拟图 (每个#表示一平方米):" << "\n";
        //后续可以在边框处少做标记用于快速定位出某一点的坐标
        //以下是一个暂时的分贝分布图示例，无运算逻辑，具体运算逻辑需要队友实现
        for (int i = 0; i < fieldLength; ++i) {
            for (int j = 0; j < fieldWidth; ++j) {
                int decibel = (i + j) % 100; // 示例分贝值计算
                if (decibel < 30) {
                    setConsoleColor(2);
                    cout << "#";
                } else if (decibel < 60) {
                    setConsoleColor(6);
                    cout << "#";
                } else if (decibel < 90) {
                    setConsoleColor(12);
                    cout << "#";
                } else {
                    setConsoleColor(4);
                    cout << "#";
                }
            }
            setConsoleColor(7); // 恢复默认颜色
            cout << "\n";
        }
        // 后续考虑在图旁边标注音箱的参数

    } else {
        cout << "\n尚未设置场地大小\n";
    }
    cout << "请选择操作: ";
}

// 设置场地大小
void setFieldSize() {
    cout << "请输入场地左右宽度（单位：米）: ";
    cin >> fieldWidth;
    cout << "请输入场地上下宽度（单位：米）: ";
    cin >> fieldLength;


    if (fieldWidth > 0 && fieldLength > 0) {
        cout << "场地大小设置为 " << fieldWidth << " 米 x " << fieldLength << " 米。\n";
        cout << "按任意键继续...\n";
        system("pause > nul"); // 暂停，等待用户按任意键
    } else {
        cout << "输入无效，请重新设置场地大小。\n";
        fieldWidth = 0;
        fieldLength = 0;
    }
}

// 添加新的音响
// 考虑给音箱标序号①到⑩这样，输出在图上直接用序号表示某个音箱
// 缺点是只能同时存在十个音箱
void addSpeaker() {
    Speaker newSpeaker;
    cout << "请输入音响名称: ";
    cin >> newSpeaker.name;
    cout << "请输入音响功率(W): ";
    cin >> newSpeaker.power;
    cout << "请输入音响距离(m): ";
    cin >> newSpeaker.distance;

    cout << "音响添加成功！\n";
}

// 主菜单逻辑
// 后面有时间考虑加一个储存功能，把暂存的方案储存起来可以直接读取
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
                cout << "退出中...\n";
                break;
            default:
                cout << "无效的选择，请重新选择。\n";
        }
    } while (choice != 5);
}