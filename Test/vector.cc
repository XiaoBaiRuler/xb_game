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
    return EXIT_SUCCESS;
}