/**
 * @file OCREngine.h
 * @brief 工业级OCR文字识别引擎
 *
 * 支持多种OCR后端：
 * - PaddleOCR / PPOCR
 * - Tesseract (可选)
 * - 自定义ONNX模型
 *
 * 功能特点：
 * - 文本检测（DBNet）
 * - 文本识别（CRNN/SVTR）
 * - 方向分类
 * - 多语言支持（中英日韩）
 *
 * @author VisionForge Pro Team
 * @version 1.0.0
 */

#pragma once

#include "InferenceEngine.h"
#include <QObject>
#include <QImage>
#include <QRectF>
#include <QPolygonF>
#include <opencv2/core.hpp>
#include <vector>
#include <memory>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// OCR后端类型
// ============================================================
enum class OCRBackend {
    PaddleOCR,      // PaddleOCR/PPOCR
    Tesseract,      // Tesseract OCR
    Custom          // 自定义模型
};

// ============================================================
// 语言类型
// ============================================================
enum class OCRLanguage {
    Chinese,        // 中文
    English,        // 英文
    Japanese,       // 日文
    Korean,         // 韩文
    ChineseEnglish, // 中英混合
    Auto            // 自动检测
};

// ============================================================
// 文本方向
// ============================================================
enum class TextDirection {
    Horizontal,     // 水平
    Vertical,       // 垂直
    Auto            // 自动检测
};

// ============================================================
// 单个文本框结果
// ============================================================
struct TextBox {
    // 文本内容
    QString text;
    float confidence = 0.0f;

    // 位置信息
    QPolygonF polygon;        // 四点多边形
    QRectF boundingBox;       // 轴对齐边界框
    float angle = 0.0f;       // 倾斜角度

    // 字符级信息
    struct CharInfo {
        QChar character;
        float confidence;
        QRectF boundingBox;
    };
    std::vector<CharInfo> characters;

    // 属性
    TextDirection direction = TextDirection::Horizontal;
    bool isVertical = false;

    // 辅助方法
    QPointF topLeft() const { return polygon.isEmpty() ? boundingBox.topLeft() : polygon[0]; }
    QPointF topRight() const { return polygon.size() > 1 ? polygon[1] : boundingBox.topRight(); }
    QPointF bottomRight() const { return polygon.size() > 2 ? polygon[2] : boundingBox.bottomRight(); }
    QPointF bottomLeft() const { return polygon.size() > 3 ? polygon[3] : boundingBox.bottomLeft(); }
};

// ============================================================
// 文本检测结果
// ============================================================
struct TextDetectionResult {
    bool success = false;
    QString errorMessage;

    std::vector<QPolygonF> boxes;     // 检测到的文本框
    std::vector<float> scores;        // 置信度

    float preprocessTime = 0.0f;
    float inferenceTime = 0.0f;
    float postprocessTime = 0.0f;
};

// ============================================================
// OCR完整结果
// ============================================================
struct OCRResult {
    bool success = false;
    QString errorMessage;

    // 识别结果
    std::vector<TextBox> textBoxes;

    // 统计
    int boxCount = 0;
    QString fullText;             // 所有文本拼接
    float avgConfidence = 0.0f;

    // 性能统计
    float detectionTime = 0.0f;   // 检测时间
    float recognitionTime = 0.0f; // 识别时间
    float totalTime = 0.0f;

    // 辅助方法
    QStringList getTexts() const;
    std::vector<TextBox*> getBoxesByConfidence(float minConf);
    TextBox* findTextContaining(const QString& substring);
};

// ============================================================
// OCR配置
// ============================================================
struct OCRConfig {
    // 后端设置
    OCRBackend backend = OCRBackend::PaddleOCR;
    OCRLanguage language = OCRLanguage::ChineseEnglish;

    // 模型路径
    QString detectionModelPath;     // 检测模型
    QString recognitionModelPath;   // 识别模型
    QString classifierModelPath;    // 方向分类模型（可选）
    QString dictionaryPath;         // 字典文件路径

    // 检测参数
    float detectionThreshold = 0.3f;
    float boxThreshold = 0.5f;
    float unclipRatio = 1.6f;       // 文本框扩展比例
    int maxCandidates = 1000;
    int minBoxSize = 3;             // 最小文本框尺寸

    // 识别参数
    float recognitionThreshold = 0.5f;
    int maxTextLength = 100;
    bool useAngleClassifier = false;

    // 输入设置
    int detInputHeight = 960;       // 检测模型输入高度
    int recInputHeight = 48;        // 识别模型输入高度
    int recInputWidth = 320;        // 识别模型输入宽度

    // 后处理
    bool sortByPosition = true;     // 按位置排序
    bool mergeBoxes = false;        // 合并相邻文本框
    float mergeThreshold = 0.5f;    // 合并阈值

    // 推理设置
    InferenceBackend inferenceBackend = InferenceBackend::ONNXRuntime;
    ExecutionDevice device = ExecutionDevice::Auto;

    // 序列化
    QJsonObject toJson() const;
    static OCRConfig fromJson(const QJsonObject& json);
};

// ============================================================
// OCR引擎
// ============================================================
class OCREngine : public QObject {
    Q_OBJECT

public:
    explicit OCREngine(QObject* parent = nullptr);
    ~OCREngine();

    // 初始化
    bool initialize(const OCRConfig& config);
    bool loadDetectionModel(const QString& modelPath);
    bool loadRecognitionModel(const QString& modelPath);
    bool loadClassifierModel(const QString& modelPath);
    bool loadDictionary(const QString& dictPath);
    void release();

    // OCR识别（检测+识别）
    OCRResult recognize(const cv::Mat& image);
    OCRResult recognize(const QImage& image);

    // 仅检测
    TextDetectionResult detect(const cv::Mat& image);

    // 仅识别（给定区域）
    QString recognizeRegion(const cv::Mat& image, const QPolygonF& region);
    QString recognizeRegion(const cv::Mat& image, const QRectF& rect);

    // 批量识别
    std::vector<OCRResult> recognizeBatch(const std::vector<cv::Mat>& images);

    // 可视化
    cv::Mat visualize(const cv::Mat& image, const OCRResult& result,
                      bool showText = true, bool showConfidence = true);

    // 配置
    void setConfig(const OCRConfig& config);
    OCRConfig getConfig() const { return m_config; }

    // 字典
    QStringList getDictionary() const { return m_dictionary; }
    int getDictionarySize() const { return m_dictionary.size(); }

    // 状态
    bool isInitialized() const { return m_initialized; }
    bool hasDetectionModel() const { return m_detectionEngine != nullptr; }
    bool hasRecognitionModel() const { return m_recognitionEngine != nullptr; }
    QString getLastError() const { return m_lastError; }

signals:
    void progressChanged(int percent);
    void recognitionComplete(const OCRResult& result);
    void errorOccurred(const QString& error);

private:
    // 检测相关
    cv::Mat preprocessDetection(const cv::Mat& image, float& ratio);
    std::vector<QPolygonF> postprocessDetection(const Tensor& output,
                                                 const cv::Size& originalSize,
                                                 float ratio);

    // 识别相关
    cv::Mat preprocessRecognition(const cv::Mat& image);
    QString postprocessRecognition(const Tensor& output);

    // 文本框处理
    cv::Mat cropTextRegion(const cv::Mat& image, const QPolygonF& polygon);
    std::vector<QPolygonF> sortTextBoxes(const std::vector<QPolygonF>& boxes);
    cv::Mat rotateTextRegion(const cv::Mat& region, float angle);

    // DBNet后处理
    std::vector<std::vector<cv::Point>> dbnetPostprocess(
        const cv::Mat& bitmap,
        const cv::Mat& pred,
        float boxThresh,
        float unclipRatio);
    cv::RotatedRect unclip(const std::vector<cv::Point>& box, float ratio);

    // CTC解码
    QString ctcDecode(const std::vector<int>& indices);

private:
    OCRConfig m_config;

    // 推理引擎
    std::unique_ptr<InferenceEngine> m_detectionEngine;
    std::unique_ptr<InferenceEngine> m_recognitionEngine;
    std::unique_ptr<InferenceEngine> m_classifierEngine;

    // 字典
    QStringList m_dictionary;

    bool m_initialized = false;
    QString m_lastError;
};

// ============================================================
// 表格OCR（扩展功能）
// ============================================================
class TableOCR : public QObject {
    Q_OBJECT

public:
    struct Cell {
        int row;
        int col;
        int rowSpan = 1;
        int colSpan = 1;
        QRectF boundingBox;
        QString text;
        float confidence;
    };

    struct TableResult {
        bool success = false;
        int rows = 0;
        int cols = 0;
        std::vector<Cell> cells;
        QRectF tableBoundingBox;

        QString getCell(int row, int col) const;
        QStringList getRow(int row) const;
        QStringList getColumn(int col) const;
    };

public:
    explicit TableOCR(QObject* parent = nullptr);
    ~TableOCR();

    bool initialize(const QString& structureModelPath,
                   const QString& ocrModelPath);
    TableResult recognize(const cv::Mat& image);
    void release();

private:
    std::unique_ptr<InferenceEngine> m_structureEngine;
    std::unique_ptr<OCREngine> m_ocrEngine;
    bool m_initialized = false;
};

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
