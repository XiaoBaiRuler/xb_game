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

### D3D12_COMMAND_QUEUE_DESC

```c++
// 描述命令队列
typedef struct D3D12_COMMAND_QUEUE_DESC {
    // 指定D3D12_COMMAND_LIST_TYPE的一个成员
    D3D12_COMMAND_LIST_TYPE   Type;
    // 指定D3D12_COMMAND_QUEUE_FLAGS枚举中的任何标志
    INT                       Priority;
    D3D12_COMMAND_QUEUE_FLAGS Flags;
    // 对于单 GPU 操作，将其设置为零。
    // 如果有多个 GPU 节点，请设置一个位来标识命令队列应用到的节点（设备的物理适配器）
    // 掩码中的每一位对应一个节点
    UINT                      NodeMask;
} D3D12_COMMAND_QUEUE_DESC;
```

### D3D11_VIEWPORT

```c++
// 定义视口的尺寸
typedef struct D3D11_VIEWPORT {
    // 视口左侧的 X 位置
    FLOAT TopLeftX;
    // 视口顶部的 Y 位置
    FLOAT TopLeftY;
    // 视口的宽度
    FLOAT Width;
    // 视口的高度
    FLOAT Height;
    // 视口的最小深度。范围在 0 和 1 之间
    FLOAT MinDepth;
    // 视口的最大深度。范围在 0 和 1 之间
    FLOAT MaxDepth;
} D3D11_VIEWPORT;
```



## 2. 常见接口

### ID3D12Device

#### 检测功能支持

##### CheckFeatureSupport

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

##### GetNodeCount

```c++
// 查询系统中GPU适配节点(物理GPU)的数量
```

#### 资源驻留

##### MakeResident

```c++
// 使对象驻留在设备中
HRESULT MakeResident(
    // ppObjects数组中要为设备驻留的对象数
    UINT           NumObjects,
    // 指向包含对象的ID3D12Pageable接口指针数组的内存块的指针
    // 尽管大多数 D3D12 对象都继承自ID3D12Pageable，
    // 但仅以下对象支持驻留更改：描述符堆、堆、已提交资源和查询堆
    [in] ID3D12Pageable * const *ppObjects
);
// 返回Direct3D 12 返回代码之一
```

##### Evict

```c++
// 启用数据的分页，这会阻止 GPU 访问该数据
HRESULT Evict(
    // 要从设备中逐出的ppObjects数组中的对象数
    UINT           NumObjects,
    // 指向包含对象的ID3D12Pageable接口指针数组的内存块的指针
    // 描述符堆、堆、已提交资源和查询堆
    [in] ID3D12Pageable * const *ppObjects
);
```

#### 命令队列和命令列表

##### createCommandQueue

```c++
// 创建命令队列
HRESULT CreateCommandQueue(
    // 指定描述命令队列的D3D12_COMMAND_QUEUE_DESC 
    const D3D12_COMMAND_QUEUE_DESC *pDesc,
    // 命令队列接口的全局唯一标识符 (GUID)
    REFIID  riid,
    // 一个指向内存块的指针，它接收指向命令队列的ID3D12CommandQueue接口的指针
    void   **ppCommandQueue
);
```

##### createCommandAllocator

```c++
// 创建一个命令分配器对象
HRESULT CreateCommandAllocator(
    // 指定要创建的命令分配器的类型
    [in]  D3D12_COMMAND_LIST_TYPE type,
    // 命令分配器接口 ( ID3D12CommandAllocator )的全局唯一标识符 ( GUID )
    REFIID                  riid,
    // 一个指向内存块的指针，它接收指向命令分配器的ID3D12CommandAllocator接口的指针
    [out] void                    **ppCommandAllocator
);
```

##### createCommandList

```c++
// 创建一个命令列表
HRESULT CreateCommandList(
    // 对于单 GPU 操作，将其设置为零。
    // 如果有多个 GPU 节点，则设置一个位来标识要为其创建命令列表的节点
    [in]           UINT                    nodeMask,
    // 指定要创建的命令列表的类型
    [in]           D3D12_COMMAND_LIST_TYPE type,
    // 指向设备创建命令列表的命令分配器对象的指针
    [in]           ID3D12CommandAllocator  *pCommandAllocator,
    // 指向管道状态对象的可选指针，其中包含命令列表的初始管道状态
    // 如果是nullptr,则运行时设置一个虚拟的初始管道状态,
    // 以便驱动程序不必处理未定义的状态
    [in, optional] ID3D12PipelineState     *pInitialState,
    // 对要在ppCommandList中返回的命令列表接口的全局唯一标识符 ( GUID ) 的引用
    [in]           REFIID                  riid,
    // 一个指向内存块的指针
    // 它接收指向命令列表的ID3D12CommandList或ID3D12GraphicsCommandList接口的指针
    [out]          void                    **ppCommandList
);
// S_OK
// E_OUTOFMEMORY: 内存不足，无法创建命令列表。
```

### ID3D12CommandQueue

```c++
// 提供用于提交命令列表、同步命令列表执行、检测命令队列和更新资源切片映射的方法。
// how to use?
Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
D3D12_COMMAND_QUEUE_DESC queueDesc = {};
queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
ThrowIfFailed(
    md3dDevice -> createCommandQueue(&queueDesc,IID_PPV_ARGS(&mCommandQueue)));
```

#### ExecuteCommandLists

```c++
// 提交一组命令列表以供执行(GPU从第一个开始顺序执行)
void ExecuteCommandLists(
    // 要执行的命令列表的数量
    [in] UINT              NumCommandLists,
    // 要执行的ID3D12CommandList命令列表数组
    [in] ID3D12CommandList * const *ppCommandLists
);
```

### ID3D12GraphicsCommandList

```mathematica
// 继承于ID3D12CommandList
// 方法都是将命令加入命令列表
// 当其中的一个命令列表在记录命令时，必须关闭同一命令分配器的其他命令列表
```

#### RSSetViewports

```c++
// 将一组视口绑定到管道的光栅化阶段(设置视口)
void RSSetViewports(
    // 要绑定的视口数量
    [in]           UINT                 NumViewports,
    // 要绑定到设备的D3D11_VIEWPORT结构数组
    [in, optional] const D3D11_VIEWPORT *pViewports
);
```

#### ClearRenderTargetView

```c++
// 将渲染目标中的所有元素设置为一个值(清除渲染目标视图)
void ClearRenderTargetView(
    // 指向渲染目标的指针
    [in] ID3D11RenderTargetView *pRenderTargetView,
    // 一个 4 分量数组，表示填充渲染目标的颜色
    [in] const FLOAT [4]        ColorRGBA
);
```

#### DrawIndexedInstanced

```c++
// 绘制索引的、实例化的图元(发起绘制调用)
void DrawIndexedInstanced(
    // 从每个实例的索引缓冲区读取的索引数
    [in] UINT IndexCountPerInstance,
    // 要绘制的实例数
    [in] UINT InstanceCount,
    // GPU 从索引缓冲区读取的第一个索引的位置
    [in] UINT StartIndexLocation,
    // 在从顶点缓冲区读取顶点之前添加到每个索引的值
    [in] INT  BaseVertexLocation,
    // 在从顶点缓冲区读取每个实例数据之前添加到每个索引的值
    [in] UINT StartInstanceLocation
);
```

#### close

```c++
// 表示已完成对命令列表的记录
HRESULT Close();
// 1. S_OK
// 2. E_FAIL: 命令列表已经关闭, 命令列表记录期间调用了无效的 API
// 3. E_OUTOFMEMORY: 如果操作系统在录制期间内存不足
// 4. E_INVALIDARG: 如果在录制期间将无效参数传递给命令列表 API
```

#### Reset

```c++
// 安全地复用命令列表占用的相关底层内存来记录新的命令集
```

### ID3D12CommandAllocator

```c++
// 记录在命令列表内的命令: 存储在与之关联的命令分配器(command allocator)
// 调用ExecuteCommandLists时, 命令队列就会引用分配器里的命令
```

#### Reset

```c++
// 服用命令分配器的内存(大小清零, 容量不变)
// 在没有确定GPU执行完命令分配器中的所有命令之前，不要重置命令分配器
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

### D3D12_COMMAND_LIST_TYPE

```c++
// 指定命令列表的类型
typedef enum D3D12_COMMAND_LIST_TYPE {
    // 指定 GPU 可以执行的命令缓冲区。直接命令列表不继承任何 GPU 状态
    D3D12_COMMAND_LIST_TYPE_DIRECT = 0,
    // 指定只能通过直接命令列表直接执行的命令缓冲区。捆绑命令列表继承所有 GPU 状态
    // 将命令列表打包(bundle): 产生一定CPU开销
    D3D12_COMMAND_LIST_TYPE_BUNDLE = 1,
    // 指定用于计算的命令缓冲区
    D3D12_COMMAND_LIST_TYPE_COMPUTE = 2,
    // 指定用于复制的命令缓冲区
    D3D12_COMMAND_LIST_TYPE_COPY = 3,
    // 指定用于视频解码的命令缓冲区
    D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE = 4,
    // 指定用于视频处理的命令缓冲区
    D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS = 5,
    D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE
} ;
```

### D3D12_COMMAND_QUEUE_FLAGS

```c++
// 指定创建命令队列时要使用的标志
typedef enum D3D12_COMMAND_QUEUE_FLAGS {
    // 表示默认命令队列
    D3D12_COMMAND_QUEUE_FLAG_NONE = 0,
    // 表示应该为此命令队列禁用 GPU 超时
    D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT = 0x1
} ;
```

