#include "MenuFunction.h"

// 功能1 设置场地大小
void setFieldSize(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    clearAboveLines(9);
    cout << "您正在设置场地大小，这会清空已经摆放好的音响...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    field.setWidth(askQuestion(1,10000, "请输入场地左右宽度(米): "));
    field.setLength(askQuestion(1,10000, "请输入场地上下宽度(米): "));
    speakers.clear();
    storeDataToFile("output/data.txt", field, speakers, type);
    cout << "场地大小设置成功!按任意键继续...";
    system("pause > nul");
}
// 功能2 添加新的音响类型
void addSpeakerType(vector<vector<int>> &type) {
    clearAboveLines(9);
    vector<int> newType(4,0);
    cout << "您正在添加新的音响类型...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int isOriented = askQuestion(1, 2, "请设置音响指向性( 1.全向音响  2.定向音箱 ): ");
    if (isOriented == 2) {
        newType[0] = askQuestion(0, 360, "请输入音响的-6dB覆盖角(范围: 0 ~ 360°): ");
    }
    int isImpedance = askQuestion(1,2,"请选择音响灵敏度的单位( 1. dB/W/m  2. dB/2.83V/m ): ");
    if (isImpedance == 1) {
        newType[1] = askQuestion(0,500, "请输入音响的灵敏度(dB/W/m): ");
        newType[2] = 0;
    } else {
        newType[1] = askQuestion(0,500, "请输入音响的灵敏度(dB/2.83V/m): ");
        newType[2] = askQuestion(1,500, "请输入音响的阻抗(单位:Ω): ");
    }
    newType[3] = askQuestion(0,5000, "请输入音响的RMS功率(单位:W): ");
    type.push_back(newType);
    cout << "音响类型添加成功!按任意键继续...";
    system("pause > nul");
}
// 功能3 删除指定音响类型
void deleteSpeakerType(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    clearAboveLines(9);
    if(type.empty()) {
        cout << "当前没有音响类型，无法删除。按任意键继续...";
        system("pause > nul");
        return;
    }
    int index;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    index = askQuestion(1, type.size(), "请输入要删除的音响类型序号: ");
    type.erase(type.begin() + index - 1);
    for(size_t i = 0; i < speakers.size(); ++i) {
        if (speakers[i].getType() == index) {
            speakers.erase(speakers.begin() + i);
            --i;
        } else if (speakers[i].getType() > index) {
            speakers[i].setType(speakers[i].getType() - 1);
        }
    }
    storeDataToFile("output/data.txt", field, speakers, type);
    cout << "音响类型" << index << "删除成功!按任意键继续...";
    system("pause > nul");
}
// 功能4 摆放音响
void addSpeaker(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    clearAboveLines(9);
    if (type.empty()) {
        cout << "当前没有音响类型，无法添加音响。按任意键继续...";
        system("pause > nul");
        return;
    }
    if (field.getWidth() == 0 || field.getLength() == 0) {
        cout << "请先设置场地大小！按任意键继续...";
        system("pause > nul");
        return;
    }
    Speaker newSpeaker;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    newSpeaker.setType(askQuestion(1, type.size(), "您正在摆放新的音响, 请选择音响类型: "));
    newSpeaker.setCoverageAngle(type[newSpeaker.getType() - 1][0]);
    newSpeaker.setSensitivity(type[newSpeaker.getType() - 1][1]);
    newSpeaker.setImpedance(type[newSpeaker.getType() - 1][2]);
    newSpeaker.setRmsPower(type[newSpeaker.getType() - 1][3]);
    cout << "请输入音响摆放的位置(x,y) " << "(范围: 1 ~ " << field.getWidth() << " , 1 ~ " << field.getLength() << " )\n";
    newSpeaker.setX(askQuestion(0,field.getWidth(), "请输入x坐标: "));
    newSpeaker.setY(askQuestion(0,field.getLength(), "请输入y坐标: "));
    if(newSpeaker.getCoverageAngle()){
        newSpeaker.setMainAxisOrientation(askQuestion(0, 360, "请输入音响的主轴朝向(范围: 0 ~ 360°): "));
    }
    speakers.push_back(newSpeaker);
    storeDataToFile("output/data.txt", field, speakers, type);
    cout << "音响摆放成功!按任意键继续...";
    system("pause > nul");
}
// 功能5 调整音箱位置与朝向
void adjustSpeaker(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    clearAboveLines(9);
    if(speakers.empty()) {
        cout << "当前没有音响，无法修改参数。按任意键继续...";
        return;
    }
    int index = 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    index = askQuestion(1, speakers.size(), "请输入要修改参数的音响序号: ");
    cout << "请输入音响摆放的新位置(x,y) " << "(范围: 1 ~ " << field.getWidth() << " , 1 ~ " << field.getLength() << " )\n";
    speakers[index - 1].setX(askQuestion(0,field.getWidth(), "请输入x坐标: "));
    speakers[index - 1].setY(askQuestion(0,field.getLength(), "请输入y坐标: "));
    if(speakers[index - 1].getCoverageAngle()){
        speakers[index - 1].setMainAxisOrientation(askQuestion(0, 360, "请输入音响的主轴朝向(范围: 0 ~ 360°): "));
    }
    storeDataToFile("output/data.txt", field, speakers, type);
    cout << "第" << index << "号音响参数调整成功!按任意键继续...";
    system("pause > nul");
}
// 功能6 删除音响
void deleteSpeaker(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    clearAboveLines(9);
    if(speakers.empty()) {
        cout << "当前没有音响，无法删除。按任意键继续...";
        return;
    }
    int index;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    index = askQuestion(1, speakers.size(), "请输入要删除的音响序号: ");
    speakers.erase(speakers.begin() + index - 1);
    storeDataToFile("output/data.txt", field, speakers, type);
    cout << "第" << index << "号音响删除成功!按任意键继续...";
    system("pause > nul");
}
// 功能7 打开分贝分布图
void openMap(Field &field, vector<Speaker> &speakers){
    clearAboveLines(9);
    if (speakers.empty()) {
        cout << "当前没有音响，请先添加音响！按任意键继续...";
        system("pause > nul");
        return;
    }
    if (field.getWidth() == 0 || field.getLength() == 0) {
        cout << "请先设置场地大小！按任意键继续...";
        system("pause > nul");
        return;
    }
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
    cout << "正在打开分贝分布图，按任意键继续...";
    system("pause > nul");
    system("start \"\" \"ShowMap.exe\"");
}

// 功能8 保存当前的方案
void storeSolution(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    clearAboveLines(9);
    storeDataToFile("output/solution.txt", field, speakers, type);
    cout << "您方案已保存在“output/solution.txt”中!按任意键继续...";
    system("pause > nul");
}

// 功能9 读取保存的方案
void readSolution(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    clearAboveLines(9);
    string inputPath = "output/solution.txt";
    getDataFromFile(inputPath, field, speakers, type);
    storeDataToFile("output/data.txt", field, speakers, type);
    cout << "按任意键继续...";
    system("pause > nul");
}

// 功能0
void exitProgram() {
    clearAboveLines(9);
    cout << "正在退出程序...";
    if (system("tasklist /FI \"IMAGENAME eq ShowMap.exe\" 2>NUL | find /I \"ShowMap.exe\" >NUL") == 0) {
        system("taskkill /IM ShowMap.exe /F > nul 2>&1");
    }  
    ofstream outFile("output/data.txt", ios::trunc);
    outFile.close();
}

// 选择错误
void invalidChoice() {
    cout << "无效的选择，请重新选择。\n";
    cout << "按任意键继续...";
    system("pause > nul");
}