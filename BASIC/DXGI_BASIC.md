<mark style="color:red;">DirectX Graphics Infrastructure</mark>

## 常见概念

### 显示适配器(display adapter)

> - 是一种硬件设备(独立显卡)
> - 系统用软件显示适配器来模拟硬件的图形处理功能

## 1. 常见结构体

###  DXGI_SAMPLE_DESC

```c++
// 采样次数越多或质量级别越高, 渲染的代价越高
typedef struct DXGI_SAMPLE_DESC {
    // 每个像素的多重采样数
    UINT Count;
    // 图像质量水平: 质量越高,性能越低
    UINT Quality;
} DXGI_SAMPLE_DESC;
```

### DXGI_MODE_DESC

```c++
typedef struct DXGI_MODE_DESC {
    // 分辨率宽度
    UINT Width;
    // 分辨率高度
    UINT Height;
    // 刷新率(Hz)
    DXGI_RATIONAL RefreshRate;
    // 显示格式
    DXGI_FORMAT Format;
    // 逐行扫描vs, 隔行扫描
    DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
    // 图像如何相对于屏幕进行拉伸
    DXGI_MODE_SCALING Scaling;
} DXGI_MODE_DESC;
```

### DXGI_RATIONAL

```c++
// 代表一个有理数
typedef struct DXGI_RATIONAL {
    // 有理数分子
    UINT Numerator;
    // 有理数分母
    UINT Denominator;
} DXGI_RATIONAL;
// 0/0 是合法的，将被解释为 0/1
// 如果你代表一个整数，分母应该是 1
```



## 2. 常见枚举

### DXGI_FORMAT

```c++
// 一种四分量、32 位无符号归一化整数格式，每个通道支持 8 位，包括 alpha
DXGI_FORMAT_R8G8B8A8_UNORM = 28;    
```

### DXGI_MODE_SCANLINE_ORDER

```c++
// 指示光栅用于在表面上创建图像的方法的标志
typedef enum DXGI_MODE_SCANLINE_ORDER { 
    // 扫描线顺序未指定
    DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED        = 0,
    // 从第一个扫描线到最后一个扫描线创建图像，不跳过任何扫描线
    DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE        = 1,
    // 图像从上部字段开始创建
    DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST  = 2,
    // 图像从下部字段开始创建
    DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST  = 3
} DXGI_MODE_SCANLINE_ORDER;
```

### DXGI_MODE_SCALING

```c++
// 指示如何拉伸图像以适应给定显示器分辨率的标志
typedef enum DXGI_MODE_SCALING { 
    // 未指定的缩放
    DXGI_MODE_SCALING_UNSPECIFIED  = 0,
    // 指定不缩放。图像位于显示屏的中心
    DXGI_MODE_SCALING_CENTERED     = 1,
    // 根据屏幕的分辨率对图像进行拉伸缩放
    DXGI_MODE_SCALING_STRETCHED    = 2
} DXGI_MODE_SCALING;
```

## 3. 常见接口

### IDXGIFactory

#### EnumAdapters

```c++
// 枚举适配器（视频卡）
HRESULT EnumAdapters(
    // 要枚举的适配器的索引
    UINT         Adapter,
    // 指向由Adapter参数指定的位置处的IDXGIAdapter接口的指针的地址
    [out] IDXGIAdapter **ppAdapter
);
```

### IDXGISwapChain

### IDXGIAdapter

#### EnumOutputs

```c++
// 枚举适配器（视频卡）输出
HRESULT EnumOutputs(
    // 输出的索引
    UINT        Output,
    // 指向由Output参数指定的位置处的IDXGIOutput接口的指针的地址
    [out] IDXGIOutput **ppOutput
);
```

### IDXGIOutput

#### GetDisplayModeList

```c++
// D3D11.1开始建议  IDXGIOutput1::GetDisplayModeList1
HRESULT GetDisplayModeList(
    // 颜色格式
    DXGI_FORMAT    EnumFormat,
    // 需要指定 DXGI_ENUM_MODES_SCALING 以公开需要缩放的显示模式
    UINT           Flags,
    // 将pDesc设置为NULL，以便pNumModes返回与格式和选项匹配的显示模式的数量
    // 否则，pNumModes返回 pDesc 中返回的显示模式数
    [in, out]       UINT           *pNumModes,
    // 指向显示模式列表的指针(参见DXGI_MODE_DESC), 设置为NULL以获取显示模式的数量
    [out, optional] DXGI_MODE_DESC *pDesc
);
```

