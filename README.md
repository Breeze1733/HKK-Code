# 音响声压分布模拟系统

---

## 1. 项目简介

本项目为“音响声压分布模拟系统”，面向声学教学、扩声设计、音响工程等场景，提供可视化的声压分布仿真。用户可自定义场地、音响类型及参数，系统基于科学声学模型计算并以彩色字符图形直观展示不同频率下的声压分布。采用前后端分离设计，前端负责菜单与数据管理，后端负责声压分布计算与可视化。

**主要功能亮点：**
- 支持多种场景（如公园、舞台、礼堂、教室等）参数推荐与自定义
- 支持音响灵敏度（dB/W/m 或 dB/2.83V/m）、功率、覆盖角、主轴朝向等参数调节
- 多音响叠加，支持定向/全向模拟
- 分贝分布图彩色显示，分贝区间和音响位置颜色直观
- 支持方案保存/读取，便于多场景对比

---

## 2. 目录结构说明

```
HKK-Code/
├── Back_End/
│   ├── ShowMap.cpp           # 后端主程序，分贝分布图显示
│   ├── ShowMap.h             # 后端分贝分布图相关声明
│   ├── MapCalculator.cpp     # 声压分布计算核心实现
│   ├── MapCalculator.h       # 声压分布计算核心声明
│   ├── DecibelThreshold.h    # 分贝阈值结构体
│   └── ...
├── Front_End/
│   ├── SystemEntrance.cpp    # 前端主菜单与交互
│   ├── SystemEntrance.h      # 前端主菜单声明
│   ├── MenuFunction.cpp      # 菜单功能实现
│   ├── MenuFunction.h        # 菜单功能声明
│   ├── ManageData.cpp        # 数据存储与读取实现
│   ├── ManageData.h          # 数据存储与读取声明
│   ├── CheckInput.cpp        # 输入校验实现
│   ├── CheckInput.h          # 输入校验声明
│   └── ...
├── Public_Class/
│   ├── Field.h               # 场地类
│   ├── Speaker.h             # 音响类
│   └── ...
├── Output/
│   ├── ShowMap.exe           # 后端可执行文件
│   ├── 音响声压分布模拟系统.exe # 前端可执行文件
│   └── ...
├── Principle.md              # 算法与声学原理说明
├── README.md                 # 项目说明（本文件）
└── .vscode/
    └── tasks.json            # VSCode 构建任务
```

---

## 3. 环境与依赖

- **操作系统**：Windows 10/11
- **编译器**：g++（建议 MinGW-w64，支持 C++17 标准）
- **终端要求**：支持 ANSI 彩色（推荐 Windows Terminal 或新版 CMD，老版 CMD 可能不支持彩色字符）
- **开发工具**：推荐使用 Visual Studio Code，项目已集成 VSCode 构建任务（见 .vscode/tasks.json）

**依赖说明：**
- 项目仅依赖标准 C++ 库和 Windows 控制台 API，无需额外第三方库。
- 若需跨平台运行，需自行适配控制台颜色与字体设置相关代码。

---

## 4. 编译与运行

### 4.1 编译

推荐使用 VSCode，直接按 `Ctrl+Shift+B` 选择对应任务自动编译。

也可手动命令行编译：

```sh
g++ -std=c++17 -mconsole -I./Public_Class Front_End/CheckInput.cpp Front_End/ManageData.cpp Front_End/MenuFunction.cpp Front_End/SystemEntrance.cpp -o Output/音响声压分布模拟系统.exe

g++ -std=c++17 -mconsole -I./Public_Class Back_End/ShowMap.cpp Back_End/MapCalculator.cpp -o Output/ShowMap.exe
```

### 4.2 运行

1. 运行前端主程序（菜单与数据管理）：

   ```
   Output/音响声压分布模拟系统.exe
   ```

2. 按菜单提示设置场地、添加音响类型、摆放音响等。
3. 选择“打开声压分布图”后，会自动调用后端 `ShowMap.exe`，依次显示低频（500Hz）、中频（1kHz）、高频（8kHz）下的声压分布。

---

## 5. 使用说明

- **场地设置**：支持自定义宽度和长度，单位为米。
- **音响类型**：可自定义灵敏度（dB/W/m 或 dB/2.83V/m）、阻抗、RMS功率、-6dB覆盖角等参数。
- **音响摆放**：支持多音响叠加，定向/全向模拟，主轴朝向可调。
- **分贝分布图**：支持三种频率（500Hz、1kHz、8kHz）下的声压分布可视化，分贝区间和音响位置以不同颜色显示。
- **方案保存/读取**：可保存当前场地与音响方案，便于多场景对比。

---

## 6. 算法与原理

- **声压级计算**：采用标准电声学公式，支持灵敏度、功率、距离、方向性等参数影响，详见 [Principle.md](./Principle.md)。
- **方向性建模**：支持全指向、定向（覆盖角、主轴朝向），采用余弦幂模型等工程常用算法，频率越高定向性越强。
- **多音响叠加**：采用能量叠加（分贝转功率再求和）方式，符合实际声场物理规律。
- **频率影响**：高频段考虑空气吸收和定向性变化，低频段近似全指向。

---

## 7. 常见问题

- **Q: 分布图乱码或无彩色？**  
  A: 请使用 Windows Terminal 或新版 CMD，确保终端支持 ANSI 彩色。
- **Q: 运行 ShowMap.exe 无反应？**  
  A: 请先通过前端菜单正确保存数据，确保 output/data.txt 存在且格式正确。
- **Q: 如何自定义音响参数？**  
  A: 在菜单中选择“添加新的音响类型”，可自定义灵敏度、阻抗、功率、覆盖角等。

---

## 8. 参考与致谢

- 主要声学公式与工程模型参考自 IEC/AES/ISO 等国际标准及主流声学教材。
- 详细原理推导与工程背景见 [Principle.md](./Principle.md)。

---

## 9. 许可协议

本项目仅供学习与教学用途，禁止用于商业用途。欢迎二次开发与改进，转载请注明出处。

---
