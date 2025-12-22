#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
VisionForge Pro 现场调试说明书 PDF生成器

此脚本将Markdown格式的调试说明书转换为PDF文档。
支持中文字体、表格、代码块等格式。

使用方法:
    python generate_debug_manual_pdf.py

依赖安装:
    pip install markdown reportlab weasyprint

作者: VisionForge Team
日期: 2025-12-22
"""

import os
import sys
import markdown
from pathlib import Path

# 尝试导入PDF生成库
try:
    from reportlab.lib.pagesizes import A4
    from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
    from reportlab.lib.units import mm, cm
    from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, PageBreak, Preformatted
    from reportlab.lib.colors import HexColor, black, white, grey, lightgrey
    from reportlab.pdfbase import pdfmetrics
    from reportlab.pdfbase.ttfonts import TTFont
    from reportlab.lib.enums import TA_LEFT, TA_CENTER, TA_JUSTIFY
    REPORTLAB_AVAILABLE = True
except ImportError:
    REPORTLAB_AVAILABLE = False
    print("警告: reportlab未安装，将尝试使用weasyprint")

try:
    from weasyprint import HTML, CSS
    WEASYPRINT_AVAILABLE = True
except ImportError:
    WEASYPRINT_AVAILABLE = False

def find_chinese_font():
    """查找系统中可用的中文字体"""
    font_paths = [
        # Windows常见字体路径
        "C:/Windows/Fonts/msyh.ttc",      # 微软雅黑
        "C:/Windows/Fonts/simsun.ttc",    # 宋体
        "C:/Windows/Fonts/simhei.ttf",    # 黑体
        "C:/Windows/Fonts/STKAITI.TTF",   # 楷体
        # Linux常见字体路径
        "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc",
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
        # macOS常见字体路径
        "/System/Library/Fonts/PingFang.ttc",
        "/System/Library/Fonts/STHeiti Light.ttc",
    ]

    for path in font_paths:
        if os.path.exists(path):
            return path
    return None

def register_chinese_font():
    """注册中文字体到ReportLab"""
    font_path = find_chinese_font()
    if font_path:
        try:
            pdfmetrics.registerFont(TTFont('ChineseFont', font_path))
            return 'ChineseFont'
        except Exception as e:
            print(f"注册字体失败: {e}")
    return 'Helvetica'  # 回退到默认字体

def create_styles(font_name):
    """创建PDF样式"""
    styles = getSampleStyleSheet()

    # 标题样式
    styles.add(ParagraphStyle(
        name='ChineseTitle',
        fontName=font_name,
        fontSize=24,
        leading=30,
        alignment=TA_CENTER,
        spaceAfter=20,
        textColor=HexColor('#1a5276')
    ))

    # 一级标题
    styles.add(ParagraphStyle(
        name='ChineseH1',
        fontName=font_name,
        fontSize=18,
        leading=24,
        spaceBefore=20,
        spaceAfter=12,
        textColor=HexColor('#2c3e50'),
        borderWidth=0,
        borderPadding=0,
        borderColor=HexColor('#3498db'),
    ))

    # 二级标题
    styles.add(ParagraphStyle(
        name='ChineseH2',
        fontName=font_name,
        fontSize=14,
        leading=20,
        spaceBefore=15,
        spaceAfter=8,
        textColor=HexColor('#34495e')
    ))

    # 三级标题
    styles.add(ParagraphStyle(
        name='ChineseH3',
        fontName=font_name,
        fontSize=12,
        leading=16,
        spaceBefore=10,
        spaceAfter=6,
        textColor=HexColor('#7f8c8d')
    ))

    # 正文样式
    styles.add(ParagraphStyle(
        name='ChineseBody',
        fontName=font_name,
        fontSize=10,
        leading=16,
        alignment=TA_JUSTIFY,
        spaceAfter=8,
        firstLineIndent=20
    ))

    # 代码块样式
    styles.add(ParagraphStyle(
        name='CodeBlock',
        fontName='Courier',
        fontSize=8,
        leading=12,
        leftIndent=10,
        rightIndent=10,
        spaceBefore=8,
        spaceAfter=8,
        backColor=HexColor('#f5f5f5')
    ))

    # 表格标题样式
    styles.add(ParagraphStyle(
        name='TableHeader',
        fontName=font_name,
        fontSize=10,
        leading=14,
        alignment=TA_CENTER,
        textColor=white
    ))

    # 表格单元格样式
    styles.add(ParagraphStyle(
        name='TableCell',
        fontName=font_name,
        fontSize=9,
        leading=12,
        alignment=TA_LEFT
    ))

    return styles

def parse_markdown_to_elements(md_content, styles):
    """将Markdown内容解析为ReportLab元素"""
    elements = []
    lines = md_content.split('\n')

    i = 0
    in_code_block = False
    code_content = []
    in_table = False
    table_rows = []

    while i < len(lines):
        line = lines[i]

        # 处理代码块
        if line.startswith('```'):
            if in_code_block:
                # 结束代码块
                if code_content:
                    code_text = '\n'.join(code_content)
                    elements.append(Preformatted(code_text, styles['CodeBlock']))
                code_content = []
                in_code_block = False
            else:
                # 开始代码块
                in_code_block = True
            i += 1
            continue

        if in_code_block:
            code_content.append(line)
            i += 1
            continue

        # 处理表格
        if '|' in line and not line.startswith('#'):
            if not in_table:
                in_table = True
                table_rows = []

            # 跳过分隔行
            if line.strip().replace('-', '').replace('|', '').replace(' ', '') == '':
                i += 1
                continue

            # 解析表格行
            cells = [cell.strip() for cell in line.split('|')[1:-1]]
            if cells:
                table_rows.append(cells)
            i += 1
            continue
        elif in_table:
            # 表格结束，创建表格
            if table_rows:
                elements.append(create_table(table_rows, styles))
            table_rows = []
            in_table = False

        # 处理标题
        if line.startswith('# '):
            text = line[2:].strip()
            elements.append(Paragraph(text, styles['ChineseTitle']))
            i += 1
            continue

        if line.startswith('## '):
            text = line[3:].strip()
            elements.append(Paragraph(text, styles['ChineseH1']))
            i += 1
            continue

        if line.startswith('### '):
            text = line[4:].strip()
            elements.append(Paragraph(text, styles['ChineseH2']))
            i += 1
            continue

        if line.startswith('#### '):
            text = line[5:].strip()
            elements.append(Paragraph(text, styles['ChineseH3']))
            i += 1
            continue

        # 处理水平线
        if line.strip() in ['---', '***', '___']:
            elements.append(Spacer(1, 10))
            i += 1
            continue

        # 处理空行
        if not line.strip():
            elements.append(Spacer(1, 5))
            i += 1
            continue

        # 处理普通段落
        text = line.strip()
        if text:
            # 转义特殊字符
            text = text.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;')
            # 处理粗体
            import re
            text = re.sub(r'\*\*(.+?)\*\*', r'<b>\1</b>', text)
            text = re.sub(r'__(.+?)__', r'<b>\1</b>', text)
            # 处理斜体
            text = re.sub(r'\*(.+?)\*', r'<i>\1</i>', text)
            text = re.sub(r'_(.+?)_', r'<i>\1</i>', text)
            # 处理行内代码
            text = re.sub(r'`(.+?)`', r'<font face="Courier">\1</font>', text)

            try:
                elements.append(Paragraph(text, styles['ChineseBody']))
            except:
                # 如果段落解析失败，作为预格式化文本处理
                elements.append(Preformatted(line, styles['CodeBlock']))

        i += 1

    # 处理最后的表格
    if in_table and table_rows:
        elements.append(create_table(table_rows, styles))

    return elements

def create_table(rows, styles):
    """创建表格"""
    if not rows:
        return Spacer(1, 5)

    # 计算列宽
    num_cols = max(len(row) for row in rows)
    col_width = (A4[0] - 60) / num_cols

    # 填充缺失的单元格
    normalized_rows = []
    for row in rows:
        normalized_row = row + [''] * (num_cols - len(row))
        normalized_rows.append(normalized_row)

    # 创建表格
    table = Table(normalized_rows, colWidths=[col_width] * num_cols)

    # 表格样式
    style = TableStyle([
        ('BACKGROUND', (0, 0), (-1, 0), HexColor('#3498db')),
        ('TEXTCOLOR', (0, 0), (-1, 0), white),
        ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
        ('FONTSIZE', (0, 0), (-1, 0), 10),
        ('FONTSIZE', (0, 1), (-1, -1), 9),
        ('BOTTOMPADDING', (0, 0), (-1, 0), 8),
        ('TOPPADDING', (0, 0), (-1, 0), 8),
        ('BOTTOMPADDING', (0, 1), (-1, -1), 5),
        ('TOPPADDING', (0, 1), (-1, -1), 5),
        ('BACKGROUND', (0, 1), (-1, -1), HexColor('#ecf0f1')),
        ('GRID', (0, 0), (-1, -1), 0.5, HexColor('#bdc3c7')),
        ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
    ])

    # 交替行背景色
    for i in range(1, len(normalized_rows)):
        if i % 2 == 0:
            style.add('BACKGROUND', (0, i), (-1, i), white)

    table.setStyle(style)
    return table

def generate_pdf_reportlab(md_file, pdf_file):
    """使用ReportLab生成PDF"""
    print("使用ReportLab生成PDF...")

    # 注册中文字体
    font_name = register_chinese_font()
    print(f"使用字体: {font_name}")

    # 读取Markdown内容
    with open(md_file, 'r', encoding='utf-8') as f:
        md_content = f.read()

    # 创建样式
    styles = create_styles(font_name)

    # 创建PDF文档
    doc = SimpleDocTemplate(
        pdf_file,
        pagesize=A4,
        rightMargin=20*mm,
        leftMargin=20*mm,
        topMargin=20*mm,
        bottomMargin=20*mm
    )

    # 解析Markdown为元素
    elements = parse_markdown_to_elements(md_content, styles)

    # 生成PDF
    doc.build(elements)
    print(f"PDF生成成功: {pdf_file}")

def generate_pdf_weasyprint(md_file, pdf_file):
    """使用WeasyPrint生成PDF"""
    print("使用WeasyPrint生成PDF...")

    # 读取Markdown内容
    with open(md_file, 'r', encoding='utf-8') as f:
        md_content = f.read()

    # 转换Markdown为HTML
    md_extensions = ['tables', 'fenced_code', 'codehilite', 'toc']
    html_content = markdown.markdown(md_content, extensions=md_extensions)

    # 创建完整的HTML文档
    html_template = f"""
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="utf-8">
        <title>VisionForge Pro 现场调试说明书</title>
        <style>
            @page {{
                size: A4;
                margin: 2cm;
                @top-center {{
                    content: "VisionForge Pro 现场调试说明书";
                    font-size: 10pt;
                    color: #666;
                }}
                @bottom-center {{
                    content: "第 " counter(page) " 页";
                    font-size: 10pt;
                    color: #666;
                }}
            }}
            body {{
                font-family: "Microsoft YaHei", "SimHei", sans-serif;
                font-size: 10pt;
                line-height: 1.6;
                color: #333;
            }}
            h1 {{
                color: #1a5276;
                font-size: 24pt;
                text-align: center;
                border-bottom: 2px solid #3498db;
                padding-bottom: 10px;
                page-break-after: avoid;
            }}
            h2 {{
                color: #2c3e50;
                font-size: 16pt;
                border-left: 4px solid #3498db;
                padding-left: 10px;
                margin-top: 20px;
                page-break-after: avoid;
            }}
            h3 {{
                color: #34495e;
                font-size: 13pt;
                margin-top: 15px;
                page-break-after: avoid;
            }}
            h4 {{
                color: #7f8c8d;
                font-size: 11pt;
                margin-top: 12px;
                page-break-after: avoid;
            }}
            table {{
                width: 100%;
                border-collapse: collapse;
                margin: 15px 0;
                font-size: 9pt;
            }}
            th {{
                background-color: #3498db;
                color: white;
                padding: 10px 8px;
                text-align: left;
                font-weight: bold;
            }}
            td {{
                padding: 8px;
                border: 1px solid #ddd;
            }}
            tr:nth-child(even) {{
                background-color: #f9f9f9;
            }}
            tr:hover {{
                background-color: #f5f5f5;
            }}
            pre {{
                background-color: #f5f5f5;
                border: 1px solid #ddd;
                border-radius: 4px;
                padding: 12px;
                overflow-x: auto;
                font-family: "Consolas", "Courier New", monospace;
                font-size: 9pt;
                line-height: 1.4;
                white-space: pre-wrap;
                word-wrap: break-word;
            }}
            code {{
                font-family: "Consolas", "Courier New", monospace;
                background-color: #f0f0f0;
                padding: 2px 5px;
                border-radius: 3px;
                font-size: 9pt;
            }}
            pre code {{
                background-color: transparent;
                padding: 0;
            }}
            ul, ol {{
                margin: 10px 0;
                padding-left: 25px;
            }}
            li {{
                margin: 5px 0;
            }}
            blockquote {{
                border-left: 4px solid #3498db;
                margin: 15px 0;
                padding: 10px 20px;
                background-color: #f9f9f9;
                color: #555;
            }}
            hr {{
                border: none;
                border-top: 1px solid #ddd;
                margin: 20px 0;
            }}
            .toc {{
                background-color: #f9f9f9;
                border: 1px solid #ddd;
                padding: 15px;
                margin-bottom: 20px;
            }}
            .note {{
                background-color: #fff3cd;
                border: 1px solid #ffc107;
                border-radius: 4px;
                padding: 12px;
                margin: 15px 0;
            }}
            .warning {{
                background-color: #f8d7da;
                border: 1px solid #f5c6cb;
                border-radius: 4px;
                padding: 12px;
                margin: 15px 0;
            }}
            .info {{
                background-color: #d1ecf1;
                border: 1px solid #bee5eb;
                border-radius: 4px;
                padding: 12px;
                margin: 15px 0;
            }}
        </style>
    </head>
    <body>
        {html_content}
    </body>
    </html>
    """

    # 生成PDF
    HTML(string=html_template).write_pdf(pdf_file)
    print(f"PDF生成成功: {pdf_file}")

def generate_simple_pdf(md_file, pdf_file):
    """使用简化方法生成PDF（基于纯文本转换）"""
    print("使用简化方法生成PDF...")

    try:
        from reportlab.lib.pagesizes import A4
        from reportlab.pdfgen import canvas
        from reportlab.lib.units import mm
    except ImportError:
        print("错误: reportlab未安装，请运行: pip install reportlab")
        return False

    # 查找中文字体
    font_path = find_chinese_font()

    # 读取Markdown内容
    with open(md_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    # 创建PDF
    c = canvas.Canvas(pdf_file, pagesize=A4)
    width, height = A4

    # 注册中文字体
    if font_path:
        try:
            pdfmetrics.registerFont(TTFont('ChineseFont', font_path))
            font_name = 'ChineseFont'
        except:
            font_name = 'Helvetica'
    else:
        font_name = 'Helvetica'

    # 设置初始位置
    x = 20 * mm
    y = height - 20 * mm
    line_height = 5 * mm

    # 绘制内容
    for line in lines:
        line = line.rstrip()

        # 处理标题
        if line.startswith('# '):
            c.setFont(font_name, 18)
            y -= 10 * mm
        elif line.startswith('## '):
            c.setFont(font_name, 14)
            y -= 8 * mm
            line = line[3:]
        elif line.startswith('### '):
            c.setFont(font_name, 12)
            y -= 6 * mm
            line = line[4:]
        elif line.startswith('#### '):
            c.setFont(font_name, 11)
            y -= 5 * mm
            line = line[5:]
        else:
            c.setFont(font_name, 10)

        # 检查是否需要换页
        if y < 30 * mm:
            c.showPage()
            y = height - 20 * mm
            c.setFont(font_name, 10)

        # 绘制文本（处理长行）
        if len(line) > 80:
            # 简单分割长行
            while len(line) > 80:
                c.drawString(x, y, line[:80])
                line = line[80:]
                y -= line_height
                if y < 30 * mm:
                    c.showPage()
                    y = height - 20 * mm
            if line:
                c.drawString(x, y, line)
        else:
            c.drawString(x, y, line)

        y -= line_height

    c.save()
    print(f"PDF生成成功: {pdf_file}")
    return True

def main():
    """主函数"""
    # 获取脚本所在目录
    script_dir = Path(__file__).parent

    # 输入输出文件路径
    md_file = script_dir / "现场调试说明书.md"
    pdf_file = script_dir / "VisionForge_Pro_现场调试说明书.pdf"

    # 检查输入文件是否存在
    if not md_file.exists():
        print(f"错误: 找不到Markdown文件: {md_file}")
        return 1

    print(f"源文件: {md_file}")
    print(f"目标文件: {pdf_file}")

    # 选择PDF生成方法
    success = False

    if WEASYPRINT_AVAILABLE:
        try:
            generate_pdf_weasyprint(str(md_file), str(pdf_file))
            success = True
        except Exception as e:
            print(f"WeasyPrint生成失败: {e}")

    if not success and REPORTLAB_AVAILABLE:
        try:
            generate_pdf_reportlab(str(md_file), str(pdf_file))
            success = True
        except Exception as e:
            print(f"ReportLab生成失败: {e}")
            # 尝试简化方法
            try:
                success = generate_simple_pdf(str(md_file), str(pdf_file))
            except Exception as e2:
                print(f"简化方法也失败: {e2}")

    if not success:
        print("\n所有PDF生成方法都失败了。")
        print("请安装必要的依赖:")
        print("  pip install reportlab markdown")
        print("  或")
        print("  pip install weasyprint markdown")
        return 1

    return 0

if __name__ == "__main__":
    sys.exit(main())
