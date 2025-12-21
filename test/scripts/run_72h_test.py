#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
72小时稳定性测试自动化脚本
功能:
- 启动VisionForge Pro测试程序
- 监控进程状态
- 自动重启崩溃进程
- 收集性能数据
- 生成测试报告

使用方法:
    python run_72h_test.py --exe-path "path/to/VisionForge.exe" --duration 72

作者: VisionForge Team
日期: 2025-12-20
"""

import argparse
import subprocess
import time
import os
import sys
import json
import psutil
import logging
from datetime import datetime, timedelta
from pathlib import Path
from typing import Optional, Dict, List

# ========== 配置 ==========

DEFAULT_DURATION_HOURS = 72
DEFAULT_CHECK_INTERVAL = 60  # 每分钟检查一次进程状态
DEFAULT_SNAPSHOT_INTERVAL = 300  # 每5分钟采集一次快照

# ========== 日志配置 ==========

def setup_logging(log_dir: Path) -> logging.Logger:
    """配置日志系统"""
    log_dir.mkdir(parents=True, exist_ok=True)

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    log_file = log_dir / f"test_72h_{timestamp}.log"

    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s [%(levelname)s] %(message)s',
        handlers=[
            logging.FileHandler(log_file, encoding='utf-8'),
            logging.StreamHandler(sys.stdout)
        ]
    )

    logger = logging.getLogger('72h_test')
    logger.info(f"日志文件: {log_file}")

    return logger

# ========== 进程监控类 ==========

class ProcessMonitor:
    """进程监控器"""

    def __init__(self, exe_path: str, logger: logging.Logger):
        self.exe_path = exe_path
        self.logger = logger
        self.process: Optional[subprocess.Popen] = None
        self.psutil_process: Optional[psutil.Process] = None
        self.start_time: Optional[datetime] = None
        self.crash_count = 0

    def start(self) -> bool:
        """启动进程"""
        try:
            self.logger.info(f"启动进程: {self.exe_path}")

            self.process = subprocess.Popen(
                [self.exe_path],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                creationflags=subprocess.CREATE_NEW_CONSOLE if sys.platform == 'win32' else 0
            )

            # 等待进程启动
            time.sleep(5)

            if self.process.poll() is not None:
                self.logger.error(f"进程启动失败，退出码: {self.process.returncode}")
                return False

            self.psutil_process = psutil.Process(self.process.pid)
            self.start_time = datetime.now()

            self.logger.info(f"进程已启动，PID: {self.process.pid}")
            return True

        except Exception as e:
            self.logger.error(f"启动进程异常: {e}")
            return False

    def is_running(self) -> bool:
        """检查进程是否运行"""
        if not self.process:
            return False

        return self.process.poll() is None

    def get_resource_usage(self) -> Dict:
        """获取资源使用情况"""
        if not self.psutil_process:
            return {}

        try:
            cpu_percent = self.psutil_process.cpu_percent(interval=1.0)
            memory_info = self.psutil_process.memory_info()

            return {
                'timestamp': datetime.now().isoformat(),
                'cpu_percent': cpu_percent,
                'memory_rss_mb': memory_info.rss / (1024 * 1024),
                'memory_vms_mb': memory_info.vms / (1024 * 1024),
                'num_threads': self.psutil_process.num_threads(),
                'elapsed_seconds': (datetime.now() - self.start_time).total_seconds()
            }
        except psutil.NoSuchProcess:
            return {}

    def stop(self):
        """停止进程"""
        if not self.process:
            return

        try:
            self.logger.info("正在停止进程...")

            if sys.platform == 'win32':
                self.process.terminate()
            else:
                self.process.send_signal(signal.SIGTERM)

            # 等待进程优雅退出
            try:
                self.process.wait(timeout=30)
            except subprocess.TimeoutExpired:
                self.logger.warning("进程未响应，强制终止")
                self.process.kill()
                self.process.wait()

            self.logger.info("进程已停止")

        except Exception as e:
            self.logger.error(f"停止进程异常: {e}")

# ========== 测试管理器 ==========

class TestManager:
    """测试管理器"""

    def __init__(self, args, logger: logging.Logger):
        self.args = args
        self.logger = logger
        self.monitor = ProcessMonitor(args.exe_path, logger)

        # 创建输出目录
        self.output_dir = Path(args.output_dir)
        self.output_dir.mkdir(parents=True, exist_ok=True)

        # 数据收集
        self.snapshots: List[Dict] = []

        # 测试状态
        self.start_time: Optional[datetime] = None
        self.end_time: Optional[datetime] = None
        self.target_duration = timedelta(hours=args.duration)

    def run(self):
        """运行测试"""
        self.logger.info("="*60)
        self.logger.info(f"开始72小时稳定性测试")
        self.logger.info(f"目标时长: {self.args.duration} 小时")
        self.logger.info(f"可执行文件: {self.args.exe_path}")
        self.logger.info(f"输出目录: {self.output_dir}")
        self.logger.info("="*60)

        # 启动被测程序
        if not self.monitor.start():
            self.logger.error("无法启动测试程序，退出")
            return 1

        self.start_time = datetime.now()
        self.end_time = self.start_time + self.target_duration

        self.logger.info(f"测试开始时间: {self.start_time}")
        self.logger.info(f"预计结束时间: {self.end_time}")

        # 主测试循环
        try:
            self._test_loop()
        except KeyboardInterrupt:
            self.logger.warning("收到中断信号，停止测试")
        finally:
            self.monitor.stop()
            self._generate_report()

        return 0

    def _test_loop(self):
        """主测试循环"""
        last_snapshot_time = time.time()

        while datetime.now() < self.end_time:
            # 检查进程状态
            if not self.monitor.is_running():
                self.logger.error("进程已崩溃!")
                self.monitor.crash_count += 1

                if self.args.auto_restart:
                    self.logger.info("自动重启进程...")
                    if not self.monitor.start():
                        self.logger.error("重启失败，退出测试")
                        break
                else:
                    self.logger.error("未启用自动重启，退出测试")
                    break

            # 采集快照
            current_time = time.time()
            if current_time - last_snapshot_time >= DEFAULT_SNAPSHOT_INTERVAL:
                snapshot = self.monitor.get_resource_usage()
                if snapshot:
                    self.snapshots.append(snapshot)
                    self._save_snapshot(snapshot)
                    self._log_progress(snapshot)

                last_snapshot_time = current_time

            # 休眠
            time.sleep(DEFAULT_CHECK_INTERVAL)

        self.logger.info("测试时长已达到目标，停止测试")

    def _save_snapshot(self, snapshot: Dict):
        """保存快照到文件"""
        snapshot_file = self.output_dir / "snapshots.jsonl"

        try:
            with open(snapshot_file, 'a', encoding='utf-8') as f:
                f.write(json.dumps(snapshot, ensure_ascii=False) + '\n')
        except Exception as e:
            self.logger.error(f"保存快照失败: {e}")

    def _log_progress(self, snapshot: Dict):
        """记录进度"""
        elapsed = datetime.now() - self.start_time
        remaining = self.end_time - datetime.now()
        progress = (elapsed.total_seconds() / self.target_duration.total_seconds()) * 100

        self.logger.info(
            f"进度: {progress:.1f}% | "
            f"已运行: {str(elapsed).split('.')[0]} | "
            f"剩余: {str(remaining).split('.')[0]} | "
            f"CPU: {snapshot.get('cpu_percent', 0):.1f}% | "
            f"内存: {snapshot.get('memory_rss_mb', 0):.0f}MB | "
            f"线程: {snapshot.get('num_threads', 0)}"
        )

    def _generate_report(self):
        """生成测试报告"""
        self.logger.info("生成测试报告...")

        report = {
            'test_name': '72小时稳定性测试',
            'start_time': self.start_time.isoformat() if self.start_time else None,
            'end_time': datetime.now().isoformat(),
            'target_duration_hours': self.args.duration,
            'actual_duration_seconds': (datetime.now() - self.start_time).total_seconds() if self.start_time else 0,
            'crash_count': self.monitor.crash_count,
            'total_snapshots': len(self.snapshots),
            'exe_path': self.args.exe_path,
            'auto_restart_enabled': self.args.auto_restart
        }

        # 计算统计信息
        if self.snapshots:
            cpu_values = [s['cpu_percent'] for s in self.snapshots if 'cpu_percent' in s]
            memory_values = [s['memory_rss_mb'] for s in self.snapshots if 'memory_rss_mb' in s]

            if cpu_values:
                report['cpu_avg'] = sum(cpu_values) / len(cpu_values)
                report['cpu_max'] = max(cpu_values)

            if memory_values:
                report['memory_initial_mb'] = memory_values[0]
                report['memory_final_mb'] = memory_values[-1]
                report['memory_max_mb'] = max(memory_values)
                report['memory_growth_mb'] = memory_values[-1] - memory_values[0]

        # 保存报告
        report_file = self.output_dir / f"report_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"

        try:
            with open(report_file, 'w', encoding='utf-8') as f:
                json.dump(report, f, indent=2, ensure_ascii=False)

            self.logger.info(f"测试报告已保存: {report_file}")

            # 打印摘要
            self._print_summary(report)

        except Exception as e:
            self.logger.error(f"保存报告失败: {e}")

    def _print_summary(self, report: Dict):
        """打印测试摘要"""
        self.logger.info("\n" + "="*60)
        self.logger.info("测试摘要")
        self.logger.info("="*60)
        self.logger.info(f"测试时长: {report['actual_duration_seconds'] / 3600:.2f} 小时")
        self.logger.info(f"崩溃次数: {report['crash_count']}")
        self.logger.info(f"采样数量: {report['total_snapshots']}")

        if 'cpu_avg' in report:
            self.logger.info(f"平均CPU: {report['cpu_avg']:.1f}%")
            self.logger.info(f"峰值CPU: {report['cpu_max']:.1f}%")

        if 'memory_initial_mb' in report:
            self.logger.info(f"初始内存: {report['memory_initial_mb']:.0f} MB")
            self.logger.info(f"最终内存: {report['memory_final_mb']:.0f} MB")
            self.logger.info(f"峰值内存: {report['memory_max_mb']:.0f} MB")
            self.logger.info(f"内存增长: {report['memory_growth_mb']:.0f} MB")

        # 验收标准检查
        self.logger.info("\n验收标准检查:")

        passed = True

        # 内存增长检查 (<100MB/72h)
        if 'memory_growth_mb' in report:
            memory_growth_72h = report['memory_growth_mb'] * (72 / (report['actual_duration_seconds'] / 3600))
            if memory_growth_72h < 100:
                self.logger.info(f"✅ 内存增长: {memory_growth_72h:.1f} MB/72h (< 100 MB)")
            else:
                self.logger.error(f"❌ 内存增长: {memory_growth_72h:.1f} MB/72h (>= 100 MB)")
                passed = False

        # CPU使用率检查 (<60%)
        if 'cpu_avg' in report:
            if report['cpu_avg'] < 60:
                self.logger.info(f"✅ CPU使用率: {report['cpu_avg']:.1f}% (< 60%)")
            else:
                self.logger.error(f"❌ CPU使用率: {report['cpu_avg']:.1f}% (>= 60%)")
                passed = False

        # 崩溃检查 (=0)
        if report['crash_count'] == 0:
            self.logger.info(f"✅ 无崩溃")
        else:
            self.logger.error(f"❌ 崩溃次数: {report['crash_count']}")
            passed = False

        self.logger.info("\n" + "="*60)
        if passed:
            self.logger.info("✅ 测试通过")
        else:
            self.logger.error("❌ 测试失败")
        self.logger.info("="*60 + "\n")

# ========== 主函数 ==========

def main():
    parser = argparse.ArgumentParser(
        description='VisionForge Pro 72小时稳定性测试自动化脚本',
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument(
        '--exe-path',
        required=True,
        help='VisionForge Pro可执行文件路径'
    )

    parser.add_argument(
        '--duration',
        type=float,
        default=DEFAULT_DURATION_HOURS,
        help=f'测试时长（小时），默认{DEFAULT_DURATION_HOURS}'
    )

    parser.add_argument(
        '--output-dir',
        default='./test_results',
        help='输出目录，默认./test_results'
    )

    parser.add_argument(
        '--auto-restart',
        action='store_true',
        help='进程崩溃后自动重启'
    )

    args = parser.parse_args()

    # 检查可执行文件
    if not os.path.exists(args.exe_path):
        print(f"错误: 可执行文件不存在: {args.exe_path}")
        return 1

    # 设置日志
    log_dir = Path(args.output_dir) / 'logs'
    logger = setup_logging(log_dir)

    # 运行测试
    manager = TestManager(args, logger)
    return manager.run()

if __name__ == '__main__':
    sys.exit(main())
