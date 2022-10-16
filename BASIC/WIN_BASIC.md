## 1. 常见宏

## 2. 常见宏函数

### IID_PPV_ARGS()

```c++
// 确保返回的接口指针类型的正确性 -> 强制转void**
#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)
// __uuidof(**(ppType)): 获取(**(ppType))的COM接口ID
// (全局唯一标识符GUID[globally unique identifier])
```

## 3. 常见类型

### LRESULT\LRESULT

```mathematica
1. LRESULT: Windows进程或者回调函数的返回值(Win32: 32无符号长整型; Win64: 64无符号长整型)
2. HRESULT: 存储返回信息(LONG 4字节, 32位): 正数为成功, 负数为失败
    - S_OK 操作成功
    - S_FALSE 操作成功, 但是有问题
    - E_ABORT 操作中止
    - E_ACCESSDENIED 拒绝访问
    - E_FAIL 未知错误
```

###  LARGE_INTEGER
```mathematica
1. union, 表示64位有符号整数值
    - 支持64位: __int64, QuandPart使用
    - 支持32位: 高低位使用, LowPart(32),HighPart(32)[最高位为符号位]
```

### UINT

```mathematica
1. 无符号整型
```

### HINSTANCE 
```mathematica
1. Windows里的一种数据类型: 32位无符号的长整型
	- 用于表示一个程序的实例.
```

### HWND 
```mathematica
1. 类似long的基本类型
	- 窗口创建系统分配的一个句柄
	- H: 类型,句柄(handle); WND: 变量对象描述,窗口; 窗口句柄
```

### WPARAM\LPARAM

```mathematica
1. WPARAM: 一个与消息有关的常量值，也可能时窗口或控件的句柄.
2. LPARAM: 一个指向内存中数据的指针，必要时可以强制转换.
```

## 4. 常见函数

### OutputDebugStringW

```c++
void OutputDebugStringW(
    // 要显示的以null结尾的字符串
    // 要强制OutputDebugStringW正确输出 Unicode 字符串
    [in, optional] LPCWSTR lpOutputString
);
```

### QueryPerformanceFrequency

```c++
// 操作系统的性能统计分辨率: 每秒钟统计多少次
```

### QueryPerformanceCounter

```c++
// 系统性能统计计数器: 表示统计了多少次
```

## 5. 常见接口



