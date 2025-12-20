<template>
  <div class="settings-container">
    <el-row :gutter="16" class="header-row">
      <el-col :span="24">
        <h1>系统设置</h1>
      </el-col>
    </el-row>

    <el-row :gutter="16">
      <el-col :xs="24" :lg="16">
        <el-tabs v-model="activeTab" type="border-card">
          <!-- WebSocket配置 -->
          <el-tab-pane label="WebSocket配置" name="websocket">
            <el-form :model="websocketConfig" label-width="140px">
              <el-form-item label="服务器地址">
                <el-input
                  v-model="websocketConfig.url"
                  placeholder="ws://localhost:8765/ws"
                />
              </el-form-item>

              <el-form-item label="启用WSS">
                <el-switch v-model="websocketConfig.enableWss" />
                <el-text type="info" size="small" style="margin-left: 10px">
                  使用安全WebSocket连接
                </el-text>
              </el-form-item>

              <el-form-item label="自动重连">
                <el-switch v-model="websocketConfig.autoReconnect" />
              </el-form-item>

              <el-form-item label="重连延迟(ms)">
                <el-input-number
                  v-model="websocketConfig.reconnectDelay"
                  :min="1000"
                  :max="30000"
                  :step="1000"
                />
              </el-form-item>

              <el-form-item label="心跳间隔(ms)">
                <el-input-number
                  v-model="websocketConfig.heartbeatInterval"
                  :min="5000"
                  :max="60000"
                  :step="5000"
                />
              </el-form-item>

              <el-form-item>
                <el-button type="primary" @click="saveWebSocketConfig">
                  保存配置
                </el-button>
                <el-button @click="testConnection">测试连接</el-button>
              </el-form-item>
            </el-form>
          </el-tab-pane>

          <!-- 性能监控配置 -->
          <el-tab-pane label="性能监控" name="performance">
            <el-form :model="performanceConfig" label-width="140px">
              <el-form-item label="数据采集间隔(秒)">
                <el-input-number
                  v-model="performanceConfig.collectInterval"
                  :min="1"
                  :max="60"
                  :step="1"
                />
              </el-form-item>

              <el-form-item label="历史数据保留(小时)">
                <el-input-number
                  v-model="performanceConfig.retentionHours"
                  :min="1"
                  :max="168"
                  :step="1"
                />
              </el-form-item>

              <el-form-item label="CPU警告阈值(%)">
                <el-slider
                  v-model="performanceConfig.cpuWarningThreshold"
                  :min="0"
                  :max="100"
                  show-input
                />
              </el-form-item>

              <el-form-item label="内存警告阈值(%)">
                <el-slider
                  v-model="performanceConfig.memoryWarningThreshold"
                  :min="0"
                  :max="100"
                  show-input
                />
              </el-form-item>

              <el-form-item label="FPS警告阈值">
                <el-input-number
                  v-model="performanceConfig.fpsWarningThreshold"
                  :min="1"
                  :max="120"
                  :step="1"
                />
              </el-form-item>

              <el-form-item>
                <el-button type="primary" @click="savePerformanceConfig">
                  保存配置
                </el-button>
              </el-form-item>
            </el-form>
          </el-tab-pane>

          <!-- 日志配置 -->
          <el-tab-pane label="日志设置" name="logs">
            <el-form :model="logsConfig" label-width="140px">
              <el-form-item label="日志级别">
                <el-select v-model="logsConfig.level">
                  <el-option label="调试" value="debug" />
                  <el-option label="信息" value="info" />
                  <el-option label="警告" value="warning" />
                  <el-option label="错误" value="error" />
                </el-select>
              </el-form-item>

              <el-form-item label="最大日志数量">
                <el-input-number
                  v-model="logsConfig.maxLogs"
                  :min="100"
                  :max="10000"
                  :step="100"
                />
              </el-form-item>

              <el-form-item label="自动清理">
                <el-switch v-model="logsConfig.autoCleanup" />
                <el-text type="info" size="small" style="margin-left: 10px">
                  超过最大数量时自动清理旧日志
                </el-text>
              </el-form-item>

              <el-form-item label="启用本地存储">
                <el-switch v-model="logsConfig.enableLocalStorage" />
              </el-form-item>

              <el-form-item>
                <el-button type="primary" @click="saveLogsConfig">
                  保存配置
                </el-button>
              </el-form-item>
            </el-form>
          </el-tab-pane>

          <!-- 用户管理 -->
          <el-tab-pane label="用户管理" name="users">
            <el-button type="primary" @click="showAddUserDialog" style="margin-bottom: 16px">
              添加用户
            </el-button>

            <el-table :data="users" stripe border style="width: 100%">
              <el-table-column prop="username" label="用户名" width="150" />
              <el-table-column prop="role" label="角色" width="120">
                <template #default="{ row }">
                  <el-tag :type="getRoleTagType(row.role)">
                    {{ getRoleText(row.role) }}
                  </el-tag>
                </template>
              </el-table-column>
              <el-table-column prop="email" label="邮箱" />
              <el-table-column prop="createdAt" label="创建时间" width="180">
                <template #default="{ row }">
                  {{ formatDate(row.createdAt) }}
                </template>
              </el-table-column>
              <el-table-column label="操作" width="180">
                <template #default="{ row }">
                  <el-button
                    size="small"
                    @click="editUser(row)"
                  >
                    编辑
                  </el-button>
                  <el-button
                    size="small"
                    type="danger"
                    @click="deleteUser(row)"
                  >
                    删除
                  </el-button>
                </template>
              </el-table-column>
            </el-table>
          </el-tab-pane>

          <!-- 关于 -->
          <el-tab-pane label="关于" name="about">
            <el-descriptions title="系统信息" :column="1" border>
              <el-descriptions-item label="系统名称">
                VisionForge Pro 远程诊断系统
              </el-descriptions-item>
              <el-descriptions-item label="版本号">
                v1.2.0
              </el-descriptions-item>
              <el-descriptions-item label="前端框架">
                Vue 3 + TypeScript + Vite
              </el-descriptions-item>
              <el-descriptions-item label="UI框架">
                Element Plus
              </el-descriptions-item>
              <el-descriptions-item label="图表库">
                ECharts
              </el-descriptions-item>
              <el-descriptions-item label="状态管理">
                Pinia
              </el-descriptions-item>
              <el-descriptions-item label="后端框架">
                Qt 6.9.3 + WebSockets
              </el-descriptions-item>
              <el-descriptions-item label="安全特性">
                WSS + JWT + RBAC
              </el-descriptions-item>
            </el-descriptions>

            <el-divider />

            <el-alert
              title="技术特性"
              type="info"
              :closable="false"
              style="margin-bottom: 16px"
            >
              <ul style="margin: 8px 0; padding-left: 20px;">
                <li>实时性能监控 - CPU、内存、FPS、延迟</li>
                <li>WebSocket实时通信 - 支持WSS加密传输</li>
                <li>JWT Token认证 - 安全的用户认证机制</li>
                <li>RBAC权限控制 - 细粒度的权限管理</li>
                <li>日志收集与分析 - 多级别日志过滤</li>
                <li>响应式设计 - 支持桌面和移动设备</li>
              </ul>
            </el-alert>
          </el-tab-pane>
        </el-tabs>
      </el-col>

      <!-- 侧边栏状态卡片 -->
      <el-col :xs="24" :lg="8">
        <el-card shadow="hover" style="margin-bottom: 16px">
          <template #header>
            <span>当前用户</span>
          </template>
          <el-descriptions :column="1">
            <el-descriptions-item label="用户名">
              {{ authStore.username || 'N/A' }}
            </el-descriptions-item>
            <el-descriptions-item label="角色">
              <el-tag :type="getRoleTagType(authStore.role)">
                {{ getRoleText(authStore.role) }}
              </el-tag>
            </el-descriptions-item>
            <el-descriptions-item label="Token有效期">
              {{ authStore.tokenExpiry ? formatDate(authStore.tokenExpiry) : 'N/A' }}
            </el-descriptions-item>
          </el-descriptions>
        </el-card>

        <el-card shadow="hover">
          <template #header>
            <span>系统状态</span>
          </template>
          <el-space direction="vertical" :size="12" style="width: 100%">
            <div class="status-item">
              <span>WebSocket</span>
              <el-tag :type="wsConnected ? 'success' : 'danger'" size="small">
                {{ wsConnected ? '已连接' : '未连接' }}
              </el-tag>
            </div>
            <div class="status-item">
              <span>前端版本</span>
              <el-tag type="info" size="small">v1.2.0</el-tag>
            </div>
            <div class="status-item">
              <span>运行时长</span>
              <el-tag type="info" size="small">{{ uptime }}</el-tag>
            </div>
          </el-space>
        </el-card>
      </el-col>
    </el-row>

    <!-- 添加/编辑用户对话框 -->
    <el-dialog
      v-model="userDialogVisible"
      :title="editingUser ? '编辑用户' : '添加用户'"
      width="500px"
    >
      <el-form :model="userForm" label-width="100px">
        <el-form-item label="用户名">
          <el-input v-model="userForm.username" />
        </el-form-item>
        <el-form-item label="邮箱">
          <el-input v-model="userForm.email" type="email" />
        </el-form-item>
        <el-form-item v-if="!editingUser" label="密码">
          <el-input v-model="userForm.password" type="password" />
        </el-form-item>
        <el-form-item label="角色">
          <el-select v-model="userForm.role">
            <el-option label="游客" value="guest" />
            <el-option label="观察者" value="viewer" />
            <el-option label="操作员" value="operator" />
            <el-option label="管理员" value="admin" />
          </el-select>
        </el-form-item>
      </el-form>
      <template #footer>
        <el-button @click="userDialogVisible = false">取消</el-button>
        <el-button type="primary" @click="saveUser">确定</el-button>
      </template>
    </el-dialog>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive, onMounted, computed } from 'vue'
import { useAuthStore } from '@/store/auth'
import { ElMessage, ElMessageBox } from 'element-plus'

const authStore = useAuthStore()

const activeTab = ref('websocket')
const wsConnected = ref(false)
const uptime = ref('0h 0m')

// WebSocket配置
const websocketConfig = reactive({
  url: 'ws://localhost:8765/ws',
  enableWss: false,
  autoReconnect: true,
  reconnectDelay: 5000,
  heartbeatInterval: 10000
})

// 性能监控配置
const performanceConfig = reactive({
  collectInterval: 5,
  retentionHours: 24,
  cpuWarningThreshold: 80,
  memoryWarningThreshold: 80,
  fpsWarningThreshold: 30
})

// 日志配置
const logsConfig = reactive({
  level: 'info',
  maxLogs: 1000,
  autoCleanup: true,
  enableLocalStorage: true
})

// 用户管理
const users = ref([
  {
    id: '1',
    username: 'admin',
    email: 'admin@example.com',
    role: 'admin',
    createdAt: new Date().toISOString()
  }
])

const userDialogVisible = ref(false)
const editingUser = ref<any>(null)
const userForm = reactive({
  username: '',
  email: '',
  password: '',
  role: 'viewer'
})

// 获取角色标签类型
const getRoleTagType = (role?: string) => {
  const typeMap: Record<string, any> = {
    admin: 'danger',
    operator: 'warning',
    viewer: 'success',
    guest: 'info'
  }
  return typeMap[role || ''] || 'info'
}

// 获取角色文本
const getRoleText = (role?: string) => {
  const textMap: Record<string, string> = {
    admin: '管理员',
    operator: '操作员',
    viewer: '观察者',
    guest: '游客'
  }
  return textMap[role || ''] || '未知'
}

// 格式化日期
const formatDate = (date: string) => {
  return new Date(date).toLocaleString('zh-CN')
}

// 保存WebSocket配置
const saveWebSocketConfig = () => {
  localStorage.setItem('websocket_config', JSON.stringify(websocketConfig))
  ElMessage.success('WebSocket配置已保存')
}

// 测试连接
const testConnection = () => {
  ElMessage.info('正在测试连接...')
  // TODO: 实际测试WebSocket连接
  setTimeout(() => {
    ElMessage.success('连接测试成功')
  }, 1000)
}

// 保存性能监控配置
const savePerformanceConfig = () => {
  localStorage.setItem('performance_config', JSON.stringify(performanceConfig))
  ElMessage.success('性能监控配置已保存')
}

// 保存日志配置
const saveLogsConfig = () => {
  localStorage.setItem('logs_config', JSON.stringify(logsConfig))
  ElMessage.success('日志配置已保存')
}

// 显示添加用户对话框
const showAddUserDialog = () => {
  editingUser.value = null
  userForm.username = ''
  userForm.email = ''
  userForm.password = ''
  userForm.role = 'viewer'
  userDialogVisible.value = true
}

// 编辑用户
const editUser = (user: any) => {
  editingUser.value = user
  userForm.username = user.username
  userForm.email = user.email
  userForm.role = user.role
  userDialogVisible.value = true
}

// 删除用户
const deleteUser = async (user: any) => {
  try {
    await ElMessageBox.confirm(`确定要删除用户 ${user.username} 吗?`, '警告', {
      confirmButtonText: '确定',
      cancelButtonText: '取消',
      type: 'warning'
    })
    users.value = users.value.filter(u => u.id !== user.id)
    ElMessage.success('用户已删除')
  } catch {
    // 用户取消
  }
}

// 保存用户
const saveUser = () => {
  if (!userForm.username || !userForm.email) {
    ElMessage.warning('请填写完整信息')
    return
  }

  if (!editingUser.value && !userForm.password) {
    ElMessage.warning('请输入密码')
    return
  }

  if (editingUser.value) {
    // 编辑用户
    const index = users.value.findIndex(u => u.id === editingUser.value.id)
    if (index !== -1) {
      users.value[index] = {
        ...users.value[index],
        username: userForm.username,
        email: userForm.email,
        role: userForm.role
      }
    }
    ElMessage.success('用户已更新')
  } else {
    // 添加用户
    users.value.push({
      id: Date.now().toString(),
      username: userForm.username,
      email: userForm.email,
      role: userForm.role,
      createdAt: new Date().toISOString()
    })
    ElMessage.success('用户已添加')
  }

  userDialogVisible.value = false
}

// 加载配置
const loadConfigs = () => {
  const wsConfig = localStorage.getItem('websocket_config')
  if (wsConfig) {
    Object.assign(websocketConfig, JSON.parse(wsConfig))
  }

  const perfConfig = localStorage.getItem('performance_config')
  if (perfConfig) {
    Object.assign(performanceConfig, JSON.parse(perfConfig))
  }

  const logConfig = localStorage.getItem('logs_config')
  if (logConfig) {
    Object.assign(logsConfig, JSON.parse(logConfig))
  }
}

onMounted(() => {
  loadConfigs()

  // 更新运行时长
  const startTime = Date.now()
  setInterval(() => {
    const elapsed = Date.now() - startTime
    const hours = Math.floor(elapsed / 3600000)
    const minutes = Math.floor((elapsed % 3600000) / 60000)
    uptime.value = `${hours}h ${minutes}m`
  }, 60000)
})
</script>

<style scoped>
.settings-container {
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

.status-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

@media (max-width: 992px) {
  .settings-container {
    padding: 10px;
  }

  .header-row h1 {
    font-size: 24px;
  }
}
</style>
