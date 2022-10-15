## 1. 基本概念

- **组件对象模型(Component Object Model):  视为一种接口。**

  ```c++
  #include <wrl.h>
  #include <d3d12.h>
  
  // 通过特定函数或另一COM接口的方法去创建一个COM接口(会统计引用次数)
  ComPtr<ID3D12RootSignature> mRootSignature;
  // 返回指向此底层COM接口的指针
  mRootSignature.Get();
  // 返回指向此底层COM接口指针的地址
  mRootSignature.GetAddressOf();
  // 设置为null并释放与之相关的所有引用(减少底层COM接口的引用计数)
  mRootSignature.Reset();
  
  // 命令列表COM接口
  ComPtr<ID3D12GraphicsCommandList> mRootCommandList;
  // 使用完之后应调用它的Release方法来释放
  ```
  
- <mark style="color:red;">纹理格式</mark>

  ```mathematica
  1. 2D纹理(2D texture): 数据元素构成的矩阵: 存储2D图像数据(每个元素存储一个像素颜色)
  2. 法线贴图(normal mapping): 纹理内的每个元素存储一个3D向量(非颜色信息)
  3. DXGI_FORMAT枚举类型:(R(RED), G(GREEN), B(BLUE), A(ALPHA))[不一定存储颜色信息]
  补充: ALPHA通道: 用于控制透明度
  	- DXGI_FORMAT_R32G32B32_FLOAT: 每个元素由3个32位浮点数分量构成
  	- DXGI_FORMAT_R16G16B16A16_UNORM: 每个元素由4个16位分量构成，每个分量都被映射到[0,1]区间
  	- DXGI_FORMAT_R32G32_UINT: 每个元素由2个32位无符号整数分量构成
  	- DXGI_FORMAT_R8G8B8A8_UNORM: 每个元素由4个8位无符号分量构成，每个分量都被映射到[0,1]区间
  	- DXGI_FORMAT_R8G8B8A8_SNORM: 每个元素由4个8位有符号分量构成，每个分量都被映射到[-1,1]区间
  	- DXGI_FORMAT_R8G8B8A8_SINT: 每个元素由4个8位有符号整数分量构成，每个分量都被映射到[-128, 127]区间
  	- DXGI_FORMAT_R8G8B8A8_UINT: 每个元素由4个8位无符号整数分量构成，每个分量都被映射到[0, 255]区间
  	- DXGI_FORMAT_R16G16B16A16_TYPELESS 每个元素由4个16位分量构成的元素:无类型格式
  ```

- <mark style="color:blue">交换链和页面翻转</mark>

  ```mathematica
  1. 前台缓冲区(front buffer): 当前显示在屏幕上的图像数据.
  2. 后台缓冲区(back buffer): 动画的下一帧则被绘制在后台缓冲区里.
  3. 呈现(presenting): 前后台缓冲区角色互换的操作.
  4. 交换链(swap chain)由前台缓冲区和后台缓冲区构成
  	- Direct3D: IDXGISwapChain: 存储前台和后台缓冲区
  	- IDXGISwapChain::ResizeBuffers: 修改缓冲区大小
  	- IDXGISwapChain::Present: 呈现缓冲区内容
  ```

- <mark style="color:green;">深度缓冲区</mark>

  ```mathematica
  1. 深度缓冲区(depth buffer): 存储特定像素的深度信息,而非图像数据: z缓冲(z-buffering): 使得物体的绘制顺序无关紧要.
  2. 深度值范围0.0 - 1.0
  	- 0.0: 观察者在视锥体(view frustum)能看到离自己最近的物体.
  	- 1.0: 观察者在视锥体中能看到离自己最远的物体.
  3. 深度缓冲区的纹理格式:
  	- DXGI_FORMAT_D32_FLOAT_S8X24_UINT: 64位: 32位指定一个浮点型深度缓冲区;8位无符号整数分配给模板缓冲区并将该元素映射到[0, 255]; 24位仅用于填充对齐;
  	- DXGI_FORMAT_D32_FLOAT: 32位浮点型深度缓冲区
  	- DXGI_FORMAT_D24_UNORM_S8_UINT: 无符号24位深度缓冲区并将该元素映射到[0, 1]区间;8位无符号整数分配给模板缓冲区并将该元素映射到[0, 255];
  	- DXGI_FORMAT_D16_UNORM: 无符号16位深度缓冲区并将该元素映射到[0, 1]区间
  ```

  | 操作(远 -> 近 -> 中) | P    | d    | 步骤叙述: 最有找到具有最小深度值的像素,<br>才会对观察窗口内的像素及其位于深度缓冲区中的对象深度值更新 |
  | -------------------- | ---- | ---- | ------------------------------------------------------------ |
  | 清除缓冲区操作       | 黑色 | 1.0  | 对像素及其对应的深度元素进行初始化                           |
  | 绘制最远的物体       | Py   | d3   | d3<=1.0, 深度测试通过，更新缓冲区，使P=Py， d = d3           |
  | 绘制最近的物体       | Pj   | d1   | d1<=d3, 深度测试通过，更新缓冲区，使P=Pj, d = d1             |
  | 绘制中间的物体       | Pz   | d1   | d2 > d1, 深度测试失败，不更新缓冲区                          |

  三重缓冲? 垂直同步?

- <mark style="color:purple;">资源与描述符</mark>

  ```mathematica
  1. 在发出绘制命令之前，需要将本次绘制调用(draw call)相关的资源绑定(bind)\链接(link)到渲染流水线.
  2. 描述符(descriptor)\视图(view): 对送往CPU的资源进行描述的轻量级结构(中间层): 解释资源: 告知Direct3D某个资源将如何使用，指定欲绑定资源中的局部数据.
  	- CBV/SRV/UAV: 常量缓冲区视图(constant buffer view)/着色器资源视图(shader resource view)/无序访问视图(unordered access view)
  	- 采样器(sampler): 采样器资源(纹理贴图)
  	- RTV: 渲染目标视图资源(render target view)
  	- DSV: 深度/模板视图资源(depth/stencil view)
  3. 描述符堆: 存放用户程序中某种特定类型描述符的一块内存(每种类型的描述符都创建出单独的描述符堆(多个描述符堆))[创建最佳时间为初始化期间: 需要执行一些类型的检测和验证工作]
  
  ```

- <mark stype-="color:red;">多重采样技术的原理</mark>

  ```mathematica
  1. 锯齿状走样: 对于倾斜的直线，以像素矩阵逼近直线的方法所产生的"阶梯"效果.
  	1. 提高显示器分辨率能够改善这种情况
  	2. 反走样(antialiasing): 抗锯齿、反锯齿、反失真
  	3. 超级采样(supersampling)[SSAA]: 使用四倍于屏幕分辨率大小的后台缓冲区和深度缓冲区; 在显示时, 会将后台缓冲区按4个像素一组进行解析(resolve)\降采样(downsample): 把放大的采样点数降低回原采样点数每组用求平均值的方法得到一种相对平滑的像素颜色.
  		- 内存资源，处理资源都增加到之前的4倍
  	4. 多重采样(multisampling)[MSAA]: 4X多重采样(每个像素都有4个子像素): 使用4被于屏幕分辨率的后台缓冲区和深度缓冲区: 仅计算一次像素中心处的颜色，再基于可视性(每个子像素经深度/模板测试的结果)和覆盖性(子像素的中心再多边形的里面还是外面) -> 得到颜色信息分享给其子像素。   
  ```

  ```c++
  // 采样次数越多或质量级别越高, 渲染的代价越高
  typedef struct DXGI_SAMPLE_DESC{
      // 每个像素的采样次数
      UINT Count;
      // 指示用户期望的图像质量级别
      UINT Quality;
  } DXGI_SAMPLE_DESC;
  ```

  ```mathematica
  1. HINSTANCE: Windows里的一种数据类型: 32位无符号的长整型: 
  	- 用于表示一个程序的实例.
  2. HWND: 类似long的基本类型: 
  	- 窗口创建系统分配的一个句柄
  	- H: 类型,句柄(handle); WND: 变量对象描述,窗口; 窗口句柄
  3. LRESULT, HRESULT: 
  	- LRESULT: Windows进程或者回调函数的返回值(Win32: 32无符号长整型; Win64: 64无符号长整型)
  	- HRESULT: 存储返回信息(LONG 4字节, 32位): 正数为成功, 负数为失败
          - S_OK 操作成功
          - S_FALSE 操作成功, 但是有问题
          - E_ABORT 操作中止
          - E_ACCESSDENIED 拒绝访问
          - E_FAIL 未知错误
  4. UINT: 无符号整型
  5. WPARAM: 一个与消息有关的常量值，也可能时窗口或控件的句柄.
  6. LPARAM: 一个指向内存中数据的指针，必要时可以强制转换.
  7. LARGE_INTEGER: union, 表示64位有符号整数值
  	- 支持64位: __int64, QuandPart使用
  	- 支持32位: 高低位使用, LowPart(32),HighPart(32)[最高位为符号位]
  	
  ```
  
  ```c++
  // QueryPerformanceFrequency(): 操作系统的性能统计分辨率: 每秒钟统计多少次
  // QueryPerformanceCounter(): 系统性能统计计数器: 表示统计了多少次.
  
  ```
  
  