import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { resolve } from 'path'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],

  resolve: {
    alias: {
      '@': resolve(__dirname, 'src')
    }
  },

  server: {
    port: 3000,
    host: true, // 监听所有地址
    proxy: {
      '/api': {
        target: 'http://localhost:8080',
        changeOrigin: true
      },
      '/ws': {
        target: 'ws://localhost:8765',
        ws: true
      }
    }
  },

  build: {
    outDir: 'dist',
    sourcemap: false,

    // 构建优化
    target: 'es2015',
    minify: 'terser',
    terserOptions: {
      compress: {
        drop_console: true, // 生产环境移除console
        drop_debugger: true,
        pure_funcs: ['console.log']
      }
    },

    // 分块策略
    chunkSizeWarningLimit: 1000, // 提高警告阈值到1MB

    rollupOptions: {
      output: {
        // 更细粒度的代码分割
        manualChunks: (id) => {
          // node_modules分离
          if (id.includes('node_modules')) {
            // Vue核心
            if (id.includes('vue') || id.includes('pinia') || id.includes('vue-router')) {
              return 'vue-vendor'
            }

            // Element Plus UI
            if (id.includes('element-plus')) {
              return 'ui-vendor'
            }

            // ECharts图表
            if (id.includes('echarts')) {
              return 'chart-vendor'
            }

            // 其他第三方库
            return 'vendor'
          }
        },

        // 文件命名优化
        chunkFileNames: 'assets/js/[name]-[hash].js',
        entryFileNames: 'assets/js/[name]-[hash].js',
        assetFileNames: (assetInfo) => {
          // 根据文件类型分类
          const info = assetInfo.name.split('.')
          const ext = info[info.length - 1]

          if (/\.(png|jpe?g|gif|svg|webp|ico)$/.test(assetInfo.name)) {
            return 'assets/images/[name]-[hash][extname]'
          } else if (/\.(woff2?|eot|ttf|otf)$/.test(assetInfo.name)) {
            return 'assets/fonts/[name]-[hash][extname]'
          } else if (/\.css$/.test(assetInfo.name)) {
            return 'assets/css/[name]-[hash][extname]'
          }

          return 'assets/[ext]/[name]-[hash][extname]'
        }
      }
    },

    // 资源内联阈值
    assetsInlineLimit: 4096, // 小于4KB的资源内联为base64

    // CSS代码分割
    cssCodeSplit: true,

    // 清空输出目录
    emptyOutDir: true
  },

  // 依赖优化
  optimizeDeps: {
    include: [
      'vue',
      'vue-router',
      'pinia',
      'element-plus',
      'echarts'
    ],
    exclude: []
  },

  // ESBuild优化
  esbuild: {
    drop: process.env.NODE_ENV === 'production' ? ['console', 'debugger'] : []
  }
})
