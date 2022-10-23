#include "d3dUtil.h"
#include <windowsx.h>

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

LRESULT CALLBACK;
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    // Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
    // 转发hwnd: 获得信息
    // 在CreateWindow返回之前: 在mhMainWnd有效之前
    return D3DApp:GetApp() -> MsgProc(hwnd, msg, wParam, lParam);
}

D3DApp* D3DApp::mApp = nullptr;

/* 类基本功能 */ 
D3DApp* D3DApp::GetApp(){
    return mApp;
}
D3DApp::D3DApp(HINSTANCE hInstance) : mhAppInst(hInstance){
    // 只创建一个D3DApp
    assert(mApp == nullptr);
    mApp = this;
}
D3DApp::~D3DApp(){
    if (md3dDevice != nullptr){
        FlushCommandQueue();
    }
}

/* 窗口应用进本功能 */
HINSTANCE D3DApp::AppInst() const{
    return mhAppInst;
}
HWND D3DApp::MainWnd() const{
    return mhMainWnd;
}
float D3DApp::AspectRatio() const{
    return static<float>(mClientWidth) / mClientHeight;
}

/* Direct3D功能 */
bool D3DApp::InitDirect3D(){
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));
	// Try to create hardware device.
    // 尝试创建硬件设备
    HRESULT hardwareResult = D3D12CreateDevice(
        pWarpAdapter.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&md3dDevice)
    );
    // Fallback to WARP device.
    // 回退WARP设备
    if (FAILED(hardwardResult)){
        ComPtr<IDXGIAdapter> pWarpAdapter;
        ThrowIfFailed(mdxgiFactory -> EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

        ThrowIfFailed(D3D12CreateDevice(
            pWarpAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPVARGS(&md3dDevice)));
    }

    ThrowIfFailed(md3dDevice -> CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));

    mRtvDescriptorSize = md3dDevice -> GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    mDsvDescriptorSize = md3dDevice -> GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    mCbvSrvUavDescriptorSize = md3dDevice -> GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Check 4X MSAA quality support for our back buffer format.
    // 检查我们的后台缓冲区格式的 4X MSAA 质量支持。
    // All Direct3D 11 capable devices support 4X MSAA for all render 
    // target formats, so we only need to check quality support.
    // 所有支持 Direct3D 11 的设备都支持所有渲染目标格式的 4X MSAA，因此我们只需要检查质量支持。
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
    msQualityLevels.Format = mBackBufferFormat;
    msQualityLevels.SampleCount = 4;
    msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    msQualityLevels.NumQualityLevels = 0;
    ThrowIfFailed(md3dDevice -> CheckFeatureSupport(
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &msQualityLevels,
        sizeof(msQualityLevels)
    ));

    m4xMsaaQuality = msQualityLevels.NumQualityLevels;
    assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

    CreateCommandObjects();
    CreateSwapChain();
    CreateRtvAndDsvDescriptorHeaps();
    return true;
}

/**
 * @brief 使用fence刷新命令队列
 * 
 */
UINT64 mCurrentFence = 0;
void D3DApp::FlushCommandQueue(){
    // 增加围栏值，接下来将命令标记到此围栏点
    mCurrentFence ++;

    // 向命令队列中添加一条用来设置新围栏点的命令
    // 由于这条命令要交由GPU处理，所以GPU处理完命令队列此Singnal()的所有命令之前，它并不会设置新的围栏点
    ThrowIfFailed(mCommandQueue -> Signal(mFence.Get(), mCurrentFence));

    // 在CPU端等待GPU, 直到后者执行完这个围栏点之前的所有命令
    if (mFence -> GetCompletedValue() < mCurrentFence){

        HANDLE eventHandle = createEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
        // 若GPU命中当前的围栏，则激发预定事件
        ThrowIfFailed(mFence -> SetEventOnCompletion(mCurrentFence, eventHandle));
        // 等待GPU命中围栏，激发事件
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}


/**
 * @brief 枚举一个系统中的所有适配器
 * 
 */
void D3DApp::LogAdapter(){
    UINT i = 0;
    IDXGIAdapter* adapter = nullptr;
    std::vector<IDXGIAdapter*> adapterList;
    while(mdxgiFactory -> EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND){
        DXGI_ADAPTER_DESC desc;
        adapter -> GetDesc(&desc);
        
        std::wstring text = L"***Adapter:";
        text += desc.Description;
        text += L"\n";

        OutputDebugString(text.c_str());
        adapterList.push_back(adapter);
        ++ i;
    }
    for (size_t i = 0; i < adapterList.size(); ++ i){
        LogAdapterOutputs(adapterList[i]);
        ReleaseCom(adapterList[i]);
    }
}
/**
 * @brief 枚举适配器（视频卡）输出
 * 
 * @param adapter 
 */
void D3DApp::LogAdapterOutputs(IDXGIAdapter* adapter){
    UINT i = 0;
    IDXGIAdapter* output = nullptr;
    while (adapter -> EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND){
        DXGI_OUTPUT_DESC desc;
        output -> GetDesc(&desc);

        std::wstring text = L"***Output: ";
        text += desc.DeviceName;
        text += L"\n";
        OutputDebugString(text.c_str());

        LogOutputDisplayModes(output, mBackBufferFormat);

        ReleaseCom(output);
        ++ i;
    }
}
/**
 * @brief 获取与请求的格式和其他输入选项匹配的显示模式
 * 
 * @param output 
 * @param format 
 */
void D3DApp::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format){

    UINT count = 0;
    UINT flags = 0;

    // Call with nullptr to get list count
    // 使用nullptr调用以获取列表计数
    output -> GetDisplayModeList(format, flags, &count, nullptr);

    // 获取具体列表
    std::vector<DXGI_MODE_DESC> modeList(count);
    output -> GetDisplayModeList(format, flags, &count, &modeList[0]);
    
    for(auto& x : modeList)
    {
        UINT n = x.RefreshRate.Numerator;
        UINT d = x.RefreshRate.Denominator;
        std::wstring text =
            L"Width = " + std::to_wstring(x.Width) + L" " +
            L"Height = " + std::to_wstring(x.Height) + L" " +
            L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
            L"\n";

        ::OutputDebugString(text.c_str());
    }
}