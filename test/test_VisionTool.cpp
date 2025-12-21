/**
 * @file test_VisionTool.cpp
 * @brief VisionTool类单元测试
 */

#include <QtTest/QtTest>
#include "algorithm/VisionTool.h"
#include "algorithm/ToolFactory.h"
#include "base/ImageMemoryPool.h"
#include "TestUtils.h"

using namespace VisionForge;
using namespace VisionForge::Test;

class TestVisionTool : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 工具工厂测试
    void testToolFactoryCreate();
    void testToolFactoryRegisteredTypes();

    // 工具基本属性测试
    void testToolName();
    void testToolEnabled();

    // 工具执行测试
    void testToolProcess();
    void testToolProcessWithEmptyInput();

    // 工具序列化测试
    void testToolSerialization();

private:
    cv::Mat testImage_;
};

void TestVisionTool::initTestCase()
{
    testImage_ = TestUtils::createGrayImage(640, 480, 128);
}

void TestVisionTool::cleanupTestCase()
{
    testImage_.release();
}

void TestVisionTool::testToolFactoryCreate()
{
    auto& factory = Algorithm::ToolFactory::instance();
    auto tools = factory.getAllTools();

    if (tools.isEmpty()) {
        QSKIP("No tools registered");
    }

    // 使用第一个已注册的工具进行测试
    auto tool = factory.createTool(tools.first().type);
    QVERIFY(tool != nullptr);
    delete tool;
}

void TestVisionTool::testToolFactoryRegisteredTypes()
{
    auto& factory = Algorithm::ToolFactory::instance();
    auto tools = factory.getAllTools();

    // 应该有一些已注册的工具
    QVERIFY(tools.size() > 0);
}

void TestVisionTool::testToolName()
{
    auto& factory = Algorithm::ToolFactory::instance();
    auto tools = factory.getAllTools();

    if (tools.isEmpty()) {
        QSKIP("No tools registered");
    }

    // 使用第一个已注册的工具进行测试
    auto tool = factory.createTool(tools.first().type);
    if (!tool) {
        QSKIP("Could not create tool");
    }

    // 检查工具名称
    QVERIFY(!tool->toolName().isEmpty());
    QVERIFY(!tool->displayName().isEmpty());

    delete tool;
}

void TestVisionTool::testToolEnabled()
{
    auto& factory = Algorithm::ToolFactory::instance();
    auto tools = factory.getAllTools();

    if (tools.isEmpty()) {
        QSKIP("No tools registered");
    }

    auto tool = factory.createTool(tools.first().type);
    if (!tool) {
        QSKIP("Could not create tool");
    }

    // 默认应该启用
    QVERIFY(tool->isEnabled());

    // 禁用工具
    tool->setEnabled(false);
    QVERIFY(!tool->isEnabled());

    // 重新启用
    tool->setEnabled(true);
    QVERIFY(tool->isEnabled());

    delete tool;
}

void TestVisionTool::testToolProcess()
{
    auto& factory = Algorithm::ToolFactory::instance();
    auto tools = factory.getAllTools();

    if (tools.isEmpty()) {
        QSKIP("No tools registered");
    }

    auto tool = factory.createTool(tools.first().type);
    if (!tool) {
        QSKIP("Could not create tool");
    }

    // 创建测试图像
    cv::Mat colorImage = TestUtils::createColorImage(640, 480, 100, 150, 200);
    auto input = Base::ImageMemoryPool::instance().allocate(
        colorImage.cols, colorImage.rows, colorImage.type());
    if (input) {
        colorImage.copyTo(input->mat());
    }

    // 执行工具
    Algorithm::ToolResult result;
    bool success = tool->process(input, result);

    // 结果可能成功也可能失败（取决于工具类型和输入），但不应崩溃
    Q_UNUSED(success);

    delete tool;
}

void TestVisionTool::testToolProcessWithEmptyInput()
{
    auto& factory = Algorithm::ToolFactory::instance();
    auto tools = factory.getAllTools();

    if (tools.isEmpty()) {
        QSKIP("No tools registered");
    }

    auto tool = factory.createTool(tools.first().type);
    if (!tool) {
        QSKIP("Could not create tool");
    }

    // 空输入 - 使用默认构造的ImageData
    auto emptyInput = std::make_shared<Base::ImageData>();

    // 执行应该失败但不崩溃
    Algorithm::ToolResult result;
    bool success = tool->process(emptyInput, result);
    QVERIFY(!success);

    delete tool;
}

void TestVisionTool::testToolSerialization()
{
    auto& factory = Algorithm::ToolFactory::instance();
    auto tools = factory.getAllTools();

    if (tools.isEmpty()) {
        QSKIP("No tools registered");
    }

    auto tool = factory.createTool(tools.first().type);
    if (!tool) {
        QSKIP("Could not create tool");
    }

    // 序列化
    QJsonObject json = tool->serializeParams();
    // JSON可能为空也可能不为空，取决于工具

    // 反序列化（不应崩溃）
    tool->deserializeParams(json);

    delete tool;
}

QTEST_MAIN(TestVisionTool)
#include "test_VisionTool.moc"
