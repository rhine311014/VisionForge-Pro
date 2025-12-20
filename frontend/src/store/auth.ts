/**
 * 认证状态管理
 */
import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import * as authApi from '@/api/auth'

export const useAuthStore = defineStore('auth', () => {
  // 状态
  const accessToken = ref<string | null>(null)
  const refreshToken = ref<string | null>(null)
  const userId = ref<string | null>(null)
  const username = ref<string | null>(null)
  const role = ref<string | null>(null)
  const tokenExpiry = ref<Date | null>(null)

  // 计算属性
  const isAuthenticated = computed(() => !!accessToken.value)
  const isAdmin = computed(() => role.value === 'Admin')
  const isOperator = computed(() => role.value === 'Operator' || role.value === 'Admin')

  /**
   * 登录
   */
  async function login(username: string, password: string): Promise<boolean> {
    try {
      const response = await authApi.login({ username, password })

      if (response.success && response.accessToken) {
        // 保存认证信息
        accessToken.value = response.accessToken
        refreshToken.value = response.refreshToken || null
        userId.value = response.userId || null
        username = response.username || username
        role.value = response.role || null
        tokenExpiry.value = response.tokenExpiry ? new Date(response.tokenExpiry) : null

        // 保存到localStorage
        saveToLocalStorage()

        return true
      } else {
        console.error('Login failed:', response.errorMessage)
        return false
      }
    } catch (error) {
      console.error('Login error:', error)
      return false
    }
  }

  /**
   * 登出
   */
  async function logout() {
    if (accessToken.value) {
      await authApi.logout(accessToken.value)
    }

    // 清除状态
    accessToken.value = null
    refreshToken.value = null
    userId.value = null
    username.value = null
    role.value = null
    tokenExpiry.value = null

    // 清除localStorage
    clearLocalStorage()
  }

  /**
   * 刷新Token
   */
  async function refresh(): Promise<boolean> {
    if (!refreshToken.value) {
      return false
    }

    try {
      const response = await authApi.refreshToken({ refreshToken: refreshToken.value })

      if (response.success && response.accessToken) {
        accessToken.value = response.accessToken
        tokenExpiry.value = response.tokenExpiry ? new Date(response.tokenExpiry) : null

        // 更新localStorage
        saveToLocalStorage()

        return true
      } else {
        console.error('Token refresh failed:', response.errorMessage)
        await logout()
        return false
      }
    } catch (error) {
      console.error('Token refresh error:', error)
      await logout()
      return false
    }
  }

  /**
   * 检查认证状态
   */
  function checkAuth() {
    // 从localStorage恢复
    loadFromLocalStorage()

    // 检查Token是否过期
    if (tokenExpiry.value && new Date() >= tokenExpiry.value) {
      // Token已过期，尝试刷新
      refresh()
    }
  }

  /**
   * 检查Token是否即将过期
   */
  function shouldRefreshToken(): boolean {
    if (!tokenExpiry.value) {
      return false
    }

    const now = new Date()
    const expiryTime = new Date(tokenExpiry.value)
    const remainingMs = expiryTime.getTime() - now.getTime()

    // 如果剩余时间少于5分钟，应该刷新
    return remainingMs < 5 * 60 * 1000
  }

  /**
   * 保存到localStorage
   */
  function saveToLocalStorage() {
    if (accessToken.value) {
      localStorage.setItem('accessToken', accessToken.value)
    }
    if (refreshToken.value) {
      localStorage.setItem('refreshToken', refreshToken.value)
    }
    if (userId.value) {
      localStorage.setItem('userId', userId.value)
    }
    if (username.value) {
      localStorage.setItem('username', username.value)
    }
    if (role.value) {
      localStorage.setItem('role', role.value)
    }
    if (tokenExpiry.value) {
      localStorage.setItem('tokenExpiry', tokenExpiry.value.toISOString())
    }
  }

  /**
   * 从localStorage加载
   */
  function loadFromLocalStorage() {
    accessToken.value = localStorage.getItem('accessToken')
    refreshToken.value = localStorage.getItem('refreshToken')
    userId.value = localStorage.getItem('userId')
    username.value = localStorage.getItem('username')
    role.value = localStorage.getItem('role')

    const expiry = localStorage.getItem('tokenExpiry')
    if (expiry) {
      tokenExpiry.value = new Date(expiry)
    }
  }

  /**
   * 清除localStorage
   */
  function clearLocalStorage() {
    localStorage.removeItem('accessToken')
    localStorage.removeItem('refreshToken')
    localStorage.removeItem('userId')
    localStorage.removeItem('username')
    localStorage.removeItem('role')
    localStorage.removeItem('tokenExpiry')
  }

  return {
    // 状态
    accessToken,
    refreshToken,
    userId,
    username,
    role,
    tokenExpiry,

    // 计算属性
    isAuthenticated,
    isAdmin,
    isOperator,

    // 方法
    login,
    logout,
    refresh,
    checkAuth,
    shouldRefreshToken
  }
})
