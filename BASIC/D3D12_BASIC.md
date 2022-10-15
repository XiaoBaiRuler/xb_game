## 1. 常见结构体

### D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS

```c++
// 描述给定格式和样本计数的多重采样图像质量级别
typedef struct D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS {
    // 用于返回信息的格式的DXGI_FORMAT类型值
  	DXGI_FORMAT                           Format;
    // 每个像素返回信息的多样本数
  	UINT                                  SampleCount;
    // 用于控制质量级别的标志
  	D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS Flags;
    // 质量级别的数量
  	UINT                                  NumQualityLevels;
} D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS;
```

### D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS

```c++
// 指定判断品质等级的选项
typedef enum D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS {
    // 不支持任何选项
    D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE = 0,
    // 可以确定平铺资源的质量级别数
    D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_TILED_RESOURCE = 0x1
};
```

## 2. 常见函数

### ID3D12Device

#### CheckFeatureSupport

```c++
// 获取有关当前图形驱动程序支持的功能的信息
HRESULT CheckFeatureSupport(
    // D3D12_FEATURE枚举中的一个常量，描述您要查询支持的功能
    D3D12_FEATURE Feature,
    // 指向与Feature参数的值对应的数据结构的指针
    [in, out] void *pFeatureSupportData,
    // pFeatureSupportData参数指向的结构的大小
    UINT  FeatureSupportDataSize
);
```

## 3. 常见枚举

### D3D_FEATURE_LEVEL

```c++
// 描述 Direct3D 设备所针对的一组功能
typedef enum D3D_FEATURE_LEVEL {
    D3D_FEATURE_LEVEL_1_0_CORE,
    D3D_FEATURE_LEVEL_9_1,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_11_1,
    // 针对 Direct3D 12.0 支持的功能，包括着色器模型 5
    D3D_FEATURE_LEVEL_12_0,
    D3D_FEATURE_LEVEL_12_1,
    D3D_FEATURE_LEVEL_12_2
};
```



