<template>
  <div class="performance-container">
    <el-row :gutter="16" class="header-row">
      <el-col :span="24">
        <h1>性能分析</h1>
      </el-col>
    </el-row>

    <!-- 时间范围选择 -->
    <el-row :gutter="16" class="filter-row">
      <el-col :span="24">
        <el-card shadow="hover">
          <el-form :inline="true">
            <el-form-item label="时间范围">
              <el-select v-model="timeRange" @change="onTimeRangeChange">
                <el-option label="最近5分钟" value="5m" />
                <el-option label="最近15分钟" value="15m" />
                <el-option label="最近1小时" value="1h" />
                <el-option label="最近6小时" value="6h" />
                <el-option label="最近24小时" value="24h" />
                <el-option label="自定义" value="custom" />
              </el-select>
            </el-form-item>

            <el-form-item v-if="timeRange === 'custom'" label="自定义范围">
              <el-date-picker
                v-model="customTimeRange"
                type="datetimerange"
                range-separator="至"
                start-placeholder="开始时间"
                end-placeholder="结束时间"
                @change="onCustomTimeChange"
              />
            </el-form-item>

            <el-form-item>
              <el-button type="primary" @click="refreshData">刷新数据</el-button>
              <el-button @click="exportData">导出CSV</el-button>
            </el-form-item>
          </el-form>
        </el-card>
      </el-col>
    </el-row>

    <!-- 统计摘要 -->
    <el-row :gutter="16" class="summary-row">
      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <div class="summary-item">
            <div class="summary-label">平均CPU使用率</div>
            <div class="summary-value">{{ avgCpu.toFixed(1) }}%</div>
          </div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <div class="summary-item">
            <div class="summary-label">平均内存使用率</div>
            <div class="summary-value">{{ avgMemory.toFixed(1) }}%</div>
          </div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <div class="summary-item">
            <div class="summary-label">平均帧率</div>
            <div class="summary-value">{{ avgFps.toFixed(0) }} FPS</div>
          </div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <div class="summary-item">
            <div class="summary-label">平均延迟</div>
            <div class="summary-value">{{ avgLatency.toFixed(0) }} ms</div>
          </div>
        </el-card>
      </el-col>
    </el-row>

    <!-- 综合性能图表 -->
    <el-row :gutter="16" class="chart-row">
      <el-col :span="24">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>综合性能趋势</span>
              <el-radio-group v-model="chartType" size="small">
                <el-radio-button label="line">折线图</el-radio-button>
                <el-radio-button label="bar">柱状图</el-radio-button>
              </el-radio-group>
            </div>
          </template>
          <div ref="combinedChartRef" class="chart-container-large"></div>
        </el-card>
      </el-col>
    </el-row>

    <!-- 详细数据表格 -->
    <el-row :gutter="16" class="table-row">
      <el-col :span="24">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>详细数据</span>
              <el-input
                v-model="searchText"
                placeholder="搜索..."
                clearable
                style="width: 200px"
              />
            </div>
          </template>
          <el-table
            :data="filteredMetrics"
            stripe
            border
            :max-height="400"
            style="width: 100%"
          >
            <el-table-column prop="timestamp" label="时间戳" width="180">
              <template #default="{ row }">
                {{ formatTimestamp(row.timestamp) }}
              </template>
            </el-table-column>
            <el-table-column prop="cpuUsage" label="CPU使用率 (%)" width="150">
              <template #default="{ row }">
                <el-tag :type="getCpuTagType(row.cpuUsage)">
                  {{ row.cpuUsage.toFixed(1) }}
                </el-tag>
              </template>
            </el-table-column>
            <el-table-column prop="memoryUsage" label="内存使用率 (%)" width="150">
              <template #default="{ row }">
                <el-tag :type="getMemoryTagType(row.memoryUsage)">
                  {{ row.memoryUsage.toFixed(1) }}
                </el-tag>
              </template>
            </el-table-column>
            <el-table-column prop="fps" label="帧率 (FPS)" width="120">
              <template #default="{ row }">
                <el-tag :type="getFpsTagType(row.fps)">
                  {{ row.fps.toFixed(0) }}
                </el-tag>
              </template>
            </el-table-column>
            <el-table-column prop="latency" label="延迟 (ms)" width="120">
              <template #default="{ row }">
                <el-tag :type="getLatencyTagType(row.latency)">
                  {{ row.latency.toFixed(0) }}
                </el-tag>
              </template>
            </el-table-column>
          </el-table>
        </el-card>
      </el-col>
    </el-row>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, onUnmounted } from 'vue'
import { storeToRefs } from 'pinia'
import * as echarts from 'echarts'
import type { EChartsOption } from 'echarts'
import { useMonitorStore } from '@/store/monitor'
import { ElMessage } from 'element-plus'

const monitorStore = useMonitorStore()
const { performanceMetrics } = storeToRefs(monitorStore)

const timeRange = ref('1h')
const customTimeRange = ref<[Date, Date]>()
const chartType = ref('line')
const searchText = ref('')

const combinedChartRef = ref<HTMLElement>()
let combinedChart: echarts.ECharts | null = null

// 计算平均值
const avgCpu = computed(() => {
  const metrics = filteredMetrics.value
  if (metrics.length === 0) return 0
  return metrics.reduce((sum, m) => sum + m.cpuUsage, 0) / metrics.length
})

const avgMemory = computed(() => {
  const metrics = filteredMetrics.value
  if (metrics.length === 0) return 0
  return metrics.reduce((sum, m) => sum + m.memoryUsage, 0) / metrics.length
})

const avgFps = computed(() => {
  const metrics = filteredMetrics.value
  if (metrics.length === 0) return 0
  return metrics.reduce((sum, m) => sum + m.fps, 0) / metrics.length
})

const avgLatency = computed(() => {
  const metrics = filteredMetrics.value
  if (metrics.length === 0) return 0
  return metrics.reduce((sum, m) => sum + m.latency, 0) / metrics.length
})

// 过滤指标数据
const filteredMetrics = computed(() => {
  let metrics = performanceMetrics.value

  // 时间范围过滤
  const now = new Date()
  let startTime: Date

  switch (timeRange.value) {
    case '5m':
      startTime = new Date(now.getTime() - 5 * 60 * 1000)
      break
    case '15m':
      startTime = new Date(now.getTime() - 15 * 60 * 1000)
      break
    case '1h':
      startTime = new Date(now.getTime() - 60 * 60 * 1000)
      break
    case '6h':
      startTime = new Date(now.getTime() - 6 * 60 * 60 * 1000)
      break
    case '24h':
      startTime = new Date(now.getTime() - 24 * 60 * 60 * 1000)
      break
    case 'custom':
      if (customTimeRange.value && customTimeRange.value.length === 2) {
        startTime = customTimeRange.value[0]
        const endTime = customTimeRange.value[1]
        metrics = metrics.filter(m => {
          const time = new Date(m.timestamp)
          return time >= startTime && time <= endTime
        })
        return metrics
      }
      return metrics
    default:
      startTime = new Date(now.getTime() - 60 * 60 * 1000)
  }

  metrics = metrics.filter(m => new Date(m.timestamp) >= startTime)

  // 文本搜索过滤
  if (searchText.value) {
    const search = searchText.value.toLowerCase()
    metrics = metrics.filter(m => m.timestamp.toLowerCase().includes(search))
  }

  return metrics
})

// 格式化时间戳
const formatTimestamp = (timestamp: string) => {
  const date = new Date(timestamp)
  return date.toLocaleString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit'
  })
}

// 获取标签类型
const getCpuTagType = (value: number) => {
  if (value > 80) return 'danger'
  if (value > 60) return 'warning'
  return 'success'
}

const getMemoryTagType = (value: number) => {
  if (value > 80) return 'danger'
  if (value > 60) return 'warning'
  return 'success'
}

const getFpsTagType = (value: number) => {
  if (value < 30) return 'danger'
  if (value < 50) return 'warning'
  return 'success'
}

const getLatencyTagType = (value: number) => {
  if (value > 100) return 'danger'
  if (value > 50) return 'warning'
  return 'success'
}

// 初始化综合图表
const initCombinedChart = () => {
  if (combinedChartRef.value) {
    combinedChart = echarts.init(combinedChartRef.value)
    updateCombinedChart()
  }
}

// 更新综合图表
const updateCombinedChart = () => {
  const metrics = filteredMetrics.value
  const timestamps = metrics.map(m => {
    const date = new Date(m.timestamp)
    return `${date.getHours().toString().padStart(2, '0')}:${date.getMinutes().toString().padStart(2, '0')}`
  })

  const option: EChartsOption = {
    title: {
      text: '综合性能指标',
      left: 'center'
    },
    tooltip: {
      trigger: 'axis',
      axisPointer: {
        type: 'cross'
      }
    },
    legend: {
      data: ['CPU使用率', '内存使用率', '帧率', '延迟'],
      top: 30
    },
    grid: {
      left: '3%',
      right: '4%',
      bottom: '3%',
      top: '15%',
      containLabel: true
    },
    xAxis: {
      type: 'category',
      boundaryGap: chartType.value === 'bar',
      data: timestamps
    },
    yAxis: [
      {
        type: 'value',
        name: '百分比 (%)',
        position: 'left',
        min: 0,
        max: 100
      },
      {
        type: 'value',
        name: 'FPS / 延迟',
        position: 'right'
      }
    ],
    series: [
      {
        name: 'CPU使用率',
        type: chartType.value as any,
        yAxisIndex: 0,
        data: metrics.map(m => m.cpuUsage.toFixed(1)),
        itemStyle: { color: '#409EFF' }
      },
      {
        name: '内存使用率',
        type: chartType.value as any,
        yAxisIndex: 0,
        data: metrics.map(m => m.memoryUsage.toFixed(1)),
        itemStyle: { color: '#67C23A' }
      },
      {
        name: '帧率',
        type: chartType.value as any,
        yAxisIndex: 1,
        data: metrics.map(m => m.fps.toFixed(0)),
        itemStyle: { color: '#E6A23C' }
      },
      {
        name: '延迟',
        type: chartType.value as any,
        yAxisIndex: 1,
        data: metrics.map(m => m.latency.toFixed(0)),
        itemStyle: { color: '#F56C6C' }
      }
    ]
  }

  combinedChart?.setOption(option)
}

// 时间范围变化
const onTimeRangeChange = () => {
  updateCombinedChart()
}

// 自定义时间变化
const onCustomTimeChange = () => {
  updateCombinedChart()
}

// 刷新数据
const refreshData = () => {
  updateCombinedChart()
  ElMessage.success('数据已刷新')
}

// 导出CSV
const exportData = () => {
  const metrics = filteredMetrics.value
  if (metrics.length === 0) {
    ElMessage.warning('没有可导出的数据')
    return
  }

  const csv = [
    ['时间戳', 'CPU使用率(%)', '内存使用率(%)', '帧率(FPS)', '延迟(ms)'],
    ...metrics.map(m => [
      m.timestamp,
      m.cpuUsage.toFixed(1),
      m.memoryUsage.toFixed(1),
      m.fps.toFixed(0),
      m.latency.toFixed(0)
    ])
  ].map(row => row.join(',')).join('\n')

  const blob = new Blob(['\ufeff' + csv], { type: 'text/csv;charset=utf-8;' })
  const link = document.createElement('a')
  link.href = URL.createObjectURL(blob)
  link.download = `performance_${new Date().toISOString()}.csv`
  link.click()

  ElMessage.success('数据已导出')
}

// 监听图表类型变化
const onChartTypeChange = () => {
  updateCombinedChart()
}

// 响应式调整图表大小
const resizeCharts = () => {
  combinedChart?.resize()
}

onMounted(() => {
  initCombinedChart()
  window.addEventListener('resize', resizeCharts)
})

onUnmounted(() => {
  combinedChart?.dispose()
  window.removeEventListener('resize', resizeCharts)
})
</script>

<style scoped>
.performance-container {
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

.filter-row {
  margin-bottom: 20px;
}

.summary-row {
  margin-bottom: 20px;
}

.summary-item {
  text-align: center;
  padding: 20px;
}

.summary-label {
  font-size: 14px;
  color: #909399;
  margin-bottom: 10px;
}

.summary-value {
  font-size: 32px;
  font-weight: bold;
  color: #303133;
}

.chart-row {
  margin-bottom: 20px;
}

.chart-container-large {
  width: 100%;
  height: 400px;
}

.table-row {
  margin-bottom: 20px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-weight: 600;
}

@media (max-width: 768px) {
  .performance-container {
    padding: 10px;
  }

  .header-row h1 {
    font-size: 24px;
  }

  .summary-value {
    font-size: 24px;
  }

  .chart-container-large {
    height: 300px;
  }
}
</style>
