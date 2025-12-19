/**
 * @file test_ToolChain.cpp
 * @brief ToolChain类单元测试
 */

#include <QtTest/QtTest>
#include "core/ToolChain.h"
#include "algorithm/VisionTool.h"
#include "algorithm/ToolFactory.h"
#include "base/ImageData.h"
#include "TestUtils.h"

using namespace VisionForge;
using namespace VisionForge::Test;

/**
 * @brief 测试用的简单工具
 */
class MockVisionTool : public Algorithm::VisionTool
{
    Q_OBJECT
public:
    explicit MockVisionTool(QObject* parent = nullptr)
        : Algorithm::VisionTool(parent)
        , processResult_(true)
        , processCount_(0)
    {}

    ToolType toolType() const override { return ToolType::Custom; }
    QString toolName() const override { return "MockTool"; }

    bool process(const Base::ImageData::Ptr& input, Algorithm::ToolResult& result) override
    {
        processCount_++;

        if (!input || input->isEmpty()) {
            result.success = false;
            result.errorMessage = "Empty input";
            return false;
        }

        result.success = processResult_;
        result.outputImage = input;  // 直接传递

        if (!processResult_) {
            result.errorMessage = "Mock failure";
        }

        return processResult_;
    }

    QJsonObject serializeParams() const override
    {
        QJsonObject json;
        json["mockParam"] = mockParam_;
        return json;
    }

    void deserializeParams(const QJsonObject& json) override
    {
        mockParam_ = json["mockParam"].toString();
    }

    void setProcessResult(bool result) { processResult_ = result; }
    int processCount() const { return processCount_; }
    void setMockParam(const QString& param) { mockParam_ = param; }
    QString mockParam() const { return mockParam_; }

private:
    bool processResult_;
    int processCount_;
    QString mockParam_;
};

class TestToolChain : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    // 工具链管理测试
    void testAddTool();
    void testInsertTool();
    void testRemoveTool();
    void testRemoveToolAt();
    void testMoveTool();
    void testClear();
    void testToolCount();
    void testGetTool();
    void testGetNode();
    void testFindNodeIndex();

    // 节点属性测试
    void testSetNodeEnabled();
    void testSetNodeName();
    void testNodeId();

    // 工具链执行测试
    void testExecute();
    void testExecuteEmpty();
    void testExecuteWithDisabledTool();
    void testExecuteUntil();
    void testExecuteSingle();
    void testExecuteFailure();
    void testExecutionTime();

    // 序列化测试
    void testSerialize();
    void testDeserialize();
    void testSaveToFile();
    void testLoadFromFile();

    // 属性测试
    void testName();
    void testDescription();

    // 信号测试
    void testStructureChangedSignal();
    void testToolExecutionSignals();
    void testExecutionProgressSignal();
    void testNameChangedSignal();

private:
    cv::Mat testImage_;
    Base::ImageData::Ptr testImageData_;
};

void TestToolChain::initTestCase()
{
    testImage_ = TestUtils::createGrayImage(640, 480, 128);
    testImageData_ = std::make_shared<Base::ImageData>(testImage_);
}

void TestToolChain::cleanupTestCase()
{
    testImage_.release();
    testImageData_.reset();
}

void TestToolChain::cleanup()
{
}

// ========== 工具链管理测试 ==========

void TestToolChain::testAddTool()
{
    Core::ToolChain chain;

    QString id1 = chain.addTool(new MockVisionTool(), "Tool 1");
    QString id2 = chain.addTool(new MockVisionTool(), "Tool 2");

    QCOMPARE(chain.toolCount(), 2);
    QVERIFY(!id1.isEmpty());
    QVERIFY(!id2.isEmpty());
    QVERIFY(id1 != id2);
}

void TestToolChain::testInsertTool()
{
    Core::ToolChain chain;

    chain.addTool(new MockVisionTool(), "First");
    chain.addTool(new MockVisionTool(), "Last");

    QString middleId = chain.insertTool(1, new MockVisionTool(), "Middle");

    QCOMPARE(chain.toolCount(), 3);

    const Core::ToolChainNode* node = chain.getNode(1);
    QVERIFY(node != nullptr);
    QCOMPARE(node->name, QString("Middle"));
    QCOMPARE(node->id, middleId);
}

void TestToolChain::testRemoveTool()
{
    Core::ToolChain chain;

    QString id1 = chain.addTool(new MockVisionTool(), "Tool 1");
    chain.addTool(new MockVisionTool(), "Tool 2");

    QCOMPARE(chain.toolCount(), 2);

    bool removed = chain.removeTool(id1);
    QVERIFY(removed);
    QCOMPARE(chain.toolCount(), 1);

    // 移除不存在的
    removed = chain.removeTool("nonexistent");
    QVERIFY(!removed);
}

void TestToolChain::testRemoveToolAt()
{
    Core::ToolChain chain;

    chain.addTool(new MockVisionTool(), "Tool 1");
    chain.addTool(new MockVisionTool(), "Tool 2");
    chain.addTool(new MockVisionTool(), "Tool 3");

    bool removed = chain.removeToolAt(1);
    QVERIFY(removed);
    QCOMPARE(chain.toolCount(), 2);

    // 索引越界
    removed = chain.removeToolAt(99);
    QVERIFY(!removed);

    removed = chain.removeToolAt(-1);
    QVERIFY(!removed);
}

void TestToolChain::testMoveTool()
{
    Core::ToolChain chain;

    chain.addTool(new MockVisionTool(), "A");
    chain.addTool(new MockVisionTool(), "B");
    chain.addTool(new MockVisionTool(), "C");

    // A, B, C -> B, A, C
    bool moved = chain.moveTool(0, 1);
    QVERIFY(moved);

    QCOMPARE(chain.getNode(0)->name, QString("B"));
    QCOMPARE(chain.getNode(1)->name, QString("A"));
    QCOMPARE(chain.getNode(2)->name, QString("C"));
}

void TestToolChain::testClear()
{
    Core::ToolChain chain;

    chain.addTool(new MockVisionTool(), "Tool 1");
    chain.addTool(new MockVisionTool(), "Tool 2");

    QCOMPARE(chain.toolCount(), 2);

    chain.clear();

    QCOMPARE(chain.toolCount(), 0);
}

void TestToolChain::testToolCount()
{
    Core::ToolChain chain;

    QCOMPARE(chain.toolCount(), 0);

    chain.addTool(new MockVisionTool(), "1");
    QCOMPARE(chain.toolCount(), 1);

    chain.addTool(new MockVisionTool(), "2");
    QCOMPARE(chain.toolCount(), 2);

    chain.removeToolAt(0);
    QCOMPARE(chain.toolCount(), 1);
}

void TestToolChain::testGetTool()
{
    Core::ToolChain chain;

    MockVisionTool* tool = new MockVisionTool();
    QString id = chain.addTool(tool, "Test");

    // 按索引获取
    QCOMPARE(chain.getTool(0), tool);

    // 按ID获取
    QCOMPARE(chain.getTool(id), tool);

    // 无效索引
    QVERIFY(chain.getTool(99) == nullptr);

    // 无效ID
    QVERIFY(chain.getTool("invalid") == nullptr);
}

void TestToolChain::testGetNode()
{
    Core::ToolChain chain;

    chain.addTool(new MockVisionTool(), "TestNode");

    const Core::ToolChainNode* node = chain.getNode(0);
    QVERIFY(node != nullptr);
    QCOMPARE(node->name, QString("TestNode"));
    QVERIFY(node->enabled);

    // 无效索引
    QVERIFY(chain.getNode(99) == nullptr);
}

void TestToolChain::testFindNodeIndex()
{
    Core::ToolChain chain;

    QString id1 = chain.addTool(new MockVisionTool(), "Tool 1");
    QString id2 = chain.addTool(new MockVisionTool(), "Tool 2");

    QCOMPARE(chain.findNodeIndex(id1), 0);
    QCOMPARE(chain.findNodeIndex(id2), 1);
    QCOMPARE(chain.findNodeIndex("invalid"), -1);
}

// ========== 节点属性测试 ==========

void TestToolChain::testSetNodeEnabled()
{
    Core::ToolChain chain;

    QString id = chain.addTool(new MockVisionTool(), "Tool");

    QVERIFY(chain.getNode(0)->enabled);

    chain.setNodeEnabled(id, false);
    QVERIFY(!chain.getNode(0)->enabled);

    chain.setNodeEnabled(id, true);
    QVERIFY(chain.getNode(0)->enabled);
}

void TestToolChain::testSetNodeName()
{
    Core::ToolChain chain;

    QString id = chain.addTool(new MockVisionTool(), "Original");

    chain.setNodeName(id, "NewName");
    QCOMPARE(chain.getNode(0)->name, QString("NewName"));
}

void TestToolChain::testNodeId()
{
    Core::ToolChain chain;

    QString id = chain.addTool(new MockVisionTool(), "Tool");

    QCOMPARE(chain.getNodeId(0), id);
    QVERIFY(chain.getNodeId(99).isEmpty());
}

// ========== 工具链执行测试 ==========

void TestToolChain::testExecute()
{
    Core::ToolChain chain;

    MockVisionTool* tool1 = new MockVisionTool();
    MockVisionTool* tool2 = new MockVisionTool();

    chain.addTool(tool1, "Tool 1");
    chain.addTool(tool2, "Tool 2");

    Core::ToolChainResult result;
    bool success = chain.execute(testImageData_, result);

    QVERIFY(success);
    QVERIFY(result.success);
    QCOMPARE(result.failedToolIndex, -1);
    QCOMPARE(result.toolResults.size(), 2);
    QCOMPARE(tool1->processCount(), 1);
    QCOMPARE(tool2->processCount(), 1);
}

void TestToolChain::testExecuteEmpty()
{
    Core::ToolChain chain;

    Core::ToolChainResult result;
    bool success = chain.execute(testImageData_, result);

    // 空链执行应该成功（无操作）
    QVERIFY(success);
    QVERIFY(result.success);
}

void TestToolChain::testExecuteWithDisabledTool()
{
    Core::ToolChain chain;

    MockVisionTool* tool1 = new MockVisionTool();
    MockVisionTool* tool2 = new MockVisionTool();
    MockVisionTool* tool3 = new MockVisionTool();

    QString id1 = chain.addTool(tool1, "Tool 1");
    QString id2 = chain.addTool(tool2, "Tool 2");
    QString id3 = chain.addTool(tool3, "Tool 3");
    Q_UNUSED(id1);
    Q_UNUSED(id3);

    // 禁用中间的工具
    chain.setNodeEnabled(id2, false);

    Core::ToolChainResult result;
    chain.execute(testImageData_, result);

    QCOMPARE(tool1->processCount(), 1);
    QCOMPARE(tool2->processCount(), 0);  // 被禁用，不执行
    QCOMPARE(tool3->processCount(), 1);
}

void TestToolChain::testExecuteUntil()
{
    Core::ToolChain chain;

    MockVisionTool* tool1 = new MockVisionTool();
    MockVisionTool* tool2 = new MockVisionTool();
    MockVisionTool* tool3 = new MockVisionTool();

    chain.addTool(tool1, "Tool 1");
    chain.addTool(tool2, "Tool 2");
    chain.addTool(tool3, "Tool 3");

    Core::ToolChainResult result;
    bool success = chain.executeUntil(testImageData_, 1, result);

    QVERIFY(success);
    QCOMPARE(tool1->processCount(), 1);
    QCOMPARE(tool2->processCount(), 1);
    QCOMPARE(tool3->processCount(), 0);  // 不执行
}

void TestToolChain::testExecuteSingle()
{
    Core::ToolChain chain;

    MockVisionTool* tool1 = new MockVisionTool();
    MockVisionTool* tool2 = new MockVisionTool();

    chain.addTool(tool1, "Tool 1");
    chain.addTool(tool2, "Tool 2");

    Algorithm::ToolResult result;
    bool success = chain.executeSingle(testImageData_, 1, result);

    QVERIFY(success);
    QCOMPARE(tool1->processCount(), 0);
    QCOMPARE(tool2->processCount(), 1);
}

void TestToolChain::testExecuteFailure()
{
    Core::ToolChain chain;

    MockVisionTool* tool1 = new MockVisionTool();
    MockVisionTool* tool2 = new MockVisionTool();
    MockVisionTool* tool3 = new MockVisionTool();

    tool2->setProcessResult(false);  // 第二个工具会失败

    chain.addTool(tool1, "Tool 1");
    chain.addTool(tool2, "Tool 2");
    chain.addTool(tool3, "Tool 3");

    Core::ToolChainResult result;
    bool success = chain.execute(testImageData_, result);

    QVERIFY(!success);
    QVERIFY(!result.success);
    QCOMPARE(result.failedToolIndex, 1);
    QCOMPARE(tool1->processCount(), 1);
    QCOMPARE(tool2->processCount(), 1);
    QCOMPARE(tool3->processCount(), 0);  // 失败后不继续
}

void TestToolChain::testExecutionTime()
{
    Core::ToolChain chain;

    chain.addTool(new MockVisionTool(), "Tool 1");
    chain.addTool(new MockVisionTool(), "Tool 2");

    Core::ToolChainResult result;
    chain.execute(testImageData_, result);

    QVERIFY(result.totalExecutionTime >= 0);
}

// ========== 序列化测试 ==========

void TestToolChain::testSerialize()
{
    Core::ToolChain chain;
    chain.setName("TestChain");
    chain.setDescription("Test Description");

    MockVisionTool* tool = new MockVisionTool();
    tool->setMockParam("testValue");
    chain.addTool(tool, "MockTool");

    QJsonObject json = chain.serialize();

    QCOMPARE(json["name"].toString(), QString("TestChain"));
    QCOMPARE(json["description"].toString(), QString("Test Description"));
    QVERIFY(json.contains("tools"));
}

void TestToolChain::testDeserialize()
{
    // 首先创建并序列化
    Core::ToolChain chain1;
    chain1.setName("OriginalChain");
    chain1.setDescription("Original Description");

    QJsonObject json = chain1.serialize();

    // 反序列化到新链
    Core::ToolChain chain2;
    bool success = chain2.deserialize(json);

    QVERIFY(success);
    QCOMPARE(chain2.name(), QString("OriginalChain"));
    QCOMPARE(chain2.description(), QString("Original Description"));
}

void TestToolChain::testSaveToFile()
{
    Test::ScopedTempDir tempDir;
    QVERIFY(tempDir.isValid());

    QString filePath = tempDir.path() + "/test_chain.json";

    Core::ToolChain chain;
    chain.setName("SaveTest");
    chain.addTool(new MockVisionTool(), "Tool");

    bool saved = chain.saveToFile(filePath);
    QVERIFY(saved);

    QFile file(filePath);
    QVERIFY(file.exists());
}

void TestToolChain::testLoadFromFile()
{
    Test::ScopedTempDir tempDir;
    QVERIFY(tempDir.isValid());

    QString filePath = tempDir.path() + "/test_chain.json";

    // 保存
    Core::ToolChain chain1;
    chain1.setName("LoadTest");
    chain1.saveToFile(filePath);

    // 加载
    Core::ToolChain chain2;
    bool loaded = chain2.loadFromFile(filePath);

    QVERIFY(loaded);
    QCOMPARE(chain2.name(), QString("LoadTest"));

    // 加载不存在的文件
    loaded = chain2.loadFromFile("/nonexistent/path.json");
    QVERIFY(!loaded);
}

// ========== 属性测试 ==========

void TestToolChain::testName()
{
    Core::ToolChain chain;

    QVERIFY(chain.name().isEmpty());

    chain.setName("MyChain");
    QCOMPARE(chain.name(), QString("MyChain"));
}

void TestToolChain::testDescription()
{
    Core::ToolChain chain;

    QVERIFY(chain.description().isEmpty());

    chain.setDescription("My Description");
    QCOMPARE(chain.description(), QString("My Description"));
}

// ========== 信号测试 ==========

void TestToolChain::testStructureChangedSignal()
{
    Core::ToolChain chain;

    QSignalSpy spy(&chain, &Core::ToolChain::structureChanged);

    chain.addTool(new MockVisionTool(), "Tool");
    QCOMPARE(spy.count(), 1);

    chain.removeToolAt(0);
    QCOMPARE(spy.count(), 2);

    chain.addTool(new MockVisionTool(), "Tool");
    chain.clear();
    QCOMPARE(spy.count(), 4);
}

void TestToolChain::testToolExecutionSignals()
{
    Core::ToolChain chain;

    QSignalSpy startSpy(&chain, &Core::ToolChain::toolExecutionStarted);
    QSignalSpy finishSpy(&chain, &Core::ToolChain::toolExecutionFinished);

    chain.addTool(new MockVisionTool(), "Tool 1");
    chain.addTool(new MockVisionTool(), "Tool 2");

    Core::ToolChainResult result;
    chain.execute(testImageData_, result);

    QCOMPARE(startSpy.count(), 2);
    QCOMPARE(finishSpy.count(), 2);
}

void TestToolChain::testExecutionProgressSignal()
{
    Core::ToolChain chain;

    QSignalSpy spy(&chain, &Core::ToolChain::executionProgress);

    chain.addTool(new MockVisionTool(), "Tool 1");
    chain.addTool(new MockVisionTool(), "Tool 2");
    chain.addTool(new MockVisionTool(), "Tool 3");

    Core::ToolChainResult result;
    chain.execute(testImageData_, result);

    QVERIFY(spy.count() >= 3);
}

void TestToolChain::testNameChangedSignal()
{
    Core::ToolChain chain;

    QSignalSpy spy(&chain, &Core::ToolChain::nameChanged);

    chain.setName("NewName");

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("NewName"));
}

QTEST_MAIN(TestToolChain)
#include "test_ToolChain.moc"
