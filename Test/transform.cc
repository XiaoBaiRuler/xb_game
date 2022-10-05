#include "common.h"
int main(){
    // 构建一个缩放矩阵
    XMMATRIX enlarge1 = XMMatrixScaling(10.0f, 10.0f, 10.0f);
    XMMATRIX shrink1 = XMMatrixScaling(0.1f, 0.1f, 0.1f);

    XMFLOAT3 fel(10.0f, 10.0f, 10.0f);
    XMFLOAT3 fsh(0.1f, 0.1f, 0.1f);
    XMVECTOR vel = XMLoadFloat3(&fel);
    XMVECTOR vsh = XMLoadFloat3(&fsh);
    // 1. 用一个3D向量中的分量来构建缩放矩阵
    XMMATRIX enlarge2 = XMMatrixScalingFromVector(vel);
    XMMATRIX shrink2 = XMMatrixScalingFromVector(vsh);

    // 2. 构建一个绕x,y,z轴顺时针方向(当沿旋转轴向原点的方向观察, 左手坐标系)旋转90度的矩阵
    XMMATRIX rotationx = XMMatrixRotationX(90.0f);
    XMMATRIX rotationy = XMMatrixRotationY(90.0f);
    XMMATRIX rotationz = XMMatrixRotationZ(90.0f);

    XMFLOAT4 ft(1.0f, 1.0f, 1.0f, 1.0f);    
    XMVECTOR vt = XMLoadFloat4(&ft);
    // 3. 构建一个绕任意轴(该轴正方向看)旋转90度的矩阵(1点0向量)
    XMMATRIX rotationa = XMMatrixRotationAxis(vt, 90);
    cout << rotationa << endl;

    // 4. 构建一个平移矩阵
    XMMATRIX translation1 = XMMatrixTranslation(1.0f, 2.0f, 3.0f);
    
    XMFLOAT4 ft1(1.0f, 2.0f, 3.0f, 1.0f);    
    XMVECTOR vt1 = XMLoadFloat4(&ft1);
    // 5. 用一个3D向量中的分量来构建平移矩阵
    XMMATRIX translation2 = XMMatrixTranslationFromVector(vt1);

    XMFLOAT4 ft2(1.0f, 2.0f, 3.0f, 0.0f);
    XMVECTOR vt2 = XMLoadFloat4(&ft2);

    // 6. 向量与矩阵的乘积vM: 针对点(1) 函数默认为1
    XMVECTOR mulResDot = XMVector3TransformCoord(vt1, translation2);
    cout << mulResDot << endl;
    
    XMMATRIX normal = XMMatrixIdentity();
    // 7. 向量与矩阵的乘积vM: 针对向量(0) 函数默认为0
    XMVECTOR mulResVector = XMVector3TransformNormal(vt2, normal);
    cout << mulResVector << endl;


    return EXIT_SUCCESS;
}