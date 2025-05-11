#include "MenuFunction.h"

Field field(0, 0);
vector<Speaker> speakers;

// 文件数据维护
void saveDataToFile(const string & outputPath) {
    std::ofstream outFile(outputPath);

    if (!outFile) {
        cout << "无法创建文件: " << outputPath << "\n";
        return;
    }
    outFile << field.getWidth() << " " << field.getLength() << "\n";

    outFile << speakers.size() << "\n";
    for (auto& speaker : speakers) {
        outFile << speaker.getX() << " " << speaker.getY() << " "
                << speaker.getSensitivity() << " "
                << speaker.getImpedance() << " "
                << speaker.getRatedPower() << " "
                << speaker.getCoverageAngle() << " "
                << speaker.getMainAxisOrientation() << "\n";
    }

    outFile.close();
}
void updateData(){
    saveDataToFile("output/data.txt");
    cout << "数据已更新，按任意键继续...\n";
    system("pause > nul");
    clearConsoleBelow(5);
}

// 数据展示与功能菜单
void showMenu() {
    cout << "设定场地大小: " << field.getWidth() << " 米 x " << field.getLength() << " 米\n";
    cout << "音响数量: " << speakers.size() << "\n";
    cout << "音响列表:\n";
    if (speakers.empty()) {
        cout << "当前没有音响。\n";
    } else {
        cout << "┌──────┬────────────┬────────────┬───────┬──────────┐\n";
        cout << "│ 序号 │ 坐标       │ 灵敏度     │ 阻抗  │ 额定功率 │\n";
        cout << "├──────┼────────────┼────────────┼───────┼──────────┤\n";
        for (size_t i = 0; i < speakers.size(); ++i) {
            string location = "(" + to_string(speakers[i].getX()) + ", " + to_string(speakers[i].getY()) + ")";
            string sensitivity, impedance;
            if(speakers[i].getImpedance()) {
                sensitivity = to_string(speakers[i].getSensitivity()) + " V/m   ";
                impedance = to_string(speakers[i].getImpedance()) + "Ω";
            }
            else {
                sensitivity = to_string(speakers[i].getSensitivity()) + " dB/W/m";
                impedance = "   ——";
            }
            string power = to_string(speakers[i].getRatedPower()) + " W";
            cout << "│ " << left << setw(4)  << i + 1       << " │ " 
                         << left << setw(10) << location    << " │ " 
                         << right << setw(10) << sensitivity << " │ "
                         << right << setw(6) << impedance   << " │ "
                         << right << setw(8) << power       << " │\n";
        }
        cout << "└──────┴────────────┴────────────┴───────┴──────────┘\n";
    }
    cout << "功能菜单：" << "\n";
    cout << left << setw(37) << "1. 设置场地大小" << "|   " << "5. 打开分贝分布图" << "\n";
    cout << left << setw(37) << "2. 添加新的音响" << "|   " << "6. 保存当前方案"  << "\n";
    cout << left << setw(40) << "3. 修改现有音响的参数" << "|   " << "7. 读取保存的方案" << "\n";
    cout << left << setw(37) << "4. 删除指定音响" << "|   " << "8. 退出程序"  << "\n";
    cout << "请选择操作: ";
}

// 功能1
void setFieldSize() {
    cout << "您正在设置场地大小...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    field.setWidth(askQuestion(1,10000, "请输入场地左右宽度(米): "));
    field.setLength(askQuestion(1,10000, "请输入场地上下宽度(米): "));
    cout << "场地大小设置成功！\n";
    updateData();
}
// 功能2
void addSpeaker() {
    if (field.getWidth() == 0 || field.getLength() == 0) {
        cout << "请先设置场地大小！\n";
        return;
    }
    Speaker newSpeaker;
    cout << "您正在添加新的音响，这是第" << speakers.size() + 1 << "号音箱...\n";
    cout << "请输入音响位置(x,y) " << "(范围: 0 ~ " << field.getWidth() << " , 0 ~ " << field.getLength() << " )\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    newSpeaker.setX(askQuestion(0,field.getWidth(), "请输入x坐标: "));
    newSpeaker.setY(askQuestion(0,field.getLength(), "请输入y坐标: "));
    int isImpedance = askQuestion(1,2,"请选择音响灵敏度的单位( 1.dB/W/m  2.V/m ): ");
    if (isImpedance == 1) {
        newSpeaker.setImpedance(0);
        newSpeaker.setSensitivity(askQuestion(50,150, "请输入音响的灵敏度(范围: 50 ~ 150)(dB/W/m): "));
    } else {
        newSpeaker.setSensitivity(askQuestion(0,2000, "请输入音响的灵敏度(范围: 0 ~ 200)(V/m): "));
        newSpeaker.setImpedance(askQuestion(1,1000, "请输入音响的阻抗(Ω): "));
    }
    newSpeaker.setRatedPower(askQuestion(0,1000, "请输入音响的额定功率(W): "));
    speakers.push_back(newSpeaker);
    cout << "音响添加成功！\n";
    updateData();
}
// 功能3
void modifySpeaker() {
    if(speakers.empty()) {
        cout << "当前没有音响，无法修改参数。\n";
        return;
    }
    int index = 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    index = askQuestion(1, speakers.size(), "请输入要修改参数的音响序号: ");
    cout << "请输入音响新位置(x,y) " << "(范围: 0 ~ " << field.getWidth() << " , 0 ~ " << field.getLength() << " )\n";
    speakers[index - 1].setX(askQuestion(0,field.getWidth(), "请输入x坐标: "));
    speakers[index - 1].setY(askQuestion(0,field.getLength(), "请输入y坐标: "));
    int isImpedance = askQuestion(1,2,"请选择音响灵敏度的单位( 1.dB/W/m  2.V/m ): ");
    if (isImpedance == 1) {
        speakers[index - 1].setImpedance(0);
        speakers[index - 1].setSensitivity(askQuestion(50,150, "请输入音响的新灵敏度(范围: 50 ~ 150)(dB/W/m): "));
    } else {
        speakers[index - 1].setSensitivity(askQuestion(0,2000, "请输入音响的新灵敏度(范围: 0 ~ 200)(V/m): "));
        speakers[index - 1].setImpedance(askQuestion(1,1000, "请输入音响的新阻抗(Ω): "));
    }
    speakers[index - 1].setRatedPower(askQuestion(0,1000, "请输入音响的新额定功率(W): "));
    cout << "音响参数修改成功！\n";
    updateData();
}
// 功能4
void deleteSpeaker() {
    if(speakers.empty()) {
        cout << "当前没有音响，无法删除。\n";
        return;
    }
    int index;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    index = askQuestion(1, speakers.size(), "请输入要删除的音响序号: ");
    speakers.erase(speakers.begin() + index - 1);
    cout << "音响删除成功！\n";
    updateData();
}
// 功能5
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
    clearConsoleBelow(5);
}
// 功能6
void storeSolution() {
    saveDataToFile("output/solution.txt");
    cout << "您方案已保存在“output/solution.txt”中 按任意键继续...\n";
    system("pause > nul");
    clearConsoleBelow(5);
}
// 功能7
void readSolution() {
    string inputPath = "output/solution.txt";
    ifstream inFile(inputPath);
    if (!inFile) {
        cout << "读取错误，暂无储存方案，按任意键继续...\n";
        system("pause > nul");
        clearConsoleBelow(5);
        return;
    }
    int width, length;
    if (!(inFile >> width >> length)) {
        cout << "数据文件格式错误（场地大小）\n";
        return;
    }
    field.setWidth(width);
    field.setLength(length);
    int speakerCount;
    if (!(inFile >> speakerCount)) {
        cout << "数据文件格式错误（音响数量）\n";
        return;
    }
    speakers.clear();
    for (int i = 0; i < speakerCount; ++i) {
        int x, y, sensitivity, impedance, ratedPower, coverageAngle, mainAxisOrientation;
        if (!(inFile >> x >> y >> sensitivity >> impedance >> ratedPower >> coverageAngle >> mainAxisOrientation)) {
            cout << "数据文件格式错误（音响参数）\n";
            return;
        }
        Speaker speaker(x, y, sensitivity, impedance, ratedPower, coverageAngle, mainAxisOrientation);
        speakers.push_back(speaker);
    }
    inFile.close();
    saveDataToFile("output/data.txt");
    cout << "方案已读取，按任意键继续...\n";
    system("pause > nul");
    clearConsoleBelow(5);
}
// 功能8
void exitProgram() {
    cout << "正在退出程序...\n";
    if (system("tasklist /FI \"IMAGENAME eq ShowMap.exe\" 2>NUL | find /I \"ShowMap.exe\" >NUL") == 0) {
        system("taskkill /IM ShowMap.exe /F > nul 2>&1");
    }  
    ofstream outFile("output/data.txt", ios::trunc);
    outFile.close();
}
// 选择错误
void invalidInput() {
    cout << "无效的选择，请重新选择。\n";
    cout << "按任意键继续...\n";
    system("pause > nul");
    clearConsoleBelow(5);
}