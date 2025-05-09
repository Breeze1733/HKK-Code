# 电声学音箱声压级（SPL）公式原理与推导

## 核心公式
音箱在自由场中的声压级（SPL，单位分贝）计算公式为：
$$
\text{SPL} = \text{灵敏度} + 10 \cdot \log_{10}\left(\frac{P}{P_0}\right) - 20 \cdot \log_{10}\left(\frac{d}{d_0}\right)
$$
**参数定义**：
- **灵敏度**（dB/W/m）：1瓦输入、1米距离的基准声压级
- $P$（W）：实际输入功率，$P_0 = 1\,\text{W}$ 为参考功率
- $d$（m）：听音距离，$d_0 = 1\,\text{m}$ 为参考距离

---

## 分步推导过程

### 1. 灵敏度基准定义
音箱灵敏度表示 **1瓦功率驱动下，1米距离处产生的声压级**，记为：
$$
\text{SPL}_{\text{基准}} = \text{灵敏度} \quad (\text{单位：dB})
$$

### 2. 功率修正项推导
声强 $I$ 与输入功率 $P$ 成正比：
$$
I \propto P
$$
根据分贝定义（$\text{dB} = 10 \cdot \log_{10}\left(\frac{I}{I_0}\right)$），功率修正项为：

$$
\Delta\text{SPL}_P = 10 \cdot \log_{10}\left(\frac{P}{P_0}\right)
$$
**关键特性**：
- 功率每增加10倍 → SPL增加10 dB
- 例：100 W输入时，$\Delta\text{SPL}_P = 10 \cdot \log_{10}(100/1) = 20\,\text{dB}$

### 3. 距离衰减项推导
声压遵循 **反平方定律**（能量扩散）：
$$
p \propto \frac{1}{d}
$$
声压级衰减公式为：
$$
\Delta\text{SPL}_d = 20 \cdot \log_{10}\left(\frac{p}{p_0}\right) = -20 \cdot \log_{10}\left(\frac{d}{d_0}\right)
$$
**关键特性**：
- 距离每增加1倍 → SPL衰减6 dB（因 $20 \cdot \log_{10}(2) \approx 6\,\text{dB}$）
- 例：2米距离时，$\Delta\text{SPL}_d = -20 \cdot \log_{10}(2/1) \approx -6\,\text{dB}$

---

## 完整公式整合
将三项合并，得到总声压级：
$$
\text{SPL} = \underbrace{\text{灵敏度}}_{\text{基准值}} + \underbrace{10 \cdot \log_{10}(P)}_{\text{功率增益}} - \underbrace{20 \cdot \