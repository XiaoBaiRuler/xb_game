#include <iostream>
#include <windows.h>
#include <algorithm>
#include <utility>
#include <E:\GitStore\xb_game\DirectXMath\Inc\DirectXMath.h>
using namespace std;
using namespace DirectX;
int main(){
    if (XMVerifyCPUSupport()){
        cout << "DirectXMath" << endl;
    }
    return EXIT_SUCCESS;
}