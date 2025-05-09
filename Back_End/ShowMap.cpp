#include "../Public_Class/Field.h"
#include "../Public_Class/Speaker.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <windows.h> 
#include <fstream> // 添加文件操作头文件
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


// 需要迁移的数据
Field field(0, 0); // 初始化场地大小
vector<Speaker> speakers; // 存储音响对象的结构

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

void showMap();

// 更新分贝分布图数据
void updateMapData() {
    system("cls");
    showMap();
}

// 显示分贝分布图
void showMap() {
    int width = field.getWidth();
    int length = field.getLength();
    if (width > 0 && length > 0) {
        cout << "\n分贝分布模拟图 (每个#表示一平方米):\n";
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < width; ++j) {
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

int main(){
    SetConsoleOutputCP(CP_UTF8);
    while(true){
        getData();
        updateMapData();
        string str;
        cin>>str;
        if(str=="exit"){
            break;
        }
    }
    return 0;
}