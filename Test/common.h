#include <iostream>
#include <windows.h>
#include <algorithm>
#include <utility>
#include "../DirectXMath/Inc/DirectXMath.h"
#include "../DirectXMath/Inc/DirectXPackedVector.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;
void printXMVECTOR(FXMVECTOR a);

void printXMVECTOR(FXMVECTOR a){
    cout << XMVectorGetX(a) << " " << XMVectorGetY(a) << " " << XMVectorGetZ(a) << " " << XMVectorGetW(a) << endl;
}