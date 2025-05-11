#include "CheckInput.h"

void clearConsole() {
    // 将光标移动到指定行之后
    cout << "\033[" << 7 << ";1H"; // 移动光标到第 linesToKeep+1 行
    // 清除从光标到屏幕底部的内容
    cout << "\033[J";
}

void clearAboveLines(const int & linesToClear) {
    for (int i = 0; i < linesToClear; ++i) {
        // 将光标移动到上一行的开头
        cout << "\033[F";
        // 清除当前行内容
        cout << "\033[2K";
    }
}

int getInput(const int & min_value, const int & max_value) {
    string input;
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

int askQuestion(const int & min_value, const int & max_value, const string & question) {
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