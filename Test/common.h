#include <iostream>
#include <windows.h>
#include <algorithm>
#include <utility>
#include <climits>
#include "../DirectXMath/Inc/DirectXMath.h"
#include "../DirectXMath/Inc/DirectXPackedVector.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;
void printXMVECTOR(FXMVECTOR a);

void printXMVECTOR(FXMVECTOR a){
    cout << XMVectorGetX(a) << " " << XMVectorGetY(a) << " " << XMVectorGetZ(a) << " " << XMVectorGetW(a) << endl;
}

ostream& XM_CALLCONV operator<<(ostream& os, FXMVECTOR v){
    XMFLOAT4 dest;
    XMStoreFloat4(&dest, v);
    cout << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
    return os; 
}
