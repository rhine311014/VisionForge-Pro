<template>
  <div class="dashboard-container">
    <el-row :gutter="16" class="header-row">
      <el-col :span="24">
        <h1>性能监控仪表盘</h1>
      </el-col>
    </el-row>

    <!-- 系统状态卡片 -->
    <el-row :gutter="16" class="status-cards">
      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>CPU使用率</span>
            </div>
          </template>
          <div class="metric-value" :class="getCpuClass()">
            {{ systemStatus?.cpuUsage.toFixed(1) ?? 0 }}%
          </div>
          <div class="metric-label">当前</div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>内存使用率</span>
            </div>
          </template>
          <div class="metric-value" :class="getMemoryClass()">
            {{ systemStatus?.memoryUsage.toFixed(1) ?? 0 }}%
          </div>
          <div class="metric-label">当前</div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>帧率</span>
            </div>
          </template>
          <div class="metric-value" :class="getFpsClass()">
            {{ systemStatus?.fps.toFixed(0) ?? 0 }}
          </div>
          <div class="metric-label">FPS</div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>网络延迟</span>
            </div>
          </template>
          <div class="metric-value" :class="getLatencyClass()">
            {{ systemStatus?.latency.toFixed(0) ?? 0 }}
          </div>
          <div class="metric-label">ms</div>
        </el-card>
      </el-col>
    </el-row>

    <!-- 趋势图表 -->
    <el-row :gutter="16" class="chart-row">
      <el-col :xs="24" :md="12">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>CPU使用率趋势</span>
              <el-tag type="info" size="small">最近60秒</el-tag>
            </div>
          </template>
          <div ref="cpuChartRef" class="chart-container"></div>
        </el-card>
      </el-col>

      <el-col :xs="24" :md="12">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>内存使用率趋势</span>
              <el-tag type="info" size="small">最近60秒</el-tag>
            </div>
          </template>
          <div ref="memoryChartRef" class="chart-container"></div>
        </el-card>
      </el-col>
    </el-row>

    <el-row :gutter="16" class="chart-row">
      <el-col :xs="24" :md="12">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>帧率趋势</span>
              <el-tag type="info" size="small">最近60秒</el-tag>
            </div>
          </template>
          <div ref="fpsChartRef" class="chart-container"></div>
        </el-card>
      </el-col>

      <el-col :xs="24" :md="12">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>延迟趋势</span>
              <el-tag type="info" size="small">最近60秒</el-tag>
            </div>
          </template>
          <div ref="latencyChartRef" class="chart-container"></div>
        </el-card>
      </el-col>
    </el-row>

    <!-- 连接状态 -->
    <el-row :gutter="16" class="footer-row">
      <el-col :span="24">
        <el-card shadow="hover">
          <div class="connection-status">
            <el-tag :type="wsClient?.isConnected ? 'success' : 'danger'" size="large">
              {{ wsClient?.isConnected ? '已连接' : '未连接' }}
            </el-tag>
            <span class="status-text">WebSocket连接状态</span>
          </div>
        </el-card>
      </el-col>
    </el-row>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted, computed } from 'vue'
import { storeToRefs } from 'pinia'
import * as echarts from 'echarts'
import type { EChartsOption } from 'echarts'
import { useMonitorStore } from '@/store/monitor'
import { WebSocketClient } from '@/utils/websocket'

const monitorStore = useMonitorStore()
const { performanceMetrics, systemStatus } = storeToRefs(monitorStore)

const cpuChartRef = ref<HTMLElement>()
const memoryChartRef = ref<HTMLElement>()
const fpsChartRef = ref<HTMLElement>()
const latencyChartRef = ref<HTMLElement>()

let cpuChart: echarts.ECharts | null = null
let memoryChart: echarts.ECharts | null = null
let fpsChart: echarts.ECharts | null = null
let latencyChart: echarts.ECharts | null = null

const wsClient = ref<WebSocketClient | null>(null)

// 获取CPU使用率颜色类
const getCpuClass = () => {
  const cpu = systemStatus.value?.cpuUsage ?? 0
  if (cpu > 80) return 'danger'
  if (cpu > 60) return 'warning'
  return 'normal'
}

// 获取内存使用率颜色类
const getMemoryClass = () => {
  const memory = systemStatus.value?.memoryUsage ?? 0
  if (memory > 80) return 'danger'
  if (memory > 60) return 'warning'
  return 'normal'
}

// 获取帧率颜色类
const getFpsClass = () => {
  const fps = systemStatus.value?.fps ?? 0
  if (fps < 30) return 'danger'
  if (fps < 50) return 'warning'
  return 'normal'
}

// 获取延迟颜色类
const getLatencyClass = () => {
  const latency = systemStatus.value?.latency ?? 0
  if (latency > 100) return 'danger'
  if (latency > 50) return 'warning'
  return 'normal'
}

// 初始化图表
const initCharts = () => {
  if (cpuChartRef.value) {
    cpuChart = echarts.init(cpuChartRef.value)
    cpuChart.setOption(getLineChartOption('CPU使用率', '%', '#409EFF'))
  }

  if (memoryChartRef.value) {
    memoryChart = echarts.init(memoryChartRef.value)
    memoryChart.setOption(getLineChartOption('内存使用率', '%', '#67C23A'))
  }

  if (fpsChartRef.value) {
    fpsChart = echarts.init(fpsChartRef.value)
    fpsChart.setOption(getLineChartOption('帧率', 'FPS', '#E6A23C'))
  }

  if (latencyChartRef.value) {
    latencyChart = echarts.init(latencyChartRef.value)
    latencyChart.setOption(getLineChartOption('延迟', 'ms', '#F56C6C'))
  }
}

// 获取折线图配置
const getLineChartOption = (name: string, unit: string, color: string): EChartsOption => {
  return {
    title: {
      show: false
    },
    tooltip: {
      trigger: 'axis',
      formatter: (params: any) => {
        const data = params[0]
        return `${data.name}<br/>${data.seriesName}: ${data.value} ${unit}`
      }
    },
    grid: {
      left: '3%',
      right: '4%',
      bottom: '3%',
      top: '10%',
      containLabel: true
    },
    xAxis: {
      type: 'category',
      boundaryGap: false,
      data: []
    },
    yAxis: {
      type: 'value',
      name: unit,
      min: 0,
      max: unit === '%' ? 100 : undefined
    },
    series: [
      {
        name: name,
        type: 'line',
        smooth: true,
        symbol: 'circle',
        symbolSize: 6,
        itemStyle: {
          color: color
        },
        areaStyle: {
          color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [
            { offset: 0, color: color + '40' },
            { offset: 1, color: color + '10' }
          ])
        },
        data: []
      }
    ]
  }
}

// 更新图表数据
const updateCharts = () => {
  const metrics = performanceMetrics.value.slice(-60) // 最近60个数据点

  const timestamps = metrics.map(m => {
    const date = new Date(m.timestamp)
    return `${date.getHours().toString().padStart(2, '0')}:${date.getMinutes().toString().padStart(2, '0')}:${date.getSeconds().toString().padStart(2, '0')}`
  })

  // 更新CPU图表
  if (cpuChart) {
    cpuChart.setOption({
      xAxis: { data: timestamps },
      series: [{ data: metrics.map(m => m.cpuUsage.toFixed(1)) }]
    })
  }

  // 更新内存图表
  if (memoryChart) {
    memoryChart.setOption({
      xAxis: { data: timestamps },
      series: [{ data: metrics.map(m => m.memoryUsage.toFixed(1)) }]
    })
  }

  // 更新FPS图表
  if (fpsChart) {
    fpsChart.setOption({
      xAxis: { data: timestamps },
      series: [{ data: metrics.map(m => m.fps.toFixed(0)) }]
    })
  }

  // 更新延迟图表
  if (latencyChart) {
    latencyChart.setOption({
      xAxis: { data: timestamps },
      series: [{ data: metrics.map(m => m.latency.toFixed(0)) }]
    })
  }
}

// 连接WebSocket
const connectWebSocket = async () => {
  try {
    const wsUrl = import.meta.env.VITE_WS_URL || 'ws://localhost:8765/ws'
    wsClient.value = new WebSocketClient(wsUrl)

    await wsClient.value.connect()
    console.log('WebSocket连接成功')

    // 订阅性能数据
    wsClient.value.subscribe(['performance', 'system_status'])

    // 监听性能指标消息
    wsClient.value.on('performance', (data: any) => {
      monitorStore.addPerformanceMetric({
        timestamp: data.timestamp || new Date().toISOString(),
        cpuUsage: data.cpuUsage || 0,
        memoryUsage: data.memoryUsage || 0,
        fps: data.fps || 0,
        latency: data.latency || 0
      })
      updateCharts()
    })

    // 监听系统状态消息
    wsClient.value.on('system_status', (data: any) => {
      monitorStore.systemStatus = data
    })
  } catch (error) {
    console.error('WebSocket连接失败:', error)
  }
}

// 响应式调整图表大小
const resizeCharts = () => {
  cpuChart?.resize()
  memoryChart?.resize()
  fpsChart?.resize()
  latencyChart?.resize()
}

onMounted(() => {
  initCharts()
  connectWebSocket()
  window.addEventListener('resize', resizeCharts)
})

onUnmounted(() => {
  cpuChart?.dispose()
  memoryChart?.dispose()
  fpsChart?.dispose()
  latencyChart?.dispose()
  wsClient.value?.disconnect()
  window.removeEventListener('resize', resizeCharts)
})
</script>

<style scoped>
.dashboard-container {
  padding: 20px;
  background-color: #f5f7fa;
  min-height: 100vh;
}

.header-row {
  margin-bottom: 20px;
}

.header-row h1 {
  margin: 0;
  font-size: 28px;
  color: #303133;
}

.status-cards {
  margin-bottom: 20px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-weight: 600;
}

.metric-value {
  font-size: 48px;
  font-weight: bold;
  text-align: center;
  margin: 20px 0 10px;
  transition: color 0.3s;
}

.metric-value.normal {
  color: #67C23A;
}

.metric-value.warning {
  color: #E6A23C;
}

.metric-value.danger {
  color: #F56C6C;
}

.metric-label {
  text-align: center;
  color: #909399;
  font-size: 14px;
}

.chart-row {
  margin-bottom: 20px;
}

.chart-container {
  width: 100%;
  height: 300px;
}

.footer-row {
  margin-top: 20px;
}

.connection-status {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 16px;
  padding: 10px;
}

.status-text {
  font-size: 16px;
  color: #606266;
}

@media (max-width: 768px) {
  .dashboard-container {
    padding: 10px;
  }

  .header-row h1 {
    font-size: 24px;
  }

  .metric-value {
    font-size: 36px;
  }

  .chart-container {
    height: 250px;
  }
}
</style>
