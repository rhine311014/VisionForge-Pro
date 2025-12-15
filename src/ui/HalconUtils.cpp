/**
 * @file HalconUtils.cpp
 * @brief Halcon工具函数实现
 */

#include "ui/HalconUtils.h"
#include "base/Logger.h"

// 解决Halcon宏冲突
#ifdef high
#undef high
#endif
#ifdef low
#undef low
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace VisionForge {
namespace UI {

#ifdef _WIN32

void HalconUtils::SetDisplayFont(
    const HTuple& windowHandle,
    int size,
    const QString& font,
    bool bold,
    bool slant)
{
    try {
        // Local control variables
        HTuple hv_OS, hv_Fonts, hv_Style;
        HTuple hv_AvailableFonts, hv_Fdx, hv_Indices;
        HTuple hv_Size = size;

        // 获取操作系统类型
        GetSystem("operating_system", &hv_OS);

        // 如果size为-1或未指定，使用默认值16
        if (hv_Size == HTuple() || hv_Size == -1) {
            hv_Size = 16;
        }

        // Windows系统需要调整字体大小
        if ((hv_OS.TupleSubstr(0, 2)) == HTuple("Win")) {
            hv_Size = (1.13677 * hv_Size).TupleInt();
        } else {
            hv_Size = hv_Size.TupleInt();
        }

        // 根据字体类型选择字体列表
        if (font == "Courier") {
            hv_Fonts.Clear();
            hv_Fonts[0] = "Courier";
            hv_Fonts[1] = "Courier 10 Pitch";
            hv_Fonts[2] = "Courier New";
            hv_Fonts[3] = "CourierNew";
            hv_Fonts[4] = "Liberation Mono";
        }
        else if (font == "mono") {
            hv_Fonts.Clear();
            hv_Fonts[0] = "Consolas";
            hv_Fonts[1] = "Menlo";
            hv_Fonts[2] = "Courier";
            hv_Fonts[3] = "Courier 10 Pitch";
            hv_Fonts[4] = "FreeMono";
            hv_Fonts[5] = "Liberation Mono";
            hv_Fonts[6] = "DejaVu Sans Mono";
        }
        else if (font == "sans") {
            hv_Fonts.Clear();
            hv_Fonts[0] = "Luxi Sans";
            hv_Fonts[1] = "DejaVu Sans";
            hv_Fonts[2] = "FreeSans";
            hv_Fonts[3] = "Arial";
            hv_Fonts[4] = "Liberation Sans";
        }
        else if (font == "serif") {
            hv_Fonts.Clear();
            hv_Fonts[0] = "Times New Roman";
            hv_Fonts[1] = "Luxi Serif";
            hv_Fonts[2] = "DejaVu Serif";
            hv_Fonts[3] = "FreeSerif";
            hv_Fonts[4] = "Utopia";
            hv_Fonts[5] = "Liberation Serif";
        }
        else {
            // 使用指定的字体名称
            hv_Fonts = font.toStdString().c_str();
        }

        // 构建字体样式
        hv_Style = "";
        if (bold) {
            hv_Style += HTuple("Bold");
        }
        if (slant) {
            hv_Style += HTuple("Italic");
        }
        if (hv_Style == HTuple("")) {
            hv_Style = "Normal";
        }

        // 查询可用字体
        QueryFont(windowHandle, &hv_AvailableFonts);

        HTuple hv_Font = "";
        // 在可用字体中查找匹配的字体
        HTuple end_val = (hv_Fonts.TupleLength()) - 1;
        HTuple step_val = 1;
        for (hv_Fdx = 0; hv_Fdx.Continue(end_val, step_val); hv_Fdx += step_val) {
            hv_Indices = hv_AvailableFonts.TupleFind(HTuple(hv_Fonts[hv_Fdx]));
            if (hv_Indices.TupleLength() > 0) {
                if (HTuple(hv_Indices[0]) >= 0) {
                    hv_Font = HTuple(hv_Fonts[hv_Fdx]);
                    break;
                }
            }
        }

        // 如果没找到合适字体，使用第一个可用字体
        if (hv_Font == HTuple("")) {
            if (hv_AvailableFonts.TupleLength() > 0) {
                hv_Font = hv_AvailableFonts[0];
                LOG_WARNING(QString("未找到指定字体 '%1'，使用默认字体").arg(font));
            } else {
                LOG_ERROR("系统无可用字体");
                return;
            }
        }

        // 组合完整字体字符串
        hv_Font = (((hv_Font + "-") + hv_Style) + "-") + hv_Size;

        // 设置字体
        SetFont(windowHandle, hv_Font);

        LOG_DEBUG(QString("Halcon字体已设置: %1").arg(hv_Font.ToString().Text()));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("设置Halcon字体失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::SetColoredMode(const HTuple& windowHandle, int numGray)
{
    try {
        HalconCpp::SetColored(windowHandle, numGray);
        LOG_DEBUG(QString("Halcon颜色模式已设置: %1级灰度").arg(numGray));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("设置Halcon颜色模式失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::EnableUTF8()
{
    try {
        SetHcppInterfaceStringEncodingIsUtf8(true);
        LOG_DEBUG("Halcon UTF-8编码已启用");
    }
    catch (const HException& e) {
        LOG_ERROR(QString("启用UTF-8编码失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::SetLineWidth(const HTuple& windowHandle, int lineWidth)
{
    try {
        HalconCpp::SetLineWidth(windowHandle, lineWidth);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("设置线宽失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::SetDrawMode(const HTuple& windowHandle, const QString& drawMode)
{
    try {
        SetDraw(windowHandle, drawMode.toStdString().c_str());
    }
    catch (const HException& e) {
        LOG_ERROR(QString("设置绘制模式失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::SetColor(const HTuple& windowHandle, const QString& color)
{
    try {
        HalconCpp::SetColor(windowHandle, color.toStdString().c_str());
    }
    catch (const HException& e) {
        LOG_ERROR(QString("设置颜色失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::DisplayText(
    const HTuple& windowHandle,
    const QString& text,
    const QString& coordSystem,
    double row,
    double col,
    const QString& color,
    bool box,
    const QString& boxColor)
{
    try {
        HTuple hv_Text = text.toUtf8().constData();
        HTuple hv_CoordSystem = coordSystem.toStdString().c_str();
        HTuple hv_Row = row;
        HTuple hv_Column = col;
        HTuple hv_Color = color.toStdString().c_str();

        if (box) {
            HTuple hv_Box = "true";
            HTuple hv_BoxColor = boxColor.toStdString().c_str();

            // 使用DispText显示带框文本
            DispText(windowHandle, hv_Text, hv_CoordSystem, hv_Row, hv_Column,
                    hv_Color, hv_Box, hv_BoxColor);
        } else {
            HTuple hv_Box = "false";
            DispText(windowHandle, hv_Text, hv_CoordSystem, hv_Row, hv_Column,
                    hv_Color, hv_Box, HTuple());
        }
    }
    catch (const HException& e) {
        LOG_ERROR(QString("显示文本失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::DisplayTextWithShadow(
    const HTuple& windowHandle,
    const QString& text,
    double row,
    double col,
    const QString& color)
{
    try {
        HTuple hv_Text = text.toUtf8().constData();

        // 先显示阴影（偏移1像素，黑色）
        DisplayText(windowHandle, text, "image", row + 1, col + 1, "black", true, "black");

        // 再显示主文本
        DisplayText(windowHandle, text, "image", row, col, color, true, "black");
    }
    catch (const HException& e) {
        LOG_ERROR(QString("显示带阴影文本失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::InitializeWindow(const HTuple& windowHandle)
{
    try {
        // 启用UTF-8编码
        EnableUTF8();

        // 设置颜色模式（12级灰度，用于彩色显示）
        SetColoredMode(windowHandle, 12);

        // 设置默认线宽
        SetLineWidth(windowHandle, 2);

        // 设置默认绘制模式为边缘
        SetDrawMode(windowHandle, "margin");

        // 设置默认字体
        SetDisplayFont(windowHandle, 16, "mono", false, false);

        // 设置背景色为黑色
        SetWindowAttr("background_color", "black");

        LOG_INFO("Halcon窗口初始化配置完成");
    }
    catch (const HException& e) {
        LOG_ERROR(QString("初始化Halcon窗口配置失败: %1").arg(e.ErrorMessage().Text()));
    }
}

void HalconUtils::FitImageToWindow(const HTuple& windowHandle, const HImage& image)
{
    try {
        if (!image.IsInitialized() || image.CountObj() == 0) {
            return;
        }

        HTuple hv_Width, hv_Height;
        GetImageSize(image, &hv_Width, &hv_Height);

        // 设置显示区域为整个图像
        SetPart(windowHandle, 0, 0, hv_Height - 1, hv_Width - 1);

        LOG_DEBUG(QString("图像已适应窗口: %1x%2").arg(hv_Width[0].I()).arg(hv_Height[0].I()));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("图像适应窗口失败: %1").arg(e.ErrorMessage().Text()));
    }
}

#endif // _WIN32

} // namespace UI
} // namespace VisionForge
