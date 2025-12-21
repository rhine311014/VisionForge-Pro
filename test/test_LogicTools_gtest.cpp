/**
 * @file test_LogicTools_gtest.cpp
 * @brief 逻辑判定工具类测试 (LogicOperation, RangeJudge)
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "algorithm/LogicOperationTool.h"
#include "algorithm/RangeJudgeTool.h"
#include "base/ImageData.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/opencv.hpp>

using namespace VisionForge::Algorithm;
using namespace VisionForge::Base;

// ============================================================
// 测试夹具
// ============================================================

class LogicToolsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建简单的测试图像
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(128));
    }

    ImageData::Ptr testImage;
};

// ============================================================
// LogicOperationTool 测试
// ============================================================

TEST_F(LogicToolsTest, LogicOperation_BasicSetup) {
    LogicOperationTool tool;

    // 测试初始状态
    EXPECT_EQ(tool.logicType(), LogicType::AND);
    EXPECT_EQ(tool.inputs().size(), 0);
}

TEST_F(LogicToolsTest, LogicOperation_SetLogicType) {
    LogicOperationTool tool;

    // 测试设置逻辑类型
    tool.setLogicType(LogicType::OR);
    EXPECT_EQ(tool.logicType(), LogicType::OR);

    tool.setLogicType(LogicType::XOR);
    EXPECT_EQ(tool.logicType(), LogicType::XOR);

    tool.setLogicType(LogicType::NOT);
    EXPECT_EQ(tool.logicType(), LogicType::NOT);
}

TEST_F(LogicToolsTest, LogicOperation_AddInput) {
    LogicOperationTool tool;

    // 添加输入
    LogicInput input1;
    input1.name = "Input1";
    input1.valueKey = "result";
    input1.enabled = true;

    tool.addInput(input1);
    EXPECT_EQ(tool.inputs().size(), 1);
    EXPECT_EQ(tool.inputs()[0].name.toStdString(), "Input1");

    // 添加更多输入
    LogicInput input2;
    input2.name = "Input2";
    input2.valueKey = "ok";

    tool.addInput(input2);
    EXPECT_EQ(tool.inputs().size(), 2);
}

TEST_F(LogicToolsTest, LogicOperation_RemoveInput) {
    LogicOperationTool tool;

    LogicInput input1, input2, input3;
    input1.name = "A";
    input2.name = "B";
    input3.name = "C";

    tool.addInput(input1);
    tool.addInput(input2);
    tool.addInput(input3);

    EXPECT_EQ(tool.inputs().size(), 3);

    // 移除中间的输入
    tool.removeInput(1);
    EXPECT_EQ(tool.inputs().size(), 2);
    EXPECT_EQ(tool.inputs()[0].name.toStdString(), "A");
    EXPECT_EQ(tool.inputs()[1].name.toStdString(), "C");
}

TEST_F(LogicToolsTest, LogicOperation_ClearInputs) {
    LogicOperationTool tool;

    LogicInput input;
    tool.addInput(input);
    tool.addInput(input);
    tool.addInput(input);

    EXPECT_EQ(tool.inputs().size(), 3);

    tool.clearInputs();
    EXPECT_EQ(tool.inputs().size(), 0);
}

TEST_F(LogicToolsTest, LogicOperation_SetInputValues) {
    LogicOperationTool tool;

    // 设置输入值
    QMap<QString, bool> values;
    values["input1"] = true;
    values["input2"] = false;
    values["input3"] = true;

    tool.setInputValues(values);

    // 创建对应的输入
    LogicInput input1, input2, input3;
    input1.name = "Input1";
    input1.valueKey = "input1";
    input2.name = "Input2";
    input2.valueKey = "input2";
    input3.name = "Input3";
    input3.valueKey = "input3";

    tool.addInput(input1);
    tool.addInput(input2);
    tool.addInput(input3);
}

TEST_F(LogicToolsTest, LogicOperation_AND_AllTrue) {
    LogicOperationTool tool;
    tool.setLogicType(LogicType::AND);

    // 设置三个都为true的输入
    QMap<QString, bool> values;
    values["a"] = true;
    values["b"] = true;
    values["c"] = true;
    tool.setInputValues(values);

    LogicInput input1, input2, input3;
    input1.valueKey = "a";
    input2.valueKey = "b";
    input3.valueKey = "c";
    tool.addInput(input1);
    tool.addInput(input2);
    tool.addInput(input3);

    ToolResult result;
    bool success = tool.process(nullptr, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(tool.result().result);
    EXPECT_EQ(tool.result().trueCount, 3);
    EXPECT_EQ(tool.result().falseCount, 0);
}

TEST_F(LogicToolsTest, LogicOperation_AND_OneFalse) {
    LogicOperationTool tool;
    tool.setLogicType(LogicType::AND);

    QMap<QString, bool> values;
    values["a"] = true;
    values["b"] = false; // 一个为false
    values["c"] = true;
    tool.setInputValues(values);

    LogicInput input1, input2, input3;
    input1.valueKey = "a";
    input2.valueKey = "b";
    input3.valueKey = "c";
    tool.addInput(input1);
    tool.addInput(input2);
    tool.addInput(input3);

    ToolResult result;
    tool.process(nullptr, result);

    // AND操作，一个为false则结果为false
    EXPECT_FALSE(tool.result().result);
    EXPECT_EQ(tool.result().trueCount, 2);
    EXPECT_EQ(tool.result().falseCount, 1);
}

TEST_F(LogicToolsTest, LogicOperation_OR_OneTrue) {
    LogicOperationTool tool;
    tool.setLogicType(LogicType::OR);

    QMap<QString, bool> values;
    values["a"] = false;
    values["b"] = true;  // 一个为true
    values["c"] = false;
    tool.setInputValues(values);

    LogicInput input1, input2, input3;
    input1.valueKey = "a";
    input2.valueKey = "b";
    input3.valueKey = "c";
    tool.addInput(input1);
    tool.addInput(input2);
    tool.addInput(input3);

    ToolResult result;
    tool.process(nullptr, result);

    // OR操作，一个为true则结果为true
    EXPECT_TRUE(tool.result().result);
}

TEST_F(LogicToolsTest, LogicOperation_OR_AllFalse) {
    LogicOperationTool tool;
    tool.setLogicType(LogicType::OR);

    QMap<QString, bool> values;
    values["a"] = false;
    values["b"] = false;
    values["c"] = false;
    tool.setInputValues(values);

    LogicInput input1, input2, input3;
    input1.valueKey = "a";
    input2.valueKey = "b";
    input3.valueKey = "c";
    tool.addInput(input1);
    tool.addInput(input2);
    tool.addInput(input3);

    ToolResult result;
    tool.process(nullptr, result);

    // OR操作，全为false则结果为false
    EXPECT_FALSE(tool.result().result);
}

TEST_F(LogicToolsTest, LogicOperation_NOT) {
    LogicOperationTool tool;
    tool.setLogicType(LogicType::NOT);

    QMap<QString, bool> values;
    values["a"] = true;
    tool.setInputValues(values);

    LogicInput input1;
    input1.valueKey = "a";
    tool.addInput(input1);

    ToolResult result;
    tool.process(nullptr, result);

    // NOT操作，true变false
    EXPECT_FALSE(tool.result().result);
}

TEST_F(LogicToolsTest, LogicOperation_XOR_Even) {
    LogicOperationTool tool;
    tool.setLogicType(LogicType::XOR);

    QMap<QString, bool> values;
    values["a"] = true;
    values["b"] = true; // 两个true
    tool.setInputValues(values);

    LogicInput input1, input2;
    input1.valueKey = "a";
    input2.valueKey = "b";
    tool.addInput(input1);
    tool.addInput(input2);

    ToolResult result;
    tool.process(nullptr, result);

    // XOR操作，偶数个true则为false
    EXPECT_FALSE(tool.result().result);
}

TEST_F(LogicToolsTest, LogicOperation_XOR_Odd) {
    LogicOperationTool tool;
    tool.setLogicType(LogicType::XOR);

    QMap<QString, bool> values;
    values["a"] = true;
    values["b"] = false;
    values["c"] = false; // 一个true（奇数）
    tool.setInputValues(values);

    LogicInput input1, input2, input3;
    input1.valueKey = "a";
    input2.valueKey = "b";
    input3.valueKey = "c";
    tool.addInput(input1);
    tool.addInput(input2);
    tool.addInput(input3);

    ToolResult result;
    tool.process(nullptr, result);

    // XOR操作，奇数个true则为true
    EXPECT_TRUE(tool.result().result);
}

TEST_F(LogicToolsTest, LogicOperation_InvertedInput) {
    LogicOperationTool tool;
    tool.setLogicType(LogicType::AND);

    QMap<QString, bool> values;
    values["a"] = true;
    values["b"] = false;
    tool.setInputValues(values);

    LogicInput input1, input2;
    input1.valueKey = "a";
    input2.valueKey = "b";
    input2.inverted = true; // 反转输入b

    tool.addInput(input1);
    tool.addInput(input2);

    ToolResult result;
    tool.process(nullptr, result);

    // input1=true, input2=!false=true, AND结果为true
    EXPECT_TRUE(tool.result().result);
}

TEST_F(LogicToolsTest, LogicOperation_Serialization) {
    LogicOperationTool tool;

    tool.setLogicType(LogicType::OR);

    LogicInput input1, input2;
    input1.name = "Test1";
    input1.valueKey = "key1";
    input2.name = "Test2";
    input2.valueKey = "key2";
    input2.inverted = true;

    tool.addInput(input1);
    tool.addInput(input2);

    // 序列化
    QJsonObject json = tool.serializeParams();
    EXPECT_FALSE(json.isEmpty());

    // 反序列化
    LogicOperationTool tool2;
    tool2.deserializeParams(json);

    EXPECT_EQ(tool2.logicType(), LogicType::OR);
    EXPECT_EQ(tool2.inputs().size(), 2);
}

// ============================================================
// RangeJudgeTool 测试
// ============================================================

TEST_F(LogicToolsTest, RangeJudge_BasicSetup) {
    RangeJudgeTool tool;

    // 测试初始状态
    EXPECT_EQ(tool.combineLogic(), RangeJudgeTool::LogicAND);
    EXPECT_EQ(tool.conditions().size(), 0);
}

TEST_F(LogicToolsTest, RangeJudge_SetCombineLogic) {
    RangeJudgeTool tool;

    tool.setCombineLogic(RangeJudgeTool::LogicOR);
    EXPECT_EQ(tool.combineLogic(), RangeJudgeTool::LogicOR);

    tool.setCombineLogic(RangeJudgeTool::LogicAND);
    EXPECT_EQ(tool.combineLogic(), RangeJudgeTool::LogicAND);
}

TEST_F(LogicToolsTest, RangeJudge_AddCondition) {
    RangeJudgeTool tool;

    JudgeCondition condition1("Diameter", 10.0, 20.0);
    tool.addCondition(condition1);

    EXPECT_EQ(tool.conditions().size(), 1);
    EXPECT_EQ(tool.conditions()[0].name.toStdString(), "Diameter");
    EXPECT_EQ(tool.conditions()[0].minValue, 10.0);
    EXPECT_EQ(tool.conditions()[0].maxValue, 20.0);

    // 添加更多条件
    JudgeCondition condition2("Area", 100.0, 500.0);
    tool.addCondition(condition2);

    EXPECT_EQ(tool.conditions().size(), 2);
}

TEST_F(LogicToolsTest, RangeJudge_RemoveCondition) {
    RangeJudgeTool tool;

    tool.addCondition(JudgeCondition("A", 0, 10));
    tool.addCondition(JudgeCondition("B", 10, 20));
    tool.addCondition(JudgeCondition("C", 20, 30));

    EXPECT_EQ(tool.conditions().size(), 3);

    tool.removeCondition(1);
    EXPECT_EQ(tool.conditions().size(), 2);
    EXPECT_EQ(tool.conditions()[0].name.toStdString(), "A");
    EXPECT_EQ(tool.conditions()[1].name.toStdString(), "C");
}

TEST_F(LogicToolsTest, RangeJudge_ClearConditions) {
    RangeJudgeTool tool;

    tool.addCondition(JudgeCondition("A", 0, 10));
    tool.addCondition(JudgeCondition("B", 10, 20));

    EXPECT_EQ(tool.conditions().size(), 2);

    tool.clearConditions();
    EXPECT_EQ(tool.conditions().size(), 0);
}

TEST_F(LogicToolsTest, RangeJudge_SetInputValues) {
    RangeJudgeTool tool;

    QMap<QString, double> values;
    values["diameter"] = 15.5;
    values["area"] = 250.0;

    tool.setInputValues(values);

    // 验证可以设置多个值
    tool.setInputValue("length", 100.0);
}

TEST_F(LogicToolsTest, RangeJudge_SingleCondition_Pass) {
    RangeJudgeTool tool;

    // 添加条件：diameter应该在10-20之间
    JudgeCondition condition("Diameter", 10.0, 20.0);
    condition.valueKey = "diameter";
    tool.addCondition(condition);

    // 设置满足条件的值
    tool.setInputValue("diameter", 15.0);

    ToolResult result;
    bool success = tool.process(nullptr, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(tool.result().overallResult); // 应该通过
    EXPECT_EQ(tool.result().passCount, 1);
    EXPECT_EQ(tool.result().failCount, 0);
}

TEST_F(LogicToolsTest, RangeJudge_SingleCondition_Fail) {
    RangeJudgeTool tool;

    JudgeCondition condition("Diameter", 10.0, 20.0);
    condition.valueKey = "diameter";
    tool.addCondition(condition);

    // 设置不满足条件的值（超出范围）
    tool.setInputValue("diameter", 25.0);

    ToolResult result;
    tool.process(nullptr, result);

    EXPECT_FALSE(tool.result().overallResult); // 应该失败
    EXPECT_EQ(tool.result().passCount, 0);
    EXPECT_EQ(tool.result().failCount, 1);
}

TEST_F(LogicToolsTest, RangeJudge_MultiCondition_AND_AllPass) {
    RangeJudgeTool tool;
    tool.setCombineLogic(RangeJudgeTool::LogicAND);

    // 添加两个条件
    JudgeCondition condition1("Diameter", 10.0, 20.0);
    condition1.valueKey = "diameter";
    tool.addCondition(condition1);

    JudgeCondition condition2("Area", 100.0, 500.0);
    condition2.valueKey = "area";
    tool.addCondition(condition2);

    // 设置都满足的值
    tool.setInputValue("diameter", 15.0);
    tool.setInputValue("area", 300.0);

    ToolResult result;
    tool.process(nullptr, result);

    // AND逻辑，全部通过则结果为OK
    EXPECT_TRUE(tool.result().overallResult);
    EXPECT_EQ(tool.result().passCount, 2);
    EXPECT_EQ(tool.result().failCount, 0);
}

TEST_F(LogicToolsTest, RangeJudge_MultiCondition_AND_OneFail) {
    RangeJudgeTool tool;
    tool.setCombineLogic(RangeJudgeTool::LogicAND);

    JudgeCondition condition1("Diameter", 10.0, 20.0);
    condition1.valueKey = "diameter";
    tool.addCondition(condition1);

    JudgeCondition condition2("Area", 100.0, 500.0);
    condition2.valueKey = "area";
    tool.addCondition(condition2);

    // 第一个满足，第二个不满足
    tool.setInputValue("diameter", 15.0);
    tool.setInputValue("area", 600.0); // 超出范围

    ToolResult result;
    tool.process(nullptr, result);

    // AND逻辑，一个失败则结果为NG
    EXPECT_FALSE(tool.result().overallResult);
    EXPECT_EQ(tool.result().passCount, 1);
    EXPECT_EQ(tool.result().failCount, 1);
}

TEST_F(LogicToolsTest, RangeJudge_MultiCondition_OR_OnePass) {
    RangeJudgeTool tool;
    tool.setCombineLogic(RangeJudgeTool::LogicOR);

    JudgeCondition condition1("Diameter", 10.0, 20.0);
    condition1.valueKey = "diameter";
    tool.addCondition(condition1);

    JudgeCondition condition2("Area", 100.0, 500.0);
    condition2.valueKey = "area";
    tool.addCondition(condition2);

    // 第一个不满足，第二个满足
    tool.setInputValue("diameter", 5.0);  // 小于最小值
    tool.setInputValue("area", 300.0);

    ToolResult result;
    tool.process(nullptr, result);

    // OR逻辑，一个通过则结果为OK
    EXPECT_TRUE(tool.result().overallResult);
    EXPECT_EQ(tool.result().passCount, 1);
    EXPECT_EQ(tool.result().failCount, 1);
}

TEST_F(LogicToolsTest, RangeJudge_MultiCondition_OR_AllFail) {
    RangeJudgeTool tool;
    tool.setCombineLogic(RangeJudgeTool::LogicOR);

    JudgeCondition condition1("Diameter", 10.0, 20.0);
    condition1.valueKey = "diameter";
    tool.addCondition(condition1);

    JudgeCondition condition2("Area", 100.0, 500.0);
    condition2.valueKey = "area";
    tool.addCondition(condition2);

    // 都不满足
    tool.setInputValue("diameter", 5.0);
    tool.setInputValue("area", 50.0);

    ToolResult result;
    tool.process(nullptr, result);

    // OR逻辑，全部失败则结果为NG
    EXPECT_FALSE(tool.result().overallResult);
    EXPECT_EQ(tool.result().passCount, 0);
    EXPECT_EQ(tool.result().failCount, 2);
}

TEST_F(LogicToolsTest, RangeJudge_BoundaryValues) {
    RangeJudgeTool tool;

    JudgeCondition condition("Value", 10.0, 20.0);
    condition.valueKey = "value";
    tool.addCondition(condition);

    // 测试边界值：最小值
    tool.setInputValue("value", 10.0);
    ToolResult result1;
    tool.process(nullptr, result1);
    EXPECT_TRUE(tool.result().overallResult);

    // 测试边界值：最大值
    tool.setInputValue("value", 20.0);
    ToolResult result2;
    tool.process(nullptr, result2);
    EXPECT_TRUE(tool.result().overallResult);

    // 测试边界外：略小于最小值
    tool.setInputValue("value", 9.999);
    ToolResult result3;
    tool.process(nullptr, result3);
    EXPECT_FALSE(tool.result().overallResult);

    // 测试边界外：略大于最大值
    tool.setInputValue("value", 20.001);
    ToolResult result4;
    tool.process(nullptr, result4);
    EXPECT_FALSE(tool.result().overallResult);
}

TEST_F(LogicToolsTest, RangeJudge_DisabledCondition) {
    RangeJudgeTool tool;

    JudgeCondition condition1("Test", 10.0, 20.0);
    condition1.valueKey = "test";
    condition1.enabled = false; // 禁用条件
    tool.addCondition(condition1);

    // 即使值不满足，禁用的条件也不应影响结果
    tool.setInputValue("test", 100.0);

    ToolResult result;
    bool success = tool.process(nullptr, result);

    EXPECT_TRUE(success);
}

TEST_F(LogicToolsTest, RangeJudge_Serialization) {
    RangeJudgeTool tool;

    tool.setCombineLogic(RangeJudgeTool::LogicOR);

    JudgeCondition condition1("Diameter", 10.0, 20.0);
    JudgeCondition condition2("Area", 100.0, 500.0);
    tool.addCondition(condition1);
    tool.addCondition(condition2);

    // 序列化
    QJsonObject json = tool.serializeParams();
    EXPECT_FALSE(json.isEmpty());

    // 反序列化
    RangeJudgeTool tool2;
    tool2.deserializeParams(json);

    EXPECT_EQ(tool2.combineLogic(), RangeJudgeTool::LogicOR);
    EXPECT_EQ(tool2.conditions().size(), 2);
}

// ============================================================
// 参数化测试 - 逻辑类型
// ============================================================

class LogicTypeTest : public LogicToolsTest,
                      public ::testing::WithParamInterface<LogicType> {};

TEST_P(LogicTypeTest, AllLogicTypes) {
    LogicType type = GetParam();

    LogicOperationTool tool;
    tool.setLogicType(type);

    EXPECT_EQ(tool.logicType(), type);

    // 设置输入值
    QMap<QString, bool> values;
    values["a"] = true;
    values["b"] = false;
    tool.setInputValues(values);

    LogicInput input1, input2;
    input1.valueKey = "a";
    input2.valueKey = "b";
    tool.addInput(input1);

    // NOT操作只需要一个输入
    if (type != LogicType::NOT) {
        tool.addInput(input2);
    }

    ToolResult result;
    bool success = tool.process(nullptr, result);

    // 所有逻辑类型都应该能成功执行
    EXPECT_TRUE(success);
}

INSTANTIATE_TEST_SUITE_P(
    AllTypes,
    LogicTypeTest,
    ::testing::Values(
        LogicType::AND,
        LogicType::OR,
        LogicType::NOT,
        LogicType::XOR,
        LogicType::NAND,
        LogicType::NOR
    )
);

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
