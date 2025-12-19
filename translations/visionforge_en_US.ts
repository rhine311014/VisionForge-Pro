<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en_US">

<!-- ToolFactory 工具名称和描述翻译 -->
<context>
    <name>ToolFactory</name>

    <!-- 工具类别 -->
    <message>
        <source>图像预处理</source>
        <translation>Image Preprocessing</translation>
    </message>
    <message>
        <source>定位检测</source>
        <translation>Detection &amp; Location</translation>
    </message>
    <message>
        <source>标定</source>
        <translation>Calibration</translation>
    </message>
    <message>
        <source>测量计算</source>
        <translation>Measurement</translation>
    </message>
    <message>
        <source>判定输出</source>
        <translation>Judgment &amp; Output</translation>
    </message>
    <message>
        <source>AI视觉</source>
        <translation>AI Vision</translation>
    </message>

    <!-- 灰度转换 -->
    <message>
        <source>灰度转换</source>
        <translation>Grayscale Convert</translation>
    </message>
    <message>
        <source>将彩色图像转换为灰度图像，支持多种转换算法</source>
        <translation>Convert color image to grayscale, supports multiple algorithms</translation>
    </message>

    <!-- 图像模糊 -->
    <message>
        <source>图像模糊</source>
        <translation>Image Blur</translation>
    </message>
    <message>
        <source>对图像进行模糊处理，支持均值、高斯、中值、双边滤波</source>
        <translation>Apply blur filter to image, supports mean, Gaussian, median, bilateral filtering</translation>
    </message>

    <!-- 图像二值化 -->
    <message>
        <source>图像二值化</source>
        <translation>Image Threshold</translation>
    </message>
    <message>
        <source>将图像转换为二值图像，支持固定阈值、自适应阈值、OTSU等算法</source>
        <translation>Convert image to binary, supports fixed threshold, adaptive threshold, OTSU</translation>
    </message>

    <!-- 边缘检测 -->
    <message>
        <source>边缘检测</source>
        <translation>Edge Detection</translation>
    </message>
    <message>
        <source>检测图像边缘，支持Canny、Sobel、Laplacian、Scharr算法</source>
        <translation>Detect image edges, supports Canny, Sobel, Laplacian, Scharr algorithms</translation>
    </message>

    <!-- 形态学处理 -->
    <message>
        <source>形态学处理</source>
        <translation>Morphology</translation>
    </message>
    <message>
        <source>形态学操作，支持腐蚀、膨胀、开闭运算、梯度、顶帽、黑帽</source>
        <translation>Morphological operations: erode, dilate, open, close, gradient, top-hat, black-hat</translation>
    </message>

    <!-- ROI区域 -->
    <message>
        <source>ROI区域</source>
        <translation>ROI Region</translation>
    </message>
    <message>
        <source>定义感兴趣区域，支持矩形、圆形、椭圆、多边形、旋转矩形</source>
        <translation>Define region of interest, supports rectangle, circle, ellipse, polygon, rotated rect</translation>
    </message>

    <!-- Blob分析 -->
    <message>
        <source>Blob分析</source>
        <translation>Blob Analysis</translation>
    </message>
    <message>
        <source>连通域分析，提取面积、周长、圆度等特征，支持Halcon/OpenCV双后端</source>
        <translation>Connected component analysis, extract area, perimeter, circularity features</translation>
    </message>

    <!-- 圆检测 -->
    <message>
        <source>圆检测</source>
        <translation>Circle Detection</translation>
    </message>
    <message>
        <source>检测图像中的圆形，支持霍夫变换、轮廓拟合、边缘拟合等方法</source>
        <translation>Detect circles in image, supports Hough transform, contour fitting, edge fitting</translation>
    </message>

    <!-- 线检测 -->
    <message>
        <source>线检测</source>
        <translation>Line Detection</translation>
    </message>
    <message>
        <source>检测图像中的直线，支持霍夫变换、轮廓拟合等方法</source>
        <translation>Detect lines in image, supports Hough transform, contour fitting</translation>
    </message>

    <!-- 形状匹配 -->
    <message>
        <source>形状匹配</source>
        <translation>Shape Matching</translation>
    </message>
    <message>
        <source>使用Halcon形状匹配算法查找图像中的目标，支持角度、缩放变化</source>
        <translation>Find targets using Halcon shape matching, supports angle and scale variation</translation>
    </message>

    <!-- 相机标定 -->
    <message>
        <source>相机标定</source>
        <translation>Camera Calibration</translation>
    </message>
    <message>
        <source>使用棋盘格/圆点阵进行相机内参标定，计算畸变校正参数</source>
        <translation>Camera intrinsic calibration using checkerboard/circle grid, compute distortion correction</translation>
    </message>

    <!-- 九点标定 -->
    <message>
        <source>九点标定</source>
        <translation>9-Point Calibration</translation>
    </message>
    <message>
        <source>图像坐标与物理坐标映射标定，支持仿射/透视变换</source>
        <translation>Image to physical coordinate mapping, supports affine/perspective transform</translation>
    </message>

    <!-- 距离测量 -->
    <message>
        <source>距离测量</source>
        <translation>Distance Measurement</translation>
    </message>
    <message>
        <source>测量两点、点到线、线到线等距离，支持像素/毫米换算</source>
        <translation>Measure point-to-point, point-to-line, line-to-line distance, pixel/mm conversion</translation>
    </message>

    <!-- 角度测量 -->
    <message>
        <source>角度测量</source>
        <translation>Angle Measurement</translation>
    </message>
    <message>
        <source>测量两线夹角、三点角度、线与水平/垂直方向角度</source>
        <translation>Measure angle between lines, 3-point angle, angle to horizontal/vertical</translation>
    </message>

    <!-- 面积测量 -->
    <message>
        <source>面积测量</source>
        <translation>Area Measurement</translation>
    </message>
    <message>
        <source>测量轮廓、多边形、圆形、椭圆的面积，支持像素/毫米换算</source>
        <translation>Measure area of contour, polygon, circle, ellipse, pixel/mm conversion</translation>
    </message>

    <!-- 范围判定 -->
    <message>
        <source>范围判定</source>
        <translation>Range Judgment</translation>
    </message>
    <message>
        <source>判断测量值是否在指定范围内，输出OK/NG结果</source>
        <translation>Judge if measurement is within specified range, output OK/NG result</translation>
    </message>

    <!-- 图像保存 -->
    <message>
        <source>图像保存</source>
        <translation>Save Image</translation>
    </message>
    <message>
        <source>保存图像到文件，支持多种格式和命名规则</source>
        <translation>Save image to file, supports multiple formats and naming rules</translation>
    </message>

    <!-- PLC输出 -->
    <message>
        <source>PLC输出</source>
        <translation>PLC Output</translation>
    </message>
    <message>
        <source>将检测结果输出到PLC，支持位/字输出和地址映射</source>
        <translation>Output detection results to PLC, supports bit/word output and address mapping</translation>
    </message>

    <!-- 逻辑运算 -->
    <message>
        <source>逻辑运算</source>
        <translation>Logic Operation</translation>
    </message>
    <message>
        <source>对多个检测结果进行逻辑运算(AND/OR/NOT/XOR等)</source>
        <translation>Perform logic operations on multiple results (AND/OR/NOT/XOR etc.)</translation>
    </message>

    <!-- 模板匹配 -->
    <message>
        <source>模板匹配</source>
        <translation>Template Matching</translation>
    </message>
    <message>
        <source>使用OpenCV模板匹配算法查找目标，支持金字塔加速和角度搜索</source>
        <translation>Find targets using OpenCV template matching, supports pyramid and angle search</translation>
    </message>

    <!-- 颜色转换 -->
    <message>
        <source>颜色转换</source>
        <translation>Color Convert</translation>
    </message>
    <message>
        <source>在不同颜色空间之间转换，支持BGR/RGB/HSV/Lab等格式</source>
        <translation>Convert between color spaces, supports BGR/RGB/HSV/Lab formats</translation>
    </message>

    <!-- 边缘查找 -->
    <message>
        <source>边缘查找</source>
        <translation>Find Edge</translation>
    </message>
    <message>
        <source>沿指定方向查找边缘点，支持线搜索和矩形搜索模式</source>
        <translation>Find edge points along specified direction, supports line and rectangle search</translation>
    </message>

    <!-- 中心计算 -->
    <message>
        <source>中心计算</source>
        <translation>Center Calculation</translation>
    </message>
    <message>
        <source>计算目标中心点，支持质心、几何中心、最小包围圆等方法</source>
        <translation>Calculate target center, supports centroid, geometric center, min enclosing circle</translation>
    </message>

    <!-- 方向计算 -->
    <message>
        <source>方向计算</source>
        <translation>Orientation Calculation</translation>
    </message>
    <message>
        <source>计算目标方向角度，支持PCA、最小外接矩形、椭圆拟合等方法</source>
        <translation>Calculate target orientation, supports PCA, min bounding rect, ellipse fitting</translation>
    </message>

    <!-- AI检测 -->
    <message>
        <source>AI检测</source>
        <translation>AI Detection</translation>
    </message>
    <message>
        <source>基于深度学习的目标检测、分类、缺陷检测，支持ONNX/TensorRT推理</source>
        <translation>Deep learning based detection, classification, defect inspection, ONNX/TensorRT inference</translation>
    </message>

    <!-- 其他 -->
    <message>
        <source>已注册 %1 个内置工具</source>
        <translation>Registered %1 built-in tools</translation>
    </message>
</context>

<!-- TranslationManager 翻译 -->
<context>
    <name>TranslationManager</name>
    <message>
        <source>简体中文</source>
        <translation>Simplified Chinese</translation>
    </message>
    <message>
        <source>跟随系统</source>
        <translation>Follow System</translation>
    </message>
</context>

<!-- MainWindow 主窗口翻译 -->
<context>
    <name>MainWindow</name>
    <message>
        <source>VisionForge Pro - 工业视觉检测系统</source>
        <translation>VisionForge Pro - Industrial Vision Inspection System</translation>
    </message>
    <message>
        <source>文件</source>
        <translation>File</translation>
    </message>
    <message>
        <source>编辑</source>
        <translation>Edit</translation>
    </message>
    <message>
        <source>视图</source>
        <translation>View</translation>
    </message>
    <message>
        <source>运行</source>
        <translation>Run</translation>
    </message>
    <message>
        <source>工具</source>
        <translation>Tools</translation>
    </message>
    <message>
        <source>设置</source>
        <translation>Settings</translation>
    </message>
    <message>
        <source>帮助</source>
        <translation>Help</translation>
    </message>
    <message>
        <source>新建项目</source>
        <translation>New Project</translation>
    </message>
    <message>
        <source>打开项目</source>
        <translation>Open Project</translation>
    </message>
    <message>
        <source>保存项目</source>
        <translation>Save Project</translation>
    </message>
    <message>
        <source>退出</source>
        <translation>Exit</translation>
    </message>
    <message>
        <source>撤销</source>
        <translation>Undo</translation>
    </message>
    <message>
        <source>重做</source>
        <translation>Redo</translation>
    </message>
    <message>
        <source>开始运行</source>
        <translation>Start</translation>
    </message>
    <message>
        <source>停止运行</source>
        <translation>Stop</translation>
    </message>
    <message>
        <source>单步执行</source>
        <translation>Step</translation>
    </message>
    <message>
        <source>相机管理</source>
        <translation>Camera Manager</translation>
    </message>
    <message>
        <source>通讯设置</source>
        <translation>Communication Settings</translation>
    </message>
    <message>
        <source>系统设置</source>
        <translation>System Settings</translation>
    </message>
    <message>
        <source>关于</source>
        <translation>About</translation>
    </message>
    <message>
        <source>就绪</source>
        <translation>Ready</translation>
    </message>
    <message>
        <source>工具箱</source>
        <translation>Toolbox</translation>
    </message>
    <message>
        <source>属性</source>
        <translation>Properties</translation>
    </message>
    <message>
        <source>流程</source>
        <translation>Flow</translation>
    </message>
    <message>
        <source>日志</source>
        <translation>Log</translation>
    </message>
</context>

<!-- 通用对话框按钮 -->
<context>
    <name>Common</name>
    <message>
        <source>确定</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>取消</source>
        <translation>Cancel</translation>
    </message>
    <message>
        <source>应用</source>
        <translation>Apply</translation>
    </message>
    <message>
        <source>是</source>
        <translation>Yes</translation>
    </message>
    <message>
        <source>否</source>
        <translation>No</translation>
    </message>
    <message>
        <source>确认</source>
        <translation>Confirm</translation>
    </message>
    <message>
        <source>警告</source>
        <translation>Warning</translation>
    </message>
    <message>
        <source>错误</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>信息</source>
        <translation>Information</translation>
    </message>
</context>

</TS>
