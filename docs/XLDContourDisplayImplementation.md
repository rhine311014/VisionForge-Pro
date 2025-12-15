# XLD轮廓非破坏性显示实现文档

## 实现日期
2025-12-15

## 概述
实现了基于Halcon XLD轮廓的非破坏性显示功能，允许在原图上叠加显示匹配结果的轮廓，而不是将结果烧录到图像上。这样可以保持原图不变，同时提供更灵活的显示选项。

## 实现步骤

### 步骤1: 扩展ToolResult支持Halcon对象

**文件**: [include/algorithm/VisionTool.h](../include/algorithm/VisionTool.h:28)

添加了displayObjects字段来存储显示对象：

```cpp
struct ToolResult {
    bool success;
    QString errorMessage;
    QJsonObject data;
    Base::ImageData::Ptr outputImage;
    double executionTime;
    QVariantMap displayObjects;  // 新增：显示对象

    void setDisplayObject(const QString& key, const QVariant& object);
    QVariant getDisplayObject(const QString& key) const;
};
```

**设计考虑**:
- 使用QVariantMap而不是直接使用Halcon对象，以保持ToolResult的通用性
- 支持多种类型的显示对象（XLD轮廓、Region等）
- 与现有的outputImage字段配合使用

### 步骤2: 创建HalconObjectWrapper包装类

**文件**: [include/algorithm/HalconObjectWrapper.h](../include/algorithm/HalconObjectWrapper.h)

创建了一套包装类来封装Halcon对象：

```cpp
class HalconObjectWrapper {
public:
    virtual ~HalconObjectWrapper() = default;
    enum ObjectType { XLD_Contour, XLD_Polygon, Region, Image };
    virtual ObjectType type() const = 0;
};

class XLDContourWrapper : public HalconObjectWrapper {
public:
    XLDContourWrapper(const HXLDCont& contours);
    const HXLDCont& contours() const;
    ObjectType type() const override { return XLD_Contour; }
private:
    HXLDCont contours_;
};

// 智能指针类型
using HalconObjectPtr = QSharedPointer<HalconObjectWrapper>;
using XLDContourPtr = QSharedPointer<XLDContourWrapper>;
```

**为什么需要包装类**:
1. Halcon对象无法直接存储在QVariant中
2. 使用QSharedPointer管理内存，避免Halcon对象拷贝
3. 通过基类实现多态，支持不同类型的Halcon对象
4. 注册到Qt元类型系统：`Q_DECLARE_METATYPE(VisionForge::Algorithm::HalconObjectPtr)`

### 步骤3: 修改ShapeMatchTool支持XLD输出

#### 3.1 添加useXLDDisplay_选项

**文件**: [include/algorithm/ShapeMatchTool.h](../include/algorithm/ShapeMatchTool.h:150)

```cpp
private:
    bool useXLDDisplay_;  // 是否使用XLD轮廓显示

public:
    void setUseXLDDisplay(bool use);
    bool getUseXLDDisplay() const;
```

默认值设为true（构造函数中），优先使用非破坏性显示。

#### 3.2 修改process方法

**文件**: [src/algorithm/ShapeMatchTool.cpp](../src/algorithm/ShapeMatchTool.cpp:239-251)

根据useXLDDisplay_标志选择输出方式：

```cpp
if (useXLDDisplay_) {
    // 使用XLD轮廓显示（非破坏性）
    HXLDCont contours = createMatchContours(row, col, angle);
    XLDContourPtr xldPtr = QSharedPointer<XLDContourWrapper>::create(contours);
    output.setDisplayObject("match_contours", QVariant::fromValue(
        qSharedPointerCast<HalconObjectWrapper>(xldPtr)));
    output.outputImage = input;  // 返回原图
} else {
    // 绘制结果到输出图像（破坏性）
    cv::Mat resultImg = input->mat().clone();
    drawMatchResults(resultImg, row, col, angle, score);
    output.outputImage = std::make_shared<Base::ImageData>(resultImg);
}
```

**对比两种方式**:
| 方式 | 优点 | 缺点 |
|------|------|------|
| XLD轮廓显示 | 保持原图、可调整颜色/线宽、支持缩放 | 需要Halcon窗口支持 |
| 烧录到图像 | 简单、兼容性好 | 破坏原图、不可调整 |

#### 3.3 实现createMatchContours方法

**文件**: [src/algorithm/ShapeMatchTool.cpp](../src/algorithm/ShapeMatchTool.cpp:345-380)

```cpp
HXLDCont ShapeMatchTool::createMatchContours(const HTuple& row, const HTuple& col,
                                             const HTuple& angle)
{
    HXLDCont allContours;

    // 1. 获取模板轮廓
    HXLDCont modelContours;
    GetShapeModelContours(&modelContours, shapeModel_, 1);

    // 2. 对每个匹配结果变换轮廓
    for (int i = 0; i < numMatches; i++) {
        // 计算仿射变换矩阵
        HTuple homMat2D;
        VectorAngleToRigid(modelRefRow_, modelRefCol_, 0,  // 模板参考点
                         row[i].D(), col[i].D(), angle[i].D(),  // 匹配位置和角度
                         &homMat2D);

        // 变换轮廓
        HXLDCont transformedCont;
        AffineTransContourXld(modelContours, &transformedCont, homMat2D);

        // 合并
        allContours = (i == 0) ? transformedCont :
                                 allContours.ConcatObj(transformedCont);
    }

    return allContours;
}
```

**关键Halcon API**:
- `GetShapeModelContours`: 从形状模型获取XLD轮廓
- `VectorAngleToRigid`: 计算2D刚性变换矩阵
- `AffineTransContourXld`: 仿射变换XLD轮廓
- `ConcatObj`: 合并多个Halcon对象

### 步骤4: 参数序列化

**文件**: [src/algorithm/ShapeMatchTool.cpp](../src/algorithm/ShapeMatchTool.cpp:392,406)

添加useXLDDisplay_到序列化/反序列化：

```cpp
QJsonObject ShapeMatchTool::serializeParams() const {
    json["use_xld_display"] = useXLDDisplay_;
    // ... 其他参数
}

void ShapeMatchTool::deserializeParams(const QJsonObject& json) {
    useXLDDisplay_ = json.value("use_xld_display").toBool(true);
    // ... 其他参数
}
```

## 当前状态

### ✅ 已完成
1. ToolResult扩展支持显示对象
2. HalconObjectWrapper包装类创建
3. ShapeMatchTool XLD轮廓输出功能
4. createMatchContours方法实现
5. 编译成功

### ⏳ 待完成
1. **HalconImageViewer显示XLD对象** - 需要修改HalconDisplayWorker来渲染XLD轮廓
2. **ShapeMatchParamPanel添加XLD选项** - 添加复选框控制useXLDDisplay_
3. **MainWindow集成** - 连接ToolResult的displayObjects到HalconImageViewer

## 使用示例

### 代码示例

```cpp
// 创建工具并启用XLD显示
ShapeMatchTool* tool = new ShapeMatchTool();
tool->setUseXLDDisplay(true);  // 使用非破坏性显示

// 加载模板并处理图像
tool->loadModel("model.shm");
ToolResult result;
if (tool->process(image, result)) {
    // 获取XLD轮廓对象
    QVariant objVar = result.getDisplayObject("match_contours");
    if (objVar.isValid()) {
        HalconObjectPtr halconObj = objVar.value<HalconObjectPtr>();
        XLDContourPtr xldPtr = qSharedPointerCast<XLDContourWrapper>(halconObj);

        // 在HalconImageViewer中显示
        halconViewer->setImage(result.outputImage);
        halconViewer->displayXLDContour(xldPtr->contours(), "green", 2);
    }
}
```

### 切换显示模式

```cpp
// 方式1: XLD轮廓显示（非破坏性）
tool->setUseXLDDisplay(true);
tool->process(image, result);
// result.outputImage是原图
// result.displayObjects["match_contours"]包含XLD轮廓

// 方式2: 烧录到图像（破坏性）
tool->setUseXLDDisplay(false);
tool->process(image, result);
// result.outputImage包含绘制了结果的图像
// result.displayObjects为空
```

## 技术难点与解决方案

### 问题1: Halcon对象无法存储在QVariant

**问题**: HXLDCont等Halcon对象无法直接序列化到QJsonObject或QVariant。

**解决方案**:
1. 创建包装类HalconObjectWrapper封装Halcon对象
2. 使用QSharedPointer管理生命周期
3. 注册到Qt元类型系统：`Q_DECLARE_METATYPE`
4. 通过QVariant::fromValue()和value<T>()传递

### 问题2: Halcon API参数类型

**错误代码**:
```cpp
// ❌ 错误：VectorAngleToRigid最后一个参数是HTuple*而不是HXLDCont*
VectorAngleToRigid(0, 0, 0, row, col, angle, &transformedCont);

// ❌ 错误：AffineTransContourXld第三个参数是HTuple而不是HXLDCont
AffineTransContourXld(modelContours, &transformedCont, transformedCont);
```

**正确代码**:
```cpp
// ✅ 正确：先计算变换矩阵
HTuple homMat2D;
VectorAngleToRigid(modelRefRow_, modelRefCol_, 0,
                 row[i].D(), col[i].D(), angle[i].D(), &homMat2D);

// ✅ 正确：使用HTuple作为变换矩阵
AffineTransContourXld(modelContours, &transformedCont, homMat2D);
```

### 问题3: 多个轮廓合并

**解决方案**: 使用HXLDCont::ConcatObj方法
```cpp
if (i == 0) {
    allContours = transformedCont;
} else {
    allContours = allContours.ConcatObj(transformedCont);
}
```

## 下一步工作

### 1. 修改HalconImageViewer显示XLD对象

需要在HalconDisplayWorker中添加：

```cpp
class HalconDisplayWorker : public QThread {
    // ... 现有代码 ...
private:
    QList<HXLDCont> xldContours_;  // 存储待显示的XLD轮廓
    QStringList xldColors_;        // 对应颜色

public:
    void addXLDContour(const HXLDCont& cont, const QString& color = "green");
    void clearXLDContours();

protected:
    void run() override {
        // 显示图像
        DispObj(hImage_, windowHandle_);

        // 显示XLD轮廓
        for (int i = 0; i < xldContours_.size(); i++) {
            SetColor(windowHandle_, xldColors_[i].toStdString().c_str());
            DispObj(xldContours_[i], windowHandle_);
        }
    }
};
```

### 2. 在ShapeMatchParamPanel添加XLD选项

```cpp
QCheckBox* useXLDDisplayCheckBox_ = new QCheckBox("使用XLD轮廓显示", this);
useXLDDisplayCheckBox_->setChecked(tool_->getUseXLDDisplay());
connect(useXLDDisplayCheckBox_, &QCheckBox::toggled,
        [this](bool checked) {
            tool_->setUseXLDDisplay(checked);
            emit parameterChanged();
        });
```

### 3. MainWindow集成

```cpp
void MainWindow::onToolProcessed(const ToolResult& result) {
    // 显示输出图像
    halconViewer_->setImage(result.outputImage);

    // 如果有显示对象，叠加显示
    if (result.displayObjects.contains("match_contours")) {
        HalconObjectPtr obj = result.getDisplayObject("match_contours")
                                    .value<HalconObjectPtr>();
        if (obj->type() == HalconObjectWrapper::XLD_Contour) {
            XLDContourPtr xld = qSharedPointerCast<XLDContourWrapper>(obj);
            halconViewer_->addXLDContour(xld->contours(), "green");
        }
    }
}
```

## 优势与效果

### 优势

1. **非破坏性** - 原图保持不变，可随时切换显示/隐藏结果
2. **灵活性** - 可调整颜色、线宽等显示属性
3. **性能** - 避免图像拷贝和绘制操作
4. **精度** - XLD轮廓包含亚像素精度信息
5. **可扩展** - 支持多种Halcon对象类型（Region、Polygon等）

### 与传统方式对比

| 特性 | XLD轮廓显示 | 传统绘制 |
|------|-------------|----------|
| 原图 | 保持不变 | 被修改 |
| 显示控制 | 可调整颜色/线宽 | 固定 |
| 缩放效果 | 清晰 | 可能模糊 |
| 性能 | 优秀 | 一般 |
| 精度 | 亚像素 | 整数像素 |
| 兼容性 | 需要Halcon窗口 | 通用 |

## 编译结果

```
✅ VisionForgeAlgorithm.lib - 成功
✅ VisionForge.exe - 成功
```

新增/修改文件：
- `include/algorithm/VisionTool.h` - ToolResult扩展
- `include/algorithm/HalconObjectWrapper.h` - 包装类（新增）
- `include/algorithm/ShapeMatchTool.h` - 添加XLD支持
- `src/algorithm/ShapeMatchTool.cpp` - 实现createMatchContours
- `src/algorithm/CMakeLists.txt` - 添加HalconObjectWrapper.h

## 总结

本次实现了XLD轮廓非破坏性显示的底层支持，包括：
1. ✅ ToolResult数据结构扩展
2. ✅ Halcon对象包装机制
3. ✅ ShapeMatchTool XLD轮廓生成
4. ✅ 编译通过

还需要完成HalconImageViewer的显示功能和UI集成，之后就可以实现完整的非破坏性显示流程。

这为后续实现更多Halcon可视化功能（Region高亮、测量结果标注等）奠定了基础。
