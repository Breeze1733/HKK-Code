#include "ShowMenu.h"

Field field(0, 0); // 初始化场地大小
vector<Speaker> speakers; // 存储音响对象的结构

void clearConsoleBelow(int linesToKeep) {
    // 将光标移动到指定行之后
    cout << "\033[" << linesToKeep + 1 << ";1H"; // 移动光标到第 linesToKeep+1 行
    // 清除从光标到屏幕底部的内容
    cout << "\033[J";
}

void clearAboveLines(int linesToClear) {
    for (int i = 0; i < linesToClear; ++i) {
        // 将光标移动到上一行的开头
        cout << "\033[F";
        // 清除当前行内容
        cout << "\033[2K";
    }
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
        outFile << speaker.getX() << " " << speaker.getY() << " " << speaker.getDecibel() << "\n";
    }

    outFile.close();
}
void updateData(){
    setData();
    cout << "数据已更新，在另一工作台输入任意内容后回车，更新分布图\n";
    cout << "按任意键继续...\n";
    system("pause > nul");
    clearConsoleBelow(5);
}

void showTitle() {
    cout << "================== 音响分贝模拟系统 ==================" << "\n";
    cout << "图例：\n";
    cout << "\033[32mの\033[0m音量偏小    " << "\033[33mの\033[0m音量适中    " << 
            "\033[35mの\033[0m音量偏大    " << "\033[31mの\033[0m音响位置    " << "\n";
    cout << "模拟分布图中，每一个“の”表示一平方米，左上角为坐标原点" << "\n";
    cout << endl;
}

void showData() {
    cout << "设定场地大小: " << field.getWidth() << " 米 x " << field.getLength() << " 米\n";
    cout << "音响数量: " << speakers.size() << "\n";
    cout << "音响列表:\n";

    if (speakers.empty()) {
        cout << "当前没有音响。\n";
    } else {
        // 表头
        cout << "┌──────────┬──────────────────────┬──────────────┐\n";
        cout << "│ 序号     │ 坐标                 │ 平均分贝值   │\n";
        cout << "├──────────┼──────────────────────┼──────────────┤\n";

        // 表格内容
        for (size_t i = 0; i < speakers.size(); ++i) {
            string location = "(" + to_string(speakers[i].getX()) + ", " + to_string(speakers[i].getY()) + ")";
            cout << "│ " << left << setw(8) << i + 1 << " │ " 
                 << left << setw(20) << location << " │ " 
                 << left << setw(12) << speakers[i].getDecibel() << " │\n";
        }

        // 表尾
        cout << "└──────────┴──────────────────────┴──────────────┘\n";
    }
}

// 显示主菜单
void showCommandMenu() {
    showData();
    cout << "功能菜单：" << "\n";
    cout << left << setw(37) << "1. 设置场地大小" << "|   " << "4. 删除指定音响" << "\n";
    cout << left << setw(37) << "2. 添加新的音响" << "|   " << "5. 打开分贝分布图"  << "\n";
    cout << left << setw(40) << "3. 修改现有音响的参数" << "|   " << "6. 退出程序" << "\n";
    cout << "请选择操作: ";
}

int getInput(const int &min_value, const int &max_value) {
    string input;
    //
    getline(cin, input);
    if (input.length() > 9 || input.empty()) {
        return -1;
    }
    for (char c : input) {
        if (c < '0' || c > '9') {
            return -1;
        }
    }
    int value = stoi(input);
    if(value > max_value || value < min_value) {
        return -1;
    }
    return value;
}

int askQuestion(const int &min_value, const int &max_value, const string& question) {
    int ans = -1;
    while(true){
        cout << question;
        ans = getInput(min_value, max_value);
        if (ans == -1) {
            cout << "输入无效，请重新输入合法的正整数，按任意键继续...\n";
            system("pause > nul");
            clearAboveLines(2);
            continue;
        }
        else {
            return ans;
        }
    }
}

// 设置场地大小
void setFieldSize() {
    cout << "您正在设置场地大小...\n";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    field.setWidth(askQuestion(1,10000, "请输入场地左右宽度(米): "));
    field.setLength(askQuestion(1,10000, "请输入场地上下宽度(米): "));
    cout << "场地大小设置成功！\n";
}

// 添加新的音响
void addSpeaker() {
    if (field.getWidth() == 0 || field.getLength() == 0) {
        cout << "请先设置场地大小！\n";
        return;
    }
    Speaker newSpeaker;
    int x, y, decibel;
    cout << "您正在添加新的音响，这是第" << speakers.size() + 1 << "号音箱...\n";
    cout << "请输入音响位置(x,y) " << "(范围: 0 ~ " << field.getWidth() << " , 0 ~ " << field.getLength() << " )\n";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    newSpeaker.setX(askQuestion(0,field.getWidth(), "请输入音响位置x坐标: "));
    newSpeaker.setY(askQuestion(0,field.getLength(), "请输入音响位置y坐标: "));
    newSpeaker.setDecibel(askQuestion(0,200, "请输入音响输出的平均分贝值: "));
    speakers.push_back(newSpeaker);
    cout << "音响添加成功！\n";
}

// 修改音响参数
void modifySpeaker() {
    if(speakers.empty()) {
        cout << "当前没有音响，无法修改参数。\n";
        return;
    }
    int index = 0;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    index = askQuestion(1, speakers.size(), "请输入要修改参数的音响序号: ");
    cout << "请输入音响位置(x,y) " << "(范围: 0 ~ " << field.getWidth() << " , 0 ~ " << field.getLength() << " )\n";
    speakers[index - 1].setX(askQuestion(0,field.getWidth(), "请输入音响位置x坐标: "));
    speakers[index - 1].setY(askQuestion(0,field.getLength(), "请输入音响位置y坐标: "));
    speakers[index - 1].setDecibel(askQuestion(0,200, "请输入音响输出的平均分贝值: "));
    cout << "音响参数修改成功！\n";
}

// 删除音响
void deleteSpeaker() {
    if(speakers.empty()) {
        cout << "当前没有音响，无法删除。\n";
        return;
    }
    int index;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    index = askQuestion(1, speakers.size(), "请输入要删除的音响序号: ");
    speakers.erase(speakers.begin() + index - 1);
    cout << "音响删除成功！\n";
}

void openMap(){
    static bool isMapConsoleOpen = false;
    if (!isMapConsoleOpen) {
        isMapConsoleOpen = true;
    }
    else {
        system("taskkill /IM ShowMap.exe /F > nul 2>&1");   
    }
    cout << "使用说明：\n";
    cout << "输入“exit”后回车可以关闭分贝分布图。\n";
    cout << "输入任意其他内容回车可以更新分贝分布图。\n";
    cout << "正在打开分贝分布图，按任意键继续...\n";
    system("pause > nul");
    system("start \"\" \"ShowMap.exe\"");
}

// 主菜单逻辑
// 后面有时间考虑加一个储存功能，把暂存的方案储存起来可以直接读取
void menuLogic() {
    showTitle();
    int choice;
    do {
        showCommandMenu();
        string str_choice;
        cin >> str_choice;
        if (str_choice.length() > 1 || str_choice.empty()) {
            choice = -1;
        } else {
            choice = str_choice[0] - '0';
        }
        switch (choice) {
            case 1:
                setFieldSize();
                updateData();
                break;
            case 2:
                addSpeaker();
                updateData();
                break;
            case 3:
                modifySpeaker();
                updateData();
                break;
            case 4:
                deleteSpeaker();
                updateData();
                break;
            case 5: 
                setData();
                openMap();
                clearConsoleBelow(5);
                break;
            case 6:
                if (system("tasklist /FI \"IMAGENAME eq ShowMap.exe\" 2>NUL | find /I \"ShowMap.exe\" >NUL") == 0) {
                    system("taskkill /IM ShowMap.exe /F > nul 2>&1");
                }
                cout << "退出中...\n";
                break;
            default:
                cout << "无效的选择，请重新选择。\n";
                cout << "按任意键继续...\n";
                system("pause > nul");
                clearConsoleBelow(5);
        }
    } while (choice != 6);
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    _mkdir("output");
    menuLogic();
    return 0;
}