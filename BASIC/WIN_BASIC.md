## 1. 常见宏

## 2. 常见宏函数

### IID_PPV_ARGS()

```c++
// 确保返回的接口指针类型的正确性
#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)
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

## 4. 常见函数

### OutputDebugStringW

```c++
void OutputDebugStringW(
    // 要显示的以null结尾的字符串
    // 要强制OutputDebugStringW正确输出 Unicode 字符串
    [in, optional] LPCWSTR lpOutputString
);
```

