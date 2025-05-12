#include "SystemEntrance.h"

void showTitle() {
    cout << "====================== 音响分贝模拟系统 ======================" << "\n";
    // 可以考虑在这里加入作者信息a
    cout << left << setw(30) << "图例与注释:"           << "   (0,0)┌────────────────────→ x\n"   ;
    cout << left << setw(44) << "\033[32mの\033[0m 音量偏小" << "   │       90°\n"          ;
    cout << left << setw(44) << "\033[33mの\033[0m 音量适中" << "   │        ↑ \n"          ;
    cout << left << setw(44) << "\033[31mの\033[0m 音量偏大" << "   │ 180° ← · → 0°(360°)\n" ;
    cout << left << setw(44) << "\033[35mの\033[0m 音响位置" << "   │        ↓ \n"          ;
    cout << left << setw(38) << "每个の表示1平方米"           << " y ↓       270°\n"         ;     
    cout << "==============================================================" << "\n";
}

void showData(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    cout << "设定场地大小: " << field.getWidth() << " 米 x " << field.getLength() << " 米\n";
    cout << "场上音响数量: " << speakers.size() << " 个\n";
    showType(type);
    showDetail(field, speakers);
}

void showType(vector<vector<int>> &type) {
    if (!type.empty()) {
        cout << "┌────────┬────────────┬────────────────┬────────┬────────────┐\n";
        cout << "│ 类型   │ 水平覆盖角 │ 灵敏度         │ 阻抗   │ 额定功率   │\n";
        cout << "├────────┼────────────┼────────────────┼────────┼────────────┤\n";
        for (size_t i = 0; i < type.size(); ++i) {
            string coverageAngle, sensitivity, impedance;
            if (type[i][0] == 0) {
                coverageAngle = "全指向音响";
            } else {
                coverageAngle = to_string(type[i][0]) + "°";
            }
            if (type[i][2]) {
                sensitivity = to_string(type[i][1]) + " dB/2.83V/m";
                impedance = to_string(type[i][2]) + "Ω";
            } else {
                sensitivity = to_string(type[i][1]) + " dB/ W / m ";
                impedance = "    ——";
            }
            string power = to_string(type[i][3]) + " W";
            cout << "│ " << left << setw(8)  << "类型" + to_string(i + 1)  << " │ " 
                         << right << setw(11) << coverageAngle    << " │ " 
                         << right << setw(14) << sensitivity      << " │ "
                         << right << setw(7)  << impedance        << " │ "
                         << right << setw(10) << power            << " │\n";
        }
        cout << "└────────┴────────────┴────────────────┴────────┴────────────┘\n";
    }
}

void showDetail(Field &field, vector<Speaker> &speakers) {
    if (!speakers.empty()) {
        cout << "┌──────┬──────────┬────────────────────┬────────────┐\n";
        cout << "│ 序号 │ 类型     │ 坐标               │ 主轴朝向   │\n";
        cout << "├──────┼──────────┼────────────────────┼────────────┤\n";
        for (size_t i = 0; i < speakers.size(); ++i) {
            string type = "类型" + to_string(speakers[i].getType());
            string location = "(" + to_string(speakers[i].getX()) + ", " + to_string(speakers[i].getY()) + ")";
            string orientation;
            if (speakers[i].getCoverageAngle() == 0) {
                orientation = "全指向音响";
            } else {
                orientation = to_string(speakers[i].getMainAxisOrientation()) + "°";
            }
            cout << "│ " << left  << setw(4)  << i + 1       << " │ " 
                         << left  << setw(10) << type            << " │ " 
                         << left  << setw(18) << location        << " │ "
                         << right << setw(11) << orientation     << " │\n";
        }
        cout << "└──────┴──────────┴────────────────────┴────────────┘\n";
    }
}

void showCommandMenu(Field &field, vector<Speaker> &speakers) {
    cout << "功能菜单：" << "\n";
    cout << "┌──────────────────────────────┬─────────────────────────────┐\n";
    cout << "│ 1. 设置场地大小              │ 6. 删除音响                 │\n";
    cout << "│ 2. 添加新的音响类型          │ 7. 打开分贝分布图           │\n";
    cout << "│ 3. 删除指定音响类型          │ 8. 保存当前的方案           │\n";
    cout << "│ 4. 摆放音响                  │ 9. 读取保存的方案           │\n";
    cout << "│ 5. 调整音箱位置与朝向        │ 0. 退出程序                 │\n";
    cout << "└──────────────────────────────┴─────────────────────────────┘\n";
    cout << "请选择操作: ";
}

void menuLogic(Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    int choice;
    do {
        system("cls");
        showTitle();
        showData(field, speakers, type);
        showCommandMenu(field, speakers);
        string str_choice;
        cin >> str_choice;
        if (str_choice.length() > 1 || str_choice.empty()) {
            choice = -1;
        } else {
            choice = str_choice[0] - '0';
        }
        switch (choice) {
            case 1:
                setFieldSize(field, speakers, type);
                break;
            case 2:
                addSpeakerType(type);
                break;
            case 3:
                deleteSpeakerType(field, speakers, type);
                break;
            case 4:
                addSpeaker(field, speakers, type);
                break;
            case 5:
                adjustSpeaker(field, speakers, type);
                break;
            case 6:
                deleteSpeaker(field, speakers, type);
                break;
            case 7: 
                openMap(field, speakers);
                break;
            case 8:
                storeSolution(field, speakers, type);
                break;
            case 9:
                readSolution(field, speakers, type);
                break;
            case 0:
                exitProgram();
                break;
            default:
                invalidChoice();
        }
    } while (choice != 0);
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    _mkdir("output");
    Field field(0, 0);
    vector<Speaker> speakers;
    vector<vector<int>> type;
    menuLogic(field, speakers, type);
    return 0;
}