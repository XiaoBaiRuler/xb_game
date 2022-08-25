#include "common.h"
int main(){
    
    // 1. 局部 / 全局使用XMVECTOR
    // 2. 类中数据成员使用 XMFLOATX(2,3,4)
    // 3. 使用XMVECTOR来进行运算
    XMFLOAT2 a2(1.9, 2.1);
    // 加载
    XMVECTOR a = XMLoadFloat2(&a2);
    // 获取
    printXMVECTOR(a);
    // 更新
    a = XMVectorSetX(a, 1.1);
    a = XMVectorSetY(a, 2.2);
    a = XMVectorSetZ(a, 3.3);
    a = XMVectorSetW(a, 4.4);
    // 获取
    printXMVECTOR(a);
    // 存储
    XMFLOAT4 b;
    XMStoreFloat4(&b, a);

    // 传递XMVECTOR参数的规则 - 必须初始化 -- 以XMVECTOR参数个数为准
    // 1. 前面3个
    FXMVECTOR a3(a);
    // 2. 第4个
    GXMVECTOR a4(a);
    // 3. 第5、6个
    HXMVECTOR a5(a);
    // 4. 其他 
    CXMVECTOR a7(a);

    // 对于输入参数，何时使用SSE/SSE2寄存器
    // 1. __fastcall 将前3个用寄存器
    // 2. __vectorcall 将前6个用寄存器

    // 添加编译选项编译 --mfma

    // 常向量
    // 1. 浮点数 16字节对齐
    static const XMVECTORF32 gf = {0.5f, 0.5f, 0.5f, 0.5f};
    printXMVECTOR(gf);

    // 2. 整数
    static const XMVECTORU32 gu = {0, 0, 0, 0};
    printXMVECTOR(gu);

    XMVECTOR f = {1, 2, 3, 4};
    XMVECTOR l = {1, 1, 1, 1};
    XMVECTOR r = {2, 2, 2, 2};

    // XMVECTOR 

    // 重载运算符
    // 1. +  +=
    f = l + r;
    printXMVECTOR(f);
    l += r;
    printXMVECTOR(l);
    // 2. -  -=
    f = l - r;
    printXMVECTOR(f);
    l -= r;
    printXMVECTOR(l);
    // 3. *  *=
    f = l * r;
    printXMVECTOR(f);
    l *= r;
    printXMVECTOR(l);
    // 4. /  /=
    f = l / r;
    printXMVECTOR(f);
    l = l / r;
    printXMVECTOR(l);

    // 5. 标量乘法
    // 5.1 * 左右乘都可以
    f = l * 3;
    printXMVECTOR(f);
    f = 4 * l;
    printXMVECTOR(f);
    // 5.2 *=
    l *= 5;
    printXMVECTOR(l);

    // 6. 标量除法
    // 6.1 只能除于标量
    f = l / 5;
    printXMVECTOR(f);
    // 6.2 /=
    l /= 5;
    printXMVECTOR(l);
    return EXIT_SUCCESS;
}