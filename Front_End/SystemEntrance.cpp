#include "SystemEntrance.h"

void showTitle() {
    cout << "================== 音响分贝模拟系统 ==================" << "\n";
    // 可以考虑在这里加入作者信息a
    cout << "图例：\n";
    cout << "\033[32mの\033[0m音量偏小    " << "\033[33mの\033[0m音量适中    " << 
            "\033[35mの\033[0m音量偏大    " << "\033[31mの\033[0m音响位置    " << "\n";
    cout << "模拟分布图中，每一个“の”表示一平方米，左上角为坐标原点" << "\n";
    cout << endl;
}

void menuLogic() {
    showTitle();
    int choice;
    do {
        showMenu();
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
                break;
            case 2:
                addSpeaker();
                break;
            case 3:
                modifySpeaker();
                break;
            case 4:
                deleteSpeaker();
                break;
            case 5: 
                openMap();
                break;
            case 6:
                storeSolution();
                break;
            case 7:
                readSolution();
                break;
            case 8:
                exitProgram();
                break;
            default:
                invalidInput();
        }
    } while (choice != 8);
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    _mkdir("output");
    menuLogic();
    return 0;
}