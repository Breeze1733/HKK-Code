#include "ManageData.h"

void storeDataToFile(const string & outputPath, Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    ofstream outFile(outputPath);
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
                << speaker.getRmsPower() << " " // 保存
                << speaker.getCoverageAngle() << " "
                << speaker.getMainAxisOrientation() << "\n";
    }
    for (auto& speaker : speakers) {outFile << speaker.getType() << " ";}
    outFile << "\n";

    outFile << type.size() << "\n";
    for (auto& t : type) {
        outFile << t[0] << " " << t[1] << " " << t[2] << " " << t[3] << "\n";
    }
    outFile.close();
}

void getDataFromFile(const string & inputPath, Field &field, vector<Speaker> &speakers, vector<vector<int>> &type) {
    ifstream inFile(inputPath);
    if (!inFile) {
        cout << "读取错误，暂无储存方案，";
        return;
    }
    int width, length;
    inFile >> width >> length;
    field.setWidth(width);
    field.setLength(length);

    int speakerCount;
    inFile >> speakerCount;
    speakers.clear();
    for (int i = 0; i < speakerCount; ++i) {
        int x, y, sensitivity, impedance, rmsPower, coverageAngle, mainAxisOrientation;
        inFile >> x >> y >> sensitivity >> impedance >> rmsPower >> coverageAngle >> mainAxisOrientation; // 读取
        Speaker speaker(x, y, sensitivity, impedance, rmsPower, coverageAngle, mainAxisOrientation);
        speakers.push_back(speaker);
    }
    for (int i = 0; i < speakerCount; ++i) {
        int ty;
        inFile >> ty;
        speakers[i].setType(ty);
    }

    int typeCount;
    inFile >> typeCount;
    type.clear();
    for (int i = 0; i < typeCount; ++i) {
        vector<int> t(4);
        inFile >> t[0] >> t[1] >> t[2] >> t[3];
        type.push_back(t);
    }
    inFile.close();
    cout << "方案已读取，";
}