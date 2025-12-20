/**
 * 认证API
 */
import axios from 'axios'

const API_BASE = import.meta.env.VITE_API_BASE || 'http://localhost:8080/api'

export interface LoginRequest {
  username: string
  password: string
}

export interface LoginResponse {
  success: boolean
  userId?: string
  username?: string
  role?: string
  accessToken?: string
  refreshToken?: string
  tokenExpiry?: string
  errorMessage?: string
}

export interface RefreshTokenRequest {
  refreshToken: string
}

export interface RefreshTokenResponse {
  success: boolean
  accessToken?: string
  tokenExpiry?: string
  errorMessage?: string
}

/**
 * 登录
 */
export async function login(request: LoginRequest): Promise<LoginResponse> {
  try {
    const response = await axios.post(`${API_BASE}/auth/login`, request)
    return response.data
  } catch (error: any) {
    return {
      success: false,
      errorMessage: error.response?.data?.message || error.message || '登录失败'
    }
  }
}

/**
 * 刷新Token
 */
export async function refreshToken(request: RefreshTokenRequest): Promise<RefreshTokenResponse> {
  try {
    const response = await axios.post(`${API_BASE}/auth/refresh`, request)
    return response.data
  } catch (error: any) {
    return {
      success: false,
      errorMessage: error.response?.data?.message || error.message || 'Token刷新失败'
    }
  }
}

/**
 * 登出
 */
export async function logout(token: string): Promise<void> {
  try {
    await axios.post(`${API_BASE}/auth/logout`, {}, {
      headers: { 'Authorization': `Bearer ${token}` }
    })
  } catch (error) {
    console.error('Logout failed:', error)
  }
}
