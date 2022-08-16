#include <iostream>
#include <windows.h>
#include <algorithm>
#include <utility>
#include <E:\GitStore\xb_game\DirectXMath\Inc\DirectXMath.h>
// #include <G:\game\xb_game\DirectXMath\Inc\DirectXMath.h>
#include <E:\GitStore\xb_game\DirectXMath\Inc\DirectXPackedVector.h>
// #include <G:\game\xb_game\DirectXMath\Inc\DirectXPackedVector.h>
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;
int main(){
    if (XMVerifyCPUSupport()){
        cout << "DirectXMath" << endl;
    }
    return EXIT_SUCCESS;
}