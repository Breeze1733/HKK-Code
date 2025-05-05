#include "ShowMenu.h"
#include "ShowMap.h"
#include "Speaker.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <windows.h> 
using std::cout;
using std::cin;
using std::string;
using std::vector;


// 全局变量
vector<Speaker> speakers;
int fieldWidth = 0;  // 场地宽度
int fieldLength = 0; // 场地长度

// 显示主菜单
void showMenu() {
    cout << "\n========= 音响分贝模拟系统 =========\n";
    cout << "1. 设置场地大小" << "\n";
    cout << "2. 添加新的音响" << "\n";
    cout << "3. 修改现有音响的参数" << "\n";
    cout << "4. 删除音响" << "\n";
    cout << "5. 更新分贝分布图" << "\n";
    cout << "6. 退出程序\n";
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
    string name;
    int x, y, decibel;
    cout << "请输入音响名称: ";
    cin >> name;
    cout << "音响添加成功！\n";
}

// 主菜单逻辑
// 后面有时间考虑加一个储存功能，把暂存的方案储存起来可以直接读取
void menuLogic() {
    int choice;
    static bool isMapConsoleOpen = false; // 跟踪子控制台是否已打开
    static HANDLE hMapConsole = NULL;    // 子控制台句柄

    do {
        system("cls");
        showMenu();
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
            case 5: {
                // 更新分贝分布图数据
                updateMapData(speakers, fieldWidth, fieldLength);
                if (!isMapConsoleOpen) {
                    // 启动第二个控制台
                    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
                    PROCESS_INFORMATION pi;
                    if (CreateProcessW(
                            NULL,
                            (LPWSTR)L"ShowMap.exe", // 使用宽字符字符串
                            NULL,
                            NULL,
                            FALSE,
                            CREATE_NEW_CONSOLE,
                            NULL,
                            NULL,
                            &si,
                            &pi)) {
                        hMapConsole = pi.hProcess;
                        CloseHandle(pi.hThread);
                        isMapConsoleOpen = true;
                    } else {
                        cout << "无法启动分贝分布图显示窗口。\n";
                    }
                } else {
                    // 如果控制台已打开，直接更新数据
                    cout << "分贝分布图已更新。\n";
                }
                break;
            }
            case 6:
                cout << "退出中...\n";
                if (isMapConsoleOpen && hMapConsole) {
                    TerminateProcess(hMapConsole, 0); // 关闭子控制台
                    CloseHandle(hMapConsole);
                }
                break;
            default:
                cout << "无效的选择，请重新选择。\n";
        }
    } while (choice != 6);
}