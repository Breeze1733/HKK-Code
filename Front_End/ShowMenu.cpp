#include "ShowMenu.h"


Field field(0, 0); // 初始化场地大小
vector<Speaker> speakers; // 存储音响对象的结构

// 显示主菜单
void showMenu() {
    cout << "\n========= 音响分贝模拟系统 =========\n";
    cout << "1. 设置场地大小" << "\n";
    cout << "2. 添加新的音响" << "\n";
    cout << "3. 修改现有音响的参数" << "\n";
    cout << "4. 删除音响" << "\n";
    cout << "5. 打开分贝分布图" << "\n";
    cout << "6. 更新音响分布图" << "\n";
    cout << "7. 退出程序\n";
    cout << "请选择操作: ";
}

// 设置场地大小
// 这里还缺少一些输入检测的逻辑
void setFieldSize() {
    string str1, str2;
    cout << "请输入场地左右宽度（单位：米）: ";
    cin >> str1;
    int width = stoi(str1);
    cout << "请输入场地上下宽度（单位：米）: ";
    cin >> str2;
    int length = stoi(str2);

    if (width > 0 && length > 0) {
        cout << "场地大小设置为 " << width << " 米 x " << length << " 米。\n";
        cout << "按任意键继续...\n";
        system("pause > nul"); // 暂停，等待用户按任意键
    } else {
        cout << "输入无效，请重新设置场地大小。\n";
    }
    
    field.setWidth(width);
    field.setLength(length);
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
    cout << "请输入音响位置(x y): ";
    cin >> x >> y;
    cout << "请输入音响分贝值: ";
    cin >> decibel;
    newSpeaker.setName(name);
    newSpeaker.setX(x);
    newSpeaker.setY(y);
    newSpeaker.setDecibel(decibel);
    speakers.push_back(newSpeaker);
    cout << "音响添加成功！\n";
}

void setData() {
    // 创建输出文件路径
    std::string outputPath = "output/data.txt";
    std::ofstream outFile(outputPath);

    if (!outFile) {
        cout << "无法创建文件: " << outputPath << "\n";
        return;
    }

    // 写入 Field 数据
    outFile << field.getWidth() << " " << field.getLength() << "\n";

    // 写入 Speaker 数据
    outFile << speakers.size() << "\n"; // 先写入音响数量
    for (auto& speaker : speakers) {
        outFile << speaker.getName() << " " << speaker.getX() << " " << speaker.getY() << " " << speaker.getDecibel() << "\n";
    }

    outFile.close();
    cout << "数据已成功写入到 " << outputPath << "\n";
}

void openMap(){
    static bool isMapConsoleOpen = false;
    if (!isMapConsoleOpen) {
        cout << "使用说明：\n";
        cout << "输入“exit”后回车可以关闭分贝分布图。\n";
        cout << "输入其他内容回车可以更新分贝分布图。\n";
        cout << "正在打开分贝分布图，按任意键继续...\n";
        system("pause > nul");
        system("start \"\" \"ShowMap.exe\"");
        isMapConsoleOpen = true;
    } else {
        cout << "分贝分布图已经打开，请检查。\n";
        cout << "是否重置分贝分布图打开状态？\n";
        cout << "1. 是\n";
        cout << "2. 否\n";
        int choice;
        cin >> choice;
        if (choice == 1) {
            isMapConsoleOpen = false;
            system("taskkill /IM ShowMap.exe /F > nul 2>&1");
            cout << "分贝分布图打开状态已重置。\n";
            openMap();
        } else {
            cout << "分贝分布图打开状态未更改。\n";
            cout << "按任意键继续...\n";
            system("pause > nul");
        }
    }
}
// 主菜单逻辑
// 后面有时间考虑加一个储存功能，把暂存的方案储存起来可以直接读取
void menuLogic() {
    int choice;
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
            case 5: 
                openMap();
                break;
            case 6: 
                setData();
                cout << "数据已更新，请在另一工作台按任意键后回车更新分贝分布图\n";
                cout << "按任意键继续...\n";
                system("pause > nul");
                break;
            case 7:
                if (system("tasklist /FI \"IMAGENAME eq ShowMap.exe\" 2>NUL | find /I \"ShowMap.exe\" >NUL") == 0) {
                    system("taskkill /IM ShowMap.exe /F > nul 2>&1");
                }
                cout << "退出中...\n";
                break;
            default:
                cout << "无效的选择，请重新选择。\n";
                cout << "按任意键继续...\n";
                system("pause > nul");
        }
    } while (choice != 7);
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    _mkdir("output");
    menuLogic();
    return 0;
}