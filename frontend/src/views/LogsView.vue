<template>
  <div class="logs-container">
    <el-row :gutter="16" class="header-row">
      <el-col :span="24">
        <h1>系统日志</h1>
      </el-col>
    </el-row>

    <!-- 过滤器 -->
    <el-row :gutter="16" class="filter-row">
      <el-col :span="24">
        <el-card shadow="hover">
          <el-form :inline="true">
            <el-form-item label="日志级别">
              <el-select
                v-model="selectedLevel"
                placeholder="全部"
                clearable
                @change="onFilterChange"
              >
                <el-option label="调试" value="debug" />
                <el-option label="信息" value="info" />
                <el-option label="警告" value="warning" />
                <el-option label="错误" value="error" />
                <el-option label="严重错误" value="critical" />
              </el-select>
            </el-form-item>

            <el-form-item label="日志分类">
              <el-select
                v-model="selectedCategory"
                placeholder="全部"
                clearable
                @change="onFilterChange"
              >
                <el-option label="系统" value="system" />
                <el-option label="网络" value="network" />
                <el-option label="数据库" value="database" />
                <el-option label="认证" value="auth" />
                <el-option label="性能" value="performance" />
                <el-option label="业务" value="business" />
              </el-select>
            </el-form-item>

            <el-form-item label="搜索">
              <el-input
                v-model="searchKeyword"
                placeholder="搜索日志内容..."
                clearable
                @input="onFilterChange"
                style="width: 300px"
              >
                <template #prefix>
                  <el-icon><Search /></el-icon>
                </template>
              </el-input>
            </el-form-item>

            <el-form-item>
              <el-button type="primary" @click="clearLogs">清空日志</el-button>
              <el-button @click="exportLogs">导出日志</el-button>
              <el-button @click="toggleAutoScroll">
                {{ autoScroll ? '停止自动滚动' : '启用自动滚动' }}
              </el-button>
            </el-form-item>
          </el-form>
        </el-card>
      </el-col>
    </el-row>

    <!-- 日志统计 -->
    <el-row :gutter="16" class="stats-row">
      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <div class="stat-item">
            <el-icon :size="24" color="#909399"><Document /></el-icon>
            <div class="stat-content">
              <div class="stat-label">总计</div>
              <div class="stat-value">{{ totalLogs }}</div>
            </div>
          </div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <div class="stat-item">
            <el-icon :size="24" color="#F56C6C"><Warning /></el-icon>
            <div class="stat-content">
              <div class="stat-label">错误</div>
              <div class="stat-value error">{{ errorCount }}</div>
            </div>
          </div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <div class="stat-item">
            <el-icon :size="24" color="#E6A23C"><InfoFilled /></el-icon>
            <div class="stat-content">
              <div class="stat-label">警告</div>
              <div class="stat-value warning">{{ warningCount }}</div>
            </div>
          </div>
        </el-card>
      </el-col>

      <el-col :xs="24" :sm="12" :md="6">
        <el-card shadow="hover">
          <div class="stat-item">
            <el-icon :size="24" color="#67C23A"><SuccessFilled /></el-icon>
            <div class="stat-content">
              <div class="stat-label">信息</div>
              <div class="stat-value info">{{ infoCount }}</div>
            </div>
          </div>
        </el-card>
      </el-col>
    </el-row>

    <!-- 日志列表 -->
    <el-row :gutter="16" class="logs-row">
      <el-col :span="24">
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span>日志记录 ({{ filteredLogs.length }}条)</span>
              <el-tag :type="autoScroll ? 'success' : 'info'" size="small">
                {{ autoScroll ? '自动滚动' : '已暂停' }}
              </el-tag>
            </div>
          </template>
          <div ref="logsContainerRef" class="logs-list">
            <el-timeline>
              <el-timeline-item
                v-for="log in filteredLogs"
                :key="log.id"
                :timestamp="formatTimestamp(log.timestamp)"
                :type="getLogType(log.level)"
                placement="top"
              >
                <el-card :class="['log-card', `log-${log.level}`]">
                  <div class="log-header">
                    <el-tag :type="getLevelTagType(log.level)" size="small">
                      {{ getLevelText(log.level) }}
                    </el-tag>
                    <el-tag type="info" size="small">{{ log.category }}</el-tag>
                  </div>
                  <div class="log-message">{{ log.message }}</div>
                  <div v-if="log.details" class="log-details">
                    <el-collapse>
                      <el-collapse-item title="详细信息">
                        <pre>{{ formatDetails(log.details) }}</pre>
                      </el-collapse-item>
                    </el-collapse>
                  </div>
                </el-card>
              </el-timeline-item>
            </el-timeline>
            <div v-if="filteredLogs.length === 0" class="empty-logs">
              <el-empty description="暂无日志记录" />
            </div>
          </div>
        </el-card>
      </el-col>
    </el-row>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, onUnmounted, nextTick, watch } from 'vue'
import { storeToRefs } from 'pinia'
import { useMonitorStore } from '@/store/monitor'
import { ElMessage, ElMessageBox } from 'element-plus'
import {
  Search,
  Document,
  Warning,
  InfoFilled,
  SuccessFilled
} from '@element-plus/icons-vue'
import { WebSocketClient } from '@/utils/websocket'

const monitorStore = useMonitorStore()
const { logs } = storeToRefs(monitorStore)

const selectedLevel = ref<string>()
const selectedCategory = ref<string>()
const searchKeyword = ref('')
const autoScroll = ref(true)

const logsContainerRef = ref<HTMLElement>()
const wsClient = ref<WebSocketClient | null>(null)

// 过滤后的日志
const filteredLogs = computed(() => {
  return monitorStore.filterLogs(
    selectedLevel.value,
    selectedCategory.value,
    searchKeyword.value
  )
})

// 总日志数
const totalLogs = computed(() => logs.value.length)

// 错误数量
const errorCount = computed(() => {
  return logs.value.filter(log => log.level === 'error' || log.level === 'critical').length
})

// 警告数量
const warningCount = computed(() => {
  return logs.value.filter(log => log.level === 'warning').length
})

// 信息数量
const infoCount = computed(() => {
  return logs.value.filter(log => log.level === 'info').length
})

// 格式化时间戳
const formatTimestamp = (timestamp: string) => {
  const date = new Date(timestamp)
  return date.toLocaleString('zh-CN', {
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit'
  })
}

// 获取日志类型
const getLogType = (level: string) => {
  const typeMap: Record<string, string> = {
    debug: 'info',
    info: 'success',
    warning: 'warning',
    error: 'danger',
    critical: 'danger'
  }
  return typeMap[level] || 'info'
}

// 获取级别标签类型
const getLevelTagType = (level: string) => {
  const typeMap: Record<string, any> = {
    debug: 'info',
    info: 'success',
    warning: 'warning',
    error: 'danger',
    critical: 'danger'
  }
  return typeMap[level] || 'info'
}

// 获取级别文本
const getLevelText = (level: string) => {
  const textMap: Record<string, string> = {
    debug: '调试',
    info: '信息',
    warning: '警告',
    error: '错误',
    critical: '严重错误'
  }
  return textMap[level] || level
}

// 格式化详细信息
const formatDetails = (details: any) => {
  if (typeof details === 'string') return details
  return JSON.stringify(details, null, 2)
}

// 过滤变化
const onFilterChange = () => {
  // 过滤逻辑在computed中自动处理
}

// 清空日志
const clearLogs = async () => {
  try {
    await ElMessageBox.confirm('确定要清空所有日志吗?', '警告', {
      confirmButtonText: '确定',
      cancelButtonText: '取消',
      type: 'warning'
    })
    monitorStore.logs = []
    ElMessage.success('日志已清空')
  } catch {
    // 用户取消
  }
}

// 导出日志
const exportLogs = () => {
  const logsToExport = filteredLogs.value
  if (logsToExport.length === 0) {
    ElMessage.warning('没有可导出的日志')
    return
  }

  const csv = [
    ['时间戳', '级别', '分类', '消息', '详细信息'],
    ...logsToExport.map(log => [
      log.timestamp,
      getLevelText(log.level),
      log.category,
      log.message,
      log.details ? formatDetails(log.details) : ''
    ])
  ].map(row => row.map(cell => `"${cell}"`).join(',')).join('\n')

  const blob = new Blob(['\ufeff' + csv], { type: 'text/csv;charset=utf-8;' })
  const link = document.createElement('a')
  link.href = URL.createObjectURL(blob)
  link.download = `logs_${new Date().toISOString()}.csv`
  link.click()

  ElMessage.success('日志已导出')
}

// 切换自动滚动
const toggleAutoScroll = () => {
  autoScroll.value = !autoScroll.value
  ElMessage.info(autoScroll.value ? '已启用自动滚动' : '已停止自动滚动')
}

// 滚动到底部
const scrollToBottom = async () => {
  if (!autoScroll.value) return
  await nextTick()
  if (logsContainerRef.value) {
    logsContainerRef.value.scrollTop = logsContainerRef.value.scrollHeight
  }
}

// 监听日志变化
watch(
  () => logs.value.length,
  () => {
    scrollToBottom()
  }
)

// 连接WebSocket
const connectWebSocket = async () => {
  try {
    const wsUrl = import.meta.env.VITE_WS_URL || 'ws://localhost:8765/ws'
    wsClient.value = new WebSocketClient(wsUrl)

    await wsClient.value.connect()
    console.log('WebSocket连接成功')

    // 订阅日志消息
    wsClient.value.subscribe(['log'])

    // 监听日志消息
    wsClient.value.on('log', (data: any) => {
      monitorStore.addLog({
        id: Date.now().toString(),
        timestamp: data.timestamp || new Date().toISOString(),
        level: data.level || 'info',
        category: data.category || 'system',
        message: data.message || '',
        details: data.details
      })
    })
  } catch (error) {
    console.error('WebSocket连接失败:', error)
  }
}

onMounted(() => {
  connectWebSocket()
})

onUnmounted(() => {
  wsClient.value?.disconnect()
})
</script>

<style scoped>
.logs-container {
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

.stats-row {
  margin-bottom: 20px;
}

.stat-item {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 16px;
}

.stat-content {
  flex: 1;
}

.stat-label {
  font-size: 14px;
  color: #909399;
  margin-bottom: 4px;
}

.stat-value {
  font-size: 24px;
  font-weight: bold;
  color: #303133;
}

.stat-value.error {
  color: #F56C6C;
}

.stat-value.warning {
  color: #E6A23C;
}

.stat-value.info {
  color: #67C23A;
}

.logs-row {
  margin-bottom: 20px;
}

.logs-list {
  max-height: 600px;
  overflow-y: auto;
  padding: 16px;
}

.log-card {
  margin-bottom: 8px;
  border-left: 4px solid transparent;
}

.log-card.log-debug {
  border-left-color: #909399;
}

.log-card.log-info {
  border-left-color: #67C23A;
}

.log-card.log-warning {
  border-left-color: #E6A23C;
}

.log-card.log-error,
.log-card.log-critical {
  border-left-color: #F56C6C;
}

.log-header {
  display: flex;
  gap: 8px;
  margin-bottom: 8px;
}

.log-message {
  font-size: 14px;
  color: #303133;
  margin-bottom: 8px;
  word-break: break-all;
}

.log-details pre {
  background-color: #f5f7fa;
  padding: 12px;
  border-radius: 4px;
  overflow-x: auto;
  font-size: 12px;
  color: #606266;
  margin: 0;
}

.empty-logs {
  padding: 40px 0;
  text-align: center;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-weight: 600;
}

@media (max-width: 768px) {
  .logs-container {
    padding: 10px;
  }

  .header-row h1 {
    font-size: 24px;
  }

  .stat-value {
    font-size: 20px;
  }

  .logs-list {
    max-height: 400px;
  }
}
</style>
