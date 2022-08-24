#include "common.h"
int main(){
    
    XMFLOAT2 a2(1.9, 2.1);
    // 加载
    XMVECTOR a = XMLoadFloat2(&a2);
    // 获取
    cout << XMVectorGetX(a) << endl;
    cout << XMVectorGetY(a) << endl;
    cout << XMVectorGetZ(a) << endl;
    cout << XMVectorGetW(a) << endl;
    // 更新
    a = XMVectorSetX(a, 1.1);
    a = XMVectorSetY(a, 2.2);
    a = XMVectorSetZ(a, 3.3);
    a = XMVectorSetW(a, 4.4);
    // 获取
    cout << XMVectorGetX(a) << endl;
    cout << XMVectorGetY(a) << endl;
    cout << XMVectorGetZ(a) << endl;
    cout << XMVectorGetW(a) << endl;
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
    return EXIT_SUCCESS;
}