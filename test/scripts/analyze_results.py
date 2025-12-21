#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
72小时测试结果分析脚本
功能:
- 加载测试快照数据
- 计算统计指标
- 绘制趋势图表
- 生成详细分析报告

使用方法:
    python analyze_results.py --input test_results/snapshots.jsonl --output analysis_report.html

作者: VisionForge Team
日期: 2025-12-20
"""

import argparse
import json
import sys
from pathlib import Path
from typing import List, Dict
from datetime import datetime
import statistics

# ========== 数据加载 ==========

def load_snapshots(file_path: Path) -> List[Dict]:
    """加载快照数据"""
    snapshots = []

    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            for line in f:
                snapshot = json.loads(line.strip())
                snapshots.append(snapshot)

        print(f"已加载 {len(snapshots)} 个快照")
        return snapshots

    except Exception as e:
        print(f"加载快照失败: {e}")
        return []

# ========== 统计分析 ==========

def analyze_memory(snapshots: List[Dict]) -> Dict:
    """分析内存使用情况"""
    if not snapshots:
        return {}

    memory_values = [s['memory_rss_mb'] for s in snapshots if 'memory_rss_mb' in s]

    if not memory_values:
        return {}

    # 基础统计
    initial = memory_values[0]
    final = memory_values[-1]
    peak = max(memory_values)
    avg = statistics.mean(memory_values)

    # 内存增长
    growth = final - initial
    growth_rate_mb_h = growth / (len(snapshots) * 5 / 60)  # 假设5分钟采样间隔

    # 预测72小时增长
    growth_72h = growth_rate_mb_h * 72

    return {
        'initial_mb': initial,
        'final_mb': final,
        'peak_mb': peak,
        'avg_mb': avg,
        'growth_mb': growth,
        'growth_rate_mb_h': growth_rate_mb_h,
        'growth_72h_mb': growth_72h
    }

def analyze_cpu(snapshots: List[Dict]) -> Dict:
    """分析CPU使用情况"""
    if not snapshots:
        return {}

    cpu_values = [s['cpu_percent'] for s in snapshots if 'cpu_percent' in s]

    if not cpu_values:
        return {}

    return {
        'avg_percent': statistics.mean(cpu_values),
        'peak_percent': max(cpu_values),
        'min_percent': min(cpu_values),
        'stdev_percent': statistics.stdev(cpu_values) if len(cpu_values) > 1 else 0
    }

def analyze_threads(snapshots: List[Dict]) -> Dict:
    """分析线程数"""
    if not snapshots:
        return {}

    thread_values = [s['num_threads'] for s in snapshots if 'num_threads' in s]

    if not thread_values:
        return {}

    return {
        'avg': statistics.mean(thread_values),
        'peak': max(thread_values),
        'min': min(thread_values)
    }

def check_acceptance_criteria(analysis: Dict) -> Dict:
    """检查验收标准"""
    results = {}

    # 内存增长检查 (<100MB/72h)
    if 'memory' in analysis and 'growth_72h_mb' in analysis['memory']:
        growth_72h = analysis['memory']['growth_72h_mb']
        results['memory_growth'] = {
            'value': growth_72h,
            'threshold': 100,
            'passed': growth_72h < 100,
            'message': f"{growth_72h:.1f} MB/72h {'✅' if growth_72h < 100 else '❌'}"
        }

    # CPU使用率检查 (<60%)
    if 'cpu' in analysis and 'avg_percent' in analysis['cpu']:
        cpu_avg = analysis['cpu']['avg_percent']
        results['cpu_usage'] = {
            'value': cpu_avg,
            'threshold': 60,
            'passed': cpu_avg < 60,
            'message': f"{cpu_avg:.1f}% {'✅' if cpu_avg < 60 else '❌'}"
        }

    return results

# ========== 报告生成 ==========

def generate_text_report(analysis: Dict, output_file: Path):
    """生成文本报告"""
    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("="*60 + "\n")
            f.write("72小时稳定性测试分析报告\n")
            f.write("="*60 + "\n\n")

            # 内存分析
            if 'memory' in analysis:
                mem = analysis['memory']
                f.write("【内存分析】\n")
                f.write(f"  初始内存: {mem.get('initial_mb', 0):.0f} MB\n")
                f.write(f"  最终内存: {mem.get('final_mb', 0):.0f} MB\n")
                f.write(f"  峰值内存: {mem.get('peak_mb', 0):.0f} MB\n")
                f.write(f"  平均内存: {mem.get('avg_mb', 0):.0f} MB\n")
                f.write(f"  内存增长: {mem.get('growth_mb', 0):.0f} MB\n")
                f.write(f"  增长速率: {mem.get('growth_rate_mb_h', 0):.2f} MB/h\n")
                f.write(f"  预测72h增长: {mem.get('growth_72h_mb', 0):.1f} MB\n\n")

            # CPU分析
            if 'cpu' in analysis:
                cpu = analysis['cpu']
                f.write("【CPU分析】\n")
                f.write(f"  平均使用率: {cpu.get('avg_percent', 0):.1f}%\n")
                f.write(f"  峰值使用率: {cpu.get('peak_percent', 0):.1f}%\n")
                f.write(f"  最低使用率: {cpu.get('min_percent', 0):.1f}%\n")
                f.write(f"  标准差: {cpu.get('stdev_percent', 0):.1f}%\n\n")

            # 线程分析
            if 'threads' in analysis:
                threads = analysis['threads']
                f.write("【线程分析】\n")
                f.write(f"  平均线程数: {threads.get('avg', 0):.0f}\n")
                f.write(f"  峰值线程数: {threads.get('peak', 0)}\n")
                f.write(f"  最低线程数: {threads.get('min', 0)}\n\n")

            # 验收标准
            if 'acceptance' in analysis:
                f.write("【验收标准检查】\n")
                for criterion, result in analysis['acceptance'].items():
                    f.write(f"  {criterion}: {result['message']}\n")
                f.write("\n")

            f.write("="*60 + "\n")

        print(f"文本报告已生成: {output_file}")
        return True

    except Exception as e:
        print(f"生成文本报告失败: {e}")
        return False

def generate_html_report(analysis: Dict, snapshots: List[Dict], output_file: Path):
    """生成HTML报告（包含图表）"""
    try:
        html = """
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>72小时稳定性测试分析报告</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js@3.9.1/dist/chart.min.js"></script>
    <style>
        body {
            font-family: "Microsoft YaHei", Arial, sans-serif;
            margin: 20px;
            background-color: #f5f5f5;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: white;
            padding: 30px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
        h1 { color: #333; border-bottom: 2px solid #007bff; padding-bottom: 10px; }
        h2 { color: #555; margin-top: 30px; }
        .metric { margin: 10px 0; padding: 10px; background-color: #f8f9fa; border-left: 4px solid #007bff; }
        .pass { color: green; font-weight: bold; }
        .fail { color: red; font-weight: bold; }
        .chart-container { margin: 30px 0; height: 400px; }
        table { width: 100%; border-collapse: collapse; margin: 20px 0; }
        th, td { padding: 12px; text-align: left; border: 1px solid #ddd; }
        th { background-color: #007bff; color: white; }
        tr:nth-child(even) { background-color: #f2f2f2; }
    </style>
</head>
<body>
    <div class="container">
        <h1>72小时稳定性测试分析报告</h1>
        <p>生成时间: """ + datetime.now().strftime("%Y-%m-%d %H:%M:%S") + """</p>

        <h2>内存分析</h2>
"""

        # 内存统计表
        if 'memory' in analysis:
            mem = analysis['memory']
            html += """
        <table>
            <tr><th>指标</th><th>值</th></tr>
            <tr><td>初始内存</td><td>""" + f"{mem.get('initial_mb', 0):.0f} MB" + """</td></tr>
            <tr><td>最终内存</td><td>""" + f"{mem.get('final_mb', 0):.0f} MB" + """</td></tr>
            <tr><td>峰值内存</td><td>""" + f"{mem.get('peak_mb', 0):.0f} MB" + """</td></tr>
            <tr><td>内存增长</td><td>""" + f"{mem.get('growth_mb', 0):.0f} MB" + """</td></tr>
            <tr><td>预测72h增长</td><td>""" + f"{mem.get('growth_72h_mb', 0):.1f} MB" + """</td></tr>
        </table>
"""

        # 内存趋势图
        if snapshots:
            timestamps = [s.get('elapsed_seconds', i*300) for i, s in enumerate(snapshots)]
            memory_values = [s.get('memory_rss_mb', 0) for s in snapshots]

            html += """
        <div class="chart-container">
            <canvas id="memoryChart"></canvas>
        </div>
"""

        # CPU分析
        if 'cpu' in analysis:
            cpu = analysis['cpu']
            html += """
        <h2>CPU分析</h2>
        <table>
            <tr><th>指标</th><th>值</th></tr>
            <tr><td>平均使用率</td><td>""" + f"{cpu.get('avg_percent', 0):.1f}%" + """</td></tr>
            <tr><td>峰值使用率</td><td>""" + f"{cpu.get('peak_percent', 0):.1f}%" + """</td></tr>
            <tr><td>最低使用率</td><td>""" + f"{cpu.get('min_percent', 0):.1f}%" + """</td></tr>
        </table>
"""

        # 验收标准
        if 'acceptance' in analysis:
            html += """
        <h2>验收标准检查</h2>
        <table>
            <tr><th>标准</th><th>实际值</th><th>阈值</th><th>结果</th></tr>
"""
            for criterion, result in analysis['acceptance'].items():
                status_class = 'pass' if result['passed'] else 'fail'
                status_text = '✅ 通过' if result['passed'] else '❌ 失败'

                html += f"""
            <tr>
                <td>{criterion}</td>
                <td>{result['value']:.1f}</td>
                <td>{result['threshold']}</td>
                <td class="{status_class}">{status_text}</td>
            </tr>
"""
            html += """
        </table>
"""

        # JavaScript图表代码
        if snapshots:
            html += """
        <script>
            const ctx = document.getElementById('memoryChart').getContext('2d');
            const chart = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: """ + json.dumps([t/3600 for t in timestamps]) + """,
                    datasets: [{
                        label: '内存使用 (MB)',
                        data: """ + json.dumps(memory_values) + """,
                        borderColor: 'rgb(75, 192, 192)',
                        tension: 0.1
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    scales: {
                        x: { title: { display: true, text: '时间 (小时)' } },
                        y: { title: { display: true, text: '内存 (MB)' } }
                    }
                }
            });
        </script>
"""

        html += """
    </div>
</body>
</html>
"""

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(html)

        print(f"HTML报告已生成: {output_file}")
        return True

    except Exception as e:
        print(f"生成HTML报告失败: {e}")
        return False

# ========== 主函数 ==========

def main():
    parser = argparse.ArgumentParser(description='72小时测试结果分析工具')

    parser.add_argument(
        '--input',
        required=True,
        help='输入快照文件（snapshots.jsonl）'
    )

    parser.add_argument(
        '--output',
        default='analysis_report',
        help='输出报告文件名前缀，默认analysis_report'
    )

    args = parser.parse_args()

    input_file = Path(args.input)
    if not input_file.exists():
        print(f"错误: 输入文件不存在: {input_file}")
        return 1

    # 加载数据
    print("正在加载快照数据...")
    snapshots = load_snapshots(input_file)

    if not snapshots:
        print("错误: 没有可分析的数据")
        return 1

    # 分析数据
    print("正在分析数据...")
    analysis = {
        'memory': analyze_memory(snapshots),
        'cpu': analyze_cpu(snapshots),
        'threads': analyze_threads(snapshots)
    }

    analysis['acceptance'] = check_acceptance_criteria(analysis)

    # 生成报告
    print("正在生成报告...")
    output_dir = Path(args.output).parent
    output_name = Path(args.output).stem

    text_report = output_dir / f"{output_name}.txt"
    html_report = output_dir / f"{output_name}.html"

    generate_text_report(analysis, text_report)
    generate_html_report(analysis, snapshots, html_report)

    print("\n分析完成!")
    return 0

if __name__ == '__main__':
    sys.exit(main())
