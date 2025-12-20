/**
 * 监控数据状态管理
 */
import { defineStore } from 'pinia'
import { ref } from 'vue'
import type { PerformanceMetrics, LogEntry, SystemStatus } from '@/utils/websocket'

export const useMonitorStore = defineStore('monitor', () => {
  // 性能指标
  const performanceMetrics = ref<PerformanceMetrics[]>([])
  const maxMetricsCount = 100 // 保留最近100条

  // 日志
  const logs = ref<LogEntry[]>([])
  const maxLogsCount = 500 // 保留最近500条

  // 系统状态
  const systemStatus = ref<SystemStatus | null>(null)

  // WebSocket连接状态
  const isConnected = ref(false)

  /**
   * 添加性能指标
   */
  function addPerformanceMetric(metric: PerformanceMetrics) {
    performanceMetrics.value.push(metric)

    // 保持数组大小
    if (performanceMetrics.value.length > maxMetricsCount) {
      performanceMetrics.value.shift()
    }
  }

  /**
   * 添加日志
   */
  function addLog(log: LogEntry) {
    logs.value.unshift(log) // 新日志在前面

    // 保持数组大小
    if (logs.value.length > maxLogsCount) {
      logs.value.pop()
    }
  }

  /**
   * 更新系统状态
   */
  function updateSystemStatus(status: SystemStatus) {
    systemStatus.value = status
  }

  /**
   * 设置连接状态
   */
  function setConnected(connected: boolean) {
    isConnected.value = connected
  }

  /**
   * 清除所有数据
   */
  function clearAll() {
    performanceMetrics.value = []
    logs.value = []
    systemStatus.value = null
  }

  /**
   * 获取最新的性能指标
   */
  function getLatestMetric(): PerformanceMetrics | null {
    if (performanceMetrics.value.length === 0) {
      return null
    }
    return performanceMetrics.value[performanceMetrics.value.length - 1]
  }

  /**
   * 获取指定时间范围的性能指标
   */
  function getMetricsInRange(startTime: Date, endTime: Date): PerformanceMetrics[] {
    return performanceMetrics.value.filter(metric => {
      const time = new Date(metric.timestamp)
      return time >= startTime && time <= endTime
    })
  }

  /**
   * 过滤日志
   */
  function filterLogs(level?: string, category?: string, keyword?: string): LogEntry[] {
    return logs.value.filter(log => {
      if (level && log.level !== level) return false
      if (category && log.category !== category) return false
      if (keyword && !log.message.includes(keyword)) return false
      return true
    })
  }

  return {
    // 状态
    performanceMetrics,
    logs,
    systemStatus,
    isConnected,

    // 方法
    addPerformanceMetric,
    addLog,
    updateSystemStatus,
    setConnected,
    clearAll,
    getLatestMetric,
    getMetricsInRange,
    filterLogs
  }
})
