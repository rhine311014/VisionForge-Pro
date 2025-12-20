/**
 * WebSocket客户端 - 连接到VisionForge远程诊断服务器
 */

export interface PerformanceMetrics {
  timestamp: string
  cpuUsage: number
  memoryUsedMB: number
  fps: number
  avgLatencyMs: number
  errorCount: number
}

export interface LogEntry {
  timestamp: string
  level: string
  category: string
  message: string
  file?: string
  line?: number
}

export interface SystemStatus {
  timestamp: string
  isRunning: boolean
  uptime: number
  version: string
}

type MessageHandler = (data: any) => void

export class WebSocketClient {
  private ws: WebSocket | null = null
  private url: string
  private token: string
  private reconnectAttempts = 0
  private maxReconnectAttempts = 5
  private reconnectDelay = 3000
  private heartbeatInterval: number | null = null
  private messageHandlers = new Map<string, MessageHandler[]>()

  constructor(url: string, token: string) {
    this.url = url
    this.token = token
  }

  /**
   * 连接到WebSocket服务器
   */
  connect(): Promise<void> {
    return new Promise((resolve, reject) => {
      try {
        this.ws = new WebSocket(this.url)

        this.ws.onopen = () => {
          console.log('WebSocket connected')
          this.reconnectAttempts = 0

          // 发送认证消息
          this.send({
            type: 'client_hello',
            data: {
              client_id: 'web-monitor',
              version: '1.0.0',
              token: this.token
            }
          })

          // 启动心跳
          this.startHeartbeat()
          resolve()
        }

        this.ws.onmessage = (event) => {
          try {
            const message = JSON.parse(event.data)
            this.handleMessage(message)
          } catch (error) {
            console.error('Failed to parse message:', error)
          }
        }

        this.ws.onerror = (error) => {
          console.error('WebSocket error:', error)
          reject(error)
        }

        this.ws.onclose = () => {
          console.log('WebSocket disconnected')
          this.stopHeartbeat()
          this.attemptReconnect()
        }
      } catch (error) {
        reject(error)
      }
    })
  }

  /**
   * 订阅数据类型
   */
  subscribe(types: string[]): void {
    this.send({
      type: 'subscribe',
      data: { types }
    })
  }

  /**
   * 取消订阅
   */
  unsubscribe(types: string[]): void {
    this.send({
      type: 'unsubscribe',
      data: { types }
    })
  }

  /**
   * 发送消息
   */
  send(message: any): void {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      this.ws.send(JSON.stringify(message))
    } else {
      console.warn('WebSocket not connected')
    }
  }

  /**
   * 注册消息处理器
   */
  on(messageType: string, handler: MessageHandler): void {
    if (!this.messageHandlers.has(messageType)) {
      this.messageHandlers.set(messageType, [])
    }
    this.messageHandlers.get(messageType)!.push(handler)
  }

  /**
   * 移除消息处理器
   */
  off(messageType: string, handler: MessageHandler): void {
    const handlers = this.messageHandlers.get(messageType)
    if (handlers) {
      const index = handlers.indexOf(handler)
      if (index > -1) {
        handlers.splice(index, 1)
      }
    }
  }

  /**
   * 断开连接
   */
  disconnect(): void {
    this.stopHeartbeat()
    if (this.ws) {
      this.ws.close()
      this.ws = null
    }
  }

  /**
   * 处理接收到的消息
   */
  private handleMessage(message: any): void {
    const { type, data } = message

    // 调用注册的处理器
    const handlers = this.messageHandlers.get(type)
    if (handlers) {
      handlers.forEach(handler => {
        try {
          handler(data)
        } catch (error) {
          console.error(`Error in message handler for ${type}:`, error)
        }
      })
    }

    // 处理特殊消息
    switch (type) {
      case 'server_hello':
        console.log('Server handshake complete:', data)
        break
      case 'pong':
        // 心跳响应
        break
      case 'error':
        console.error('Server error:', data)
        break
    }
  }

  /**
   * 启动心跳
   */
  private startHeartbeat(): void {
    this.heartbeatInterval = window.setInterval(() => {
      this.send({ type: 'ping' })
    }, 10000) // 每10秒发送一次心跳
  }

  /**
   * 停止心跳
   */
  private stopHeartbeat(): void {
    if (this.heartbeatInterval) {
      clearInterval(this.heartbeatInterval)
      this.heartbeatInterval = null
    }
  }

  /**
   * 尝试重连
   */
  private attemptReconnect(): void {
    if (this.reconnectAttempts < this.maxReconnectAttempts) {
      this.reconnectAttempts++
      console.log(`Attempting to reconnect... (${this.reconnectAttempts}/${this.maxReconnectAttempts})`)

      setTimeout(() => {
        this.connect().catch(error => {
          console.error('Reconnect failed:', error)
        })
      }, this.reconnectDelay * this.reconnectAttempts)
    } else {
      console.error('Max reconnect attempts reached')
      // 触发重连失败事件
      const handlers = this.messageHandlers.get('connection_failed')
      if (handlers) {
        handlers.forEach(handler => handler({ reason: 'max_attempts_reached' }))
      }
    }
  }
}
