# 声压分布模拟系统核心原理说明

---

## 1. 主要参考资料

- [知乎：音箱声压级与方向性原理](https://zhuanlan.zhihu.com/p/581349687)
- [Aalto University 声学论文](https://aaltodoc.aalto.fi/server/api/core/bitstreams/53118e62-e0d0-4a6d-949b-b74049a069e4/content)
- IEC/AES/ISO 国际标准、主流声学教材
- Don Davis《Sound System Engineering》
- JBL Pro: Directivity Index and Q

---

## 2. 程序用到的声学原理

### 2.1 声压级（SPL）计算

自由场下，单个音箱的声压级（SPL, dB）计算公式：

$$
\text{SPL} = \text{灵敏度} + 10 \cdot \log_{10}\left(\frac{P}{P_0}\right) - 20 \cdot \log_{10}\left(\frac{d}{d_0}\right)
$$

- **灵敏度**（dB/W/m 或 dB/2.83V/m）：1瓦输入、1米距离的基准声压级
- $P$（W）：实际输入功率，$P_0 = 1\,\text{W}$
- $d$（m）：听音点到音箱的距离，$d_0 = 1\,\text{m}$

**要点：**
- 功率每增加10倍，SPL增加10dB
- 距离每加倍，SPL衰减6dB

---

### 2.2 灵敏度单位与换算

#### 灵敏度的两种单位

- **dB/W/m**：1瓦输入、1米距离的声压级（国际标准，反映功率效率）
- **dB/2.83V/m**：2.83V输入、1米距离的声压级（常见于8Ω音箱，反映电压驱动特性）

#### 单位换算逻辑

| 阻抗 | 2.83V等效功率 | 单位对应关系 |
|------|---------------|--------------|
| 8Ω   | 1W            | dB/2.83V/m = dB/W/m |
| 4Ω   | 2W            | dB/2.83V/m 比 dB/W/m 高约3dB |

- 换算公式：$\text{2.83V} = \sqrt{P \times R}$，$R$为阻抗
- 8Ω音箱：两个单位数值可直接等同比较
- 4Ω音箱：dB/2.83V/m 比 dB/W/m 高约3dB

**程序实现：**  
程序根据音箱阻抗自动换算灵敏度单位，确保不同阻抗下的灵敏度对比和声压计算准确。

---

### 2.3 方向性建模与覆盖角修正

#### 余弦幂模型（Cosine Power Model）

**公式：**
$$
D(\theta) = \cos^n(\theta)
$$

- $n = \frac{\log(0.5)}{\log(\cos(\theta_{-6dB}))}$，$\theta_{-6dB}$为-6dB覆盖角一半
- $\theta$为听音点与主轴夹角

**程序实现说明：**
- 主轴方向（$\theta=0$）无衰减，$\theta=\theta_{-6dB}$时正好-6dB
- 覆盖角随频率升高而变窄（$\text{coverage} \propto \sqrt{1000/\text{freqHz}}$，限定范围10°~180°）
- 方向性修正项 $DI = 20 \cdot \log_{10}(D(\theta))$，若$D(\theta)\leq0$则下限保护（如-40dB）

**出处：**
- Don Davis《Sound System Engineering》第8章
- [JBL Pro: Directivity Index and Q](https://jblpro.com/en/support/knowledge-base/2019-12-18-directivity-index-and-q)

#### 平滑方向性修正 `-20*log10((cosθ+1)/2)`

- 该公式常用于宽指向或全指向音箱，保证覆盖角外声压级平滑衰减，避免突变。
- 物理意义：主轴方向无衰减，侧向-6dB，背向理论上为负无穷，实际代码有下限保护。
- 相关讨论：[StackExchange: Why use (cosθ+1)/2 for directivity?](https://dsp.stackexchange.com/questions/65157/why-use-cos-theta-1-2-for-directivity)

**程序说明：**
- 程序主要采用余弦幂模型，部分宽指向场合可用平滑修正。

---

### 2.4 多音箱能量叠加

多个音箱的声压级采用**能量叠加**（分贝转功率再求和）：

$$
L_{\text{total}} = 10 \cdot \log_{10}\left(\sum_{i} 10^{L_i/10}\right)
$$

- 物理意义：声能量相加，符合实际声场规律

---

### 2.5 频率影响与空气吸收

- 高频（如8kHz）空气吸收显著，低频近似全指向
- 空气吸收修正：$\Delta SPL_{\text{air}} = \alpha \cdot r$
    - $\alpha$为频率相关吸收系数（如8kHz时约0.093dB/m）

---

### 2.6 其他程序相关物理假设

- **最小距离限制**：为避免极近距离导致的无穷大，距离下限设为1米
- **声压贡献阈值**：极低声压（如<1dB）不计入总和，提升效率
- **主轴朝向**：通过角度计算，支持任意方向的定向音箱

---

## 3. 程序未实现但常用的声学原理（可扩展）

- **线声源/阵列声源**：目前程序采用点声源模型，未实现线阵列的10dB/倍距衰减
- **房间混响与反射**：当前仅模拟自由场直达声，不含混响与反射声
- **障碍物遮挡**：未考虑声波被障碍物阻挡的影响

---

## 4. 参考与致谢

- 主要声学公式与工程模型参考自 IEC/AES/ISO 等国际标准及主流声学教材
- 方向性建模与能量叠加算法参考 Don Davis《Sound System Engineering》、JBL Pro 技术文档等

---