#include "common.h"
int main(){
    if (XMVerifyCPUSupport()){
        cout << "DirectXMath" << endl;
    }
    return EXIT_SUCCESS;
}