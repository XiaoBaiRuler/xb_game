#include "common.h"
int main(){
    // 4个XMVECTOR组成的XMMATRIX
    XMMATRIX vx4(1, 1, 1, 1,
                 1, 1, 1, 1, 
                 1, 1, 1, 1, 
                 1, 1, 1, 1);
    // 加载到XMMATRIX
    XMFLOAT4X4 fx4 = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    vx4 = XMLoadFloat4x4(&fx4);
    cout << vx4 << endl;
    // 存储到XMFLOAT4X4
    XMStoreFloat4x4(&fx4, vx4);

    // 常用函数
    // 1. 获取单位矩阵I
    vx4 = XMMatrixIdentity();
    cout << vx4 << endl;
    // 2. 是否为单位矩阵
    cout << XMMatrixIsIdentity(vx4) << endl;
    XMMATRIX mx4 = XMLoadFloat4x4(&fx4);
    // 3. 矩阵相乘 mx4 * vx4
    XMMATRIX res = XMMatrixMultiply(mx4, vx4);
    cout << mx4 * vx4 << endl;
    cout << res << endl;
    // 4. 获取转置矩阵
    XMMATRIX tx4 = XMMatrixTranspose(res);
    cout << tx4 << endl;
    // 5. 返回矩阵的行列式
    XMVECTOR v = XMMatrixDeterminant(tx4);
    cout << v << endl;
    // 6. 返回矩阵的逆矩阵: 方阵A是可逆的，当且仅当detA != 0
    XMMATRIX tx5 = XMMatrixInverse(&v, tx4);
    cout << tx5 << endl;

    // XMMATRIX的传参数 (1个XMMATRIX相当于4个XMVECTOR 通过寄存器都按照XMVECTOR)
    // 第一个使用FXMMATRIX
    // 其他使用CXMMATRIX
    return EXIT_SUCCESS;
}