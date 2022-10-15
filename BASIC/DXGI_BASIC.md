<mark style="color:red;">DirectX Graphics Infrastructure</mark>

## 常见概念

### 显示适配器(display adapter)

> - 是一种硬件设备(独立显卡)
> - 系统用软件显示适配器来模拟硬件的图形处理功能

## 1. 常见结构体

###  DXGI_SAMPLE_DESC

```c++
typedef struct DXGI_SAMPLE_DESC {
    // 每个像素的多重采样数
    UINT Count;
    // 图像质量水平: 质量越高,性能越低
    UINT Quality;
} DXGI_SAMPLE_DESC;
```

## 2. 常见枚举

### DXGI_FORMAT

#### DXGI_FORMAT_R8G8B8A8_UNORM

```c++
// 一种四分量、32 位无符号归一化整数格式，每个通道支持 8 位，包括 alpha
DXGI_FORMAT_R8G8B8A8_UNORM = 28;    
```

## 3. 常见接口

### IDXGIFactory

### IDXGISwapChain

### IDXGIAdapter

### IDXGIOutput

