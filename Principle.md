# 声压分布模拟系统核心原理说明

---

## 1. 物理与程序模型假设

本系统在声学建模与仿真过程中，采用如下物理与计算假设以确保理论严谨性与工程可实现性：

- **自由场假设**：忽略房间混响、反射及障碍物遮挡，仅考虑声源至接收点的直达声路径，符合经典自由场理论 [1][3]。
- **点声源模型**：所有扬声器均视为理想点声源，暂不支持线阵列或面阵列声源建模 [4]。
- **最小距离限制**：为避免近场区域声压理论值趋于无穷，设定距离下限为1米。
- **声压贡献阈值**：对单个声源贡献极低（如<1 dB SPL）的点不计入总和，以提升计算效率。
- **主轴朝向建模**：通过空间角度参数化，支持任意方向定向扬声器的主轴朝向建模。
- **灵敏度单位自动换算**：根据扬声器阻抗自动处理 dB/W/m 与 dB/2.83V/m 两种主流灵敏度单位，确保不同设备间的物理量可比性 [3][7]。
- **方向性建模**：主要采用余弦幂模型（Cosine Power Model）描述声源方向性，宽指向场合可选用平滑修正模型 [1][2][6]。

---

## 2. 程序用到的声学原理

### 2.1 声压级（SPL）计算

在自由场条件下，单个点声源的声压级（SPL, dB）计算公式如下 [1][3]：

$$
\mathrm{SPL} = \mathrm{Sensitivity} + 10 \cdot \log_{10}\left(\frac{P}{P_0}\right) - 20 \cdot \log_{10}\left(\frac{d}{d_0}\right)
$$

其中：

- **灵敏度**（dB/W/m 或 dB/2.83V/m）：指扬声器在1瓦输入、1米距离处的基准声压级。
- $P$（W）：实际输入功率，$P_0 = 1\,\mathrm{W}$。
- $d$（m）：听音点到扬声器的距离，$d_0 = 1\,\mathrm{m}$。

**物理要点：**
- 功率每增加10倍，SPL提升10 dB。
- 距离每加倍，SPL衰减6 dB。

---

### 2.2 灵敏度单位与换算

#### 灵敏度的两种主流单位

- **dB/W/m**：1瓦输入、1米距离的声压级（国际标准，反映功率效率）。
- **dB/2.83V/m**：2.83V输入、1米距离的声压级（常见于8Ω扬声器，反映电压驱动特性）。

#### 单位换算逻辑

| 阻抗 | 2.83V等效功率 | 单位对应关系 |
|------|---------------|--------------|
| 8Ω   | 1W            | dB/2.83V/m = dB/W/m |
| 4Ω   | 2W            | dB/2.83V/m 比 dB/W/m 高约3dB |

- 换算公式：$\mathrm{2.83V} = \sqrt{P \times R}$，$R$为阻抗。
- 8Ω扬声器：两个单位数值可直接等同比较。
- 4Ω扬声器：dB/2.83V/m 比 dB/W/m 高约3 dB。

**程序实现说明：**  
系统自动根据扬声器阻抗进行灵敏度单位换算，确保不同阻抗下的灵敏度对比和声压计算物理一致性 [7]。

---

### 2.3 方向性建模与覆盖角修正

#### 余弦幂模型（Cosine Power Model）

该模型为声源方向性建模的工程标准 [1][2]：

$$
D(\theta) = \cos^n(\theta)
$$

- $n = \frac{\log(0.5)}{\log(\cos(\theta_{-6dB}))}$，$\theta_{-6dB}$为-6 dB覆盖角的一半。
- $\theta$为听音点与主轴夹角。

**实现要点：**
- 主轴方向（$\theta=0$）无衰减，$\theta=\theta_{-6dB}$时正好-6 dB。
- 覆盖角随频率升高而变窄（$\mathrm{coverage} \propto \sqrt{1000/\mathrm{freqHz}}$，限定范围10°~180°）。
- 方向性修正项 $DI = 20 \cdot \log_{10}(D(\theta))$，若$D(\theta)\leq0$则下限保护（如-40 dB）。

#### 平滑方向性修正

对于宽指向或全指向扬声器，采用如下平滑修正公式 [6]：

$$
DI = -20 \cdot \log_{10}\left(\frac{\cos\theta + 1}{2}\right)
$$

- 主轴方向无衰减，侧向-6 dB，背向理论上为负无穷，实际实现有下限保护。

**程序说明：**
- 默认采用余弦幂模型，部分宽指向场合可选用平滑修正。

---

### 2.4 多声源能量叠加

多个扬声器的声压级采用能量叠加法（分贝转功率再求和） [1]：

$$
L_{\mathrm{total}} = 10 \cdot \log_{10}\left(\sum_{i} 10^{L_i/10}\right)
$$

- 物理意义：声能量相加，符合实际声场能量守恒规律。

---

### 2.5 频率影响与空气吸收

高频声波在空气中传播时会受到显著吸收，低频影响较小。空气吸收修正公式如下 [5]：

$$
\Delta \mathrm{SPL}_{\mathrm{air}} = \alpha \cdot r
$$

- $\alpha$为频率相关吸收系数（dB/m），$r$为传播距离（m）。

**标准吸收系数 $\alpha$ 取值：**

| 频率范围         | $\alpha$ (dB/m) |
|------------------|-----------------|
| 8000Hz及以上     | 0.093           |
| 4000~8000Hz      | 0.028           |
| 1000~4000Hz      | 0.003           |
| 低于1000Hz       | 0               |

---

## 3. 程序未实现但常用的声学原理（可扩展）

- **线声源/阵列声源**：当前仅支持点声源模型，未实现线阵列声源的10 dB/倍距衰减特性 [4]。
- **房间混响与反射**：仅模拟自由场直达声，未考虑混响与反射声场。
- **障碍物遮挡**：未考虑声波传播过程中的障碍物遮挡与衍射效应。

---

## 参考文献与致谢

[1] Davis, D., & Patronis, E. (2013). *Sound System Engineering* (4th ed.). Focal Press.  
 （系统阐述指向性指数计算、覆盖角修正模型与多声源叠加原理）

[2] JBL Professional. (2019). *Directivity Index and Q*. JBL Pro Knowledge Base.  
 Retrieved from https://jblpro.com/en/support/knowledge-base/2019-12-18-directivity-index-and-q  
 （覆盖角与余弦幂模型的工程实现标准）

[3] International Electrotechnical Commission. (2018). *IEC 60268-5: Sound system equipment - Part 5: Loudspeakers*.  
 （灵敏度定义、自由场SPL计算方法的国际标准）

[4] AES56-2007. (2008). *AES standard on acoustics - Sound source modeling*. Audio Engineering Society.  
 （点声源与线声源建模的权威规范）

[5] ISO 9613-1. (1993). *Acoustics - Attenuation of sound during propagation outdoors - Part 1: Calculation of the absorption of sound by the atmosphere*.  
 （高频空气吸收系数α的标准化取值依据）

[6] Anonymous. (2022). *Why use (cosθ+1)/2 for directivity?*. Signal Processing Stack Exchange.  
 Retrieved from https://dsp.stackexchange.com/questions/65157/why-use-cos-theta-1-2-for-directivity  
 （平滑方向性修正公式的理论探讨）

[7] 知乎专栏. (2022). *音箱声压级与方向性原理*. 知乎.  
 Retrieved from https://zhuanlan.zhihu.com/p/581349687  
 （灵敏度单位换算与SPL衰减规律的通俗解读）

[8] Aalto University. (2020). *Acoustics related thesis*.  
 Retrieved from https://aaltodoc.aalto.fi/server/api/core/bitstreams/53118e62-e0d0-4a6d-949b-b74049a069e4/content  
 （声学建模相关学术论文）

[9] CSDN 博客. (2024). *声压级SPL计算与声场仿真原理*.  
 Retrieved from https://blog.csdn.net/weixin_42929997/article/details/143598621  
 （SPL计算与仿真工程实现的网络参考）

---