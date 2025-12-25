# X1X2Y/XY1Y2 平台支持技术设计文档

## 1. 概述

本文档详细描述了VisionForge Pro中X1X2Y和XY1Y2平台类型的技术设计方案，参考VisionASM的成熟实现。

### 1.1 平台类型说明

| 平台类型 | 结构说明 | 应用场景 |
|---------|---------|---------|
| **X1X2Y** | 龙门双X轴结构 | 大幅面PCB对位、面板对位 |
| **XY1Y2** | 双Y轴结构 | 长条形产品对位 |

### 1.2 现有架构

VisionForge Pro已具备以下基础：

```
include/platform/
├── PlatformTypes.h      # PlatformType枚举、PlatformAxisPos结构
├── CoordPosition.h      # 坐标位姿类（支持多点对位）
└── PlatformConfig.h     # PlatformX1X2YInfo、PlatformXY1Y2Info类

src/platform/
├── PlatformTypes.cpp
├── CoordPosition.cpp    # toPlatformAxis()方法
└── PlatformConfig.cpp   # convertToXYD()/convertFromXYD()方法
```

---

## 2. X1X2Y平台几何模型

### 2.1 结构示意图

```
        X1轴                    X2轴
         ↑                       ↑
         |                       |
    +----+----+             +----+----+
    |         |             |         |
    |  X1电机  |             |  X2电机  |
    |         |             |         |
    +----+----+             +----+----+
         |                       |
         +----------- -----------+
                     |
                     Y
                   Y轴电机
```

### 2.2 坐标转换原理

**X1X2Y → XYD 转换：**

```cpp
// 输入: X1位置, X2位置, Y位置
// 输出: X位置, Y位置, D角度

// 1. 计算旋转臂长度 L（X1和X2之间的Y方向距离）
L = |X1_pos.Y - X2_pos.Y|

// 2. 计算旋转角度（X1X2差值产生旋转）
dx = X2 - X1
D = atan2(dx, L) * 180 / π

// 3. 计算中心X位置
X = (X1 + X2) / 2

// 4. Y位置不变
Y = Y_input
```

**XYD → X1X2Y 转换：**

```cpp
// 输入: X位置, Y位置, D角度
// 输出: X1位置, X2位置, Y位置

// 1. 计算差值
dx = L * tan(D * π / 180)

// 2. 分配到两个X轴
X1 = X - dx / 2
X2 = X + dx / 2

// 3. Y位置不变
Y = Y_input
```

### 2.3 轴方向处理

VisionASM定义了轴方向类型用于处理不同安装方向：

```cpp
enum class AxisDirectionType {
    Negative = -1,  // 轴正向与应用坐标系相反
    Positive = 1,   // 轴正向与应用坐标系相同
    None = 0        // 该轴不存在
};
```

**方向修正算法：**
```cpp
// 转换前应用方向修正
x1_corrected = x1 * x1Direction
x2_corrected = x2 * x2Direction
y_corrected = y * yDirection

// 执行标准转换
// ...

// 转换后逆向修正
x1_output = x1_result / x1Direction
x2_output = x2_result / x2Direction
y_output = y_result / yDirection
```

---

## 3. XY1Y2平台几何模型

### 3.1 结构示意图

```
              X轴电机
                ↑
    +----------+----------+
    |          |          |
    |          X          |
    |          |          |
    +----------+----------+
    |                     |
   Y1轴                  Y2轴
    ↓                     ↓
```

### 3.2 坐标转换

转换逻辑与X1X2Y对称：

```cpp
// XY1Y2 → XYD
L = |Y1_pos.X - Y2_pos.X|  // Y1和Y2之间的X方向距离
dy = Y2 - Y1
D = atan2(dy, L) * 180 / π
X = X_input
Y = (Y1 + Y2) / 2

// XYD → XY1Y2
dy = L * tan(D * π / 180)
Y1 = Y - dy / 2
Y2 = Y + dy / 2
X = X_input
```

---

## 4. 改进方案

### 4.1 PlatformX1X2YInfo::convertToXYD 改进

```cpp
void PlatformX1X2YInfo::convertToXYD(double x1, double x2, double y,
                                      double& x, double& yOut, double& d) const {
    // Step 1: 应用轴方向修正
    double x1Corrected = x1 * static_cast<int>(x1Direction);
    double x2Corrected = x2 * static_cast<int>(x2Direction);
    double yCorrected = y * static_cast<int>(yDirection);

    // Step 2: 计算旋转臂长度
    double armLength = std::abs(x1Pos.getMainY() - x2Pos.getMainY());
    if (armLength < 1e-6) {
        armLength = std::abs(x1Pos.getMainX() - x2Pos.getMainX());
        if (armLength < 1e-6) {
            armLength = 114.0;  // 默认值
        }
    }

    // Step 3: 计算旋转角度
    double dx = x2Corrected - x1Corrected;
    double angleRad = std::atan2(dx, armLength);
    d = angleRad * 180.0 / M_PI;

    // Step 4: 计算中心位置
    x = (x1Corrected + x2Corrected) / 2.0;
    yOut = yCorrected;
}
```

### 4.2 PlatformX1X2YInfo::convertFromXYD 改进

```cpp
void PlatformX1X2YInfo::convertFromXYD(double x, double y, double d,
                                        double& x1, double& x2, double& yOut) const {
    // Step 1: 计算旋转臂长度
    double armLength = std::abs(x1Pos.getMainY() - x2Pos.getMainY());
    if (armLength < 1e-6) {
        armLength = std::abs(x1Pos.getMainX() - x2Pos.getMainX());
        if (armLength < 1e-6) {
            armLength = 114.0;
        }
    }

    // Step 2: 计算差值
    double angleRad = d * M_PI / 180.0;
    double dx = armLength * std::tan(angleRad);

    // Step 3: 分配到两个X轴
    double x1Result = x - dx / 2.0;
    double x2Result = x + dx / 2.0;

    // Step 4: 应用逆向方向修正
    x1 = x1Result / static_cast<int>(x1Direction);
    x2 = x2Result / static_cast<int>(x2Direction);
    yOut = y / static_cast<int>(yDirection);
}
```

### 4.3 CoordPosition::toPlatformAxis 集成

```cpp
case PlatformType::X1X2Y: {
    // 使用平台配置进行精确转换
    if (platformInfo) {
        auto* info = static_cast<const PlatformX1X2YInfo*>(platformInfo);
        info->convertFromXYD(m_mainX, m_mainY, m_angle,
                             axisPos.x1, axisPos.x2, axisPos.y1);
    } else {
        // 默认简化计算
        double halfAngle = m_angle * M_PI / 180.0 * 57.0;  // 假设57mm臂长
        axisPos.x1 = m_mainX - halfAngle;
        axisPos.x2 = m_mainX + halfAngle;
        axisPos.y1 = m_mainY;
    }
    break;
}
```

---

## 5. 测试用例设计

### 5.1 X1X2Y转换测试

```cpp
TEST(PlatformX1X2YTest, ConvertToXYD_BasicCase) {
    PlatformX1X2YInfo info;
    // 设置旋转臂长度为114mm（X1和X2 Y位置差）
    info.x1Pos.setMainPos(67.5, -57);
    info.x2Pos.setMainPos(67.5, 57);

    double x, y, d;
    // X1=100, X2=100, Y=50 -> 无旋转
    info.convertToXYD(100, 100, 50, x, y, d);
    EXPECT_NEAR(x, 100.0, 0.001);
    EXPECT_NEAR(y, 50.0, 0.001);
    EXPECT_NEAR(d, 0.0, 0.001);
}

TEST(PlatformX1X2YTest, ConvertToXYD_WithRotation) {
    PlatformX1X2YInfo info;
    info.x1Pos.setMainPos(67.5, -57);
    info.x2Pos.setMainPos(67.5, 57);

    double x, y, d;
    // X1=90, X2=110 -> 差值20mm
    info.convertToXYD(90, 110, 50, x, y, d);
    EXPECT_NEAR(x, 100.0, 0.001);  // 中心位置
    EXPECT_NEAR(y, 50.0, 0.001);
    // 角度 = atan2(20, 114) * 180/π ≈ 9.95度
    EXPECT_NEAR(d, 9.95, 0.5);
}

TEST(PlatformX1X2YTest, RoundTrip) {
    PlatformX1X2YInfo info;
    info.x1Pos.setMainPos(67.5, -57);
    info.x2Pos.setMainPos(67.5, 57);

    double x1_in = 95, x2_in = 105, y_in = 60;
    double x, y, d;
    double x1_out, x2_out, y_out;

    // X1X2Y -> XYD
    info.convertToXYD(x1_in, x2_in, y_in, x, y, d);

    // XYD -> X1X2Y
    info.convertFromXYD(x, y, d, x1_out, x2_out, y_out);

    // 验证往返转换
    EXPECT_NEAR(x1_in, x1_out, 0.001);
    EXPECT_NEAR(x2_in, x2_out, 0.001);
    EXPECT_NEAR(y_in, y_out, 0.001);
}
```

### 5.2 轴方向测试

```cpp
TEST(PlatformX1X2YTest, AxisDirectionNegative) {
    PlatformX1X2YInfo info;
    info.x1Direction = AxisDirectionType::Negative;
    info.x2Direction = AxisDirectionType::Positive;
    info.x1Pos.setMainPos(67.5, -57);
    info.x2Pos.setMainPos(67.5, 57);

    double x, y, d;
    // X1=-100（物理正向=逻辑负向）, X2=100
    info.convertToXYD(-100, 100, 50, x, y, d);

    // 修正后: x1_corrected = -100 * (-1) = 100
    // x2_corrected = 100 * 1 = 100
    // 所以无旋转
    EXPECT_NEAR(d, 0.0, 0.001);
}
```

---

## 6. 实施步骤

1. **更新PlatformConfig.cpp**
   - 修改`convertToXYD()`添加轴方向支持
   - 修改`convertFromXYD()`添加逆向方向修正

2. **更新CoordPosition.cpp**
   - 完善`toPlatformAxis()`的X1X2Y和XY1Y2分支
   - 完善`fromPlatformAxis()`的反向转换

3. **添加单元测试**
   - 创建`test/test_PlatformCoord_gtest.cpp`
   - 覆盖基本转换、旋转、轴方向等场景

4. **构建验证**
   - 运行cmake构建
   - 执行单元测试确保通过

---

## 7. 参考资料

- VisionASM `vsUVW.h` - CX1X2Y、CXY1Y2类定义
- VisionASM `vsAlignToolDef.cpp` - ConvertX1X2YPltfmPosToXYDPltfmPos实现
- VisionASM `vsAlignToolDef.h` - PlatformType枚举、CPlatformX1X2YInfo类

---

*文档版本: 1.0*
*创建日期: 2025-12-21*
*作者: VisionForge Team*
