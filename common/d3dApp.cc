#include "d3dUtil.h"
#include <windowsx.h>

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

LRESULT CALLBACK;
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
    // 转发hwnd: 获得信息
    // 在CreateWindow返回之前: 在mhMainWnd有效之前
    return D3DApp:GetApp() -> MsgProc(hwnd, msg, wParam, lParam);
}


/* 类基本功能 */ 
D3DApp* D3DApp::mApp = nullptr;

/**
 * @brief 获取D3DApp 实例
 * 
 * @return D3DApp* 
 */
D3DApp* D3DApp::GetApp()
{
    return mApp;
}

D3DApp::D3DApp(HINSTANCE hInstance) : mhAppInst(hInstance)
{
    // 只创建一个D3DApp
    assert(mApp == nullptr);
    mApp = this;
}

D3DApp::~D3DApp()
{
    if (md3dDevice != nullptr){
        FlushCommandQueue();
    }
}


/* 窗口应用进本功能 */
/**
 * @brief 返回应用程序实例句柄
 * 
 * @return HINSTANCE 
 */
HINSTANCE D3DApp::AppInst() const
{
    return mhAppInst;
}

/**
 * @brief 返回主窗口句柄
 * 
 * @return HWND 
 */
HWND D3DApp::MainWnd() const
{
    return mhMainWnd;
}

/**
 * @brief 纵横比: 后台缓冲区的宽度与高度之比
 * 
 * @return float 
 */
float D3DApp::AspectRatio() const
{
    return static<float>(mClientWidth) / mClientHeight;
}

/**
 * @brief 是否启用4X MSAA
 * 
 * @return true y
 * @return false n
 */
bool D3DApp::Get4XMsaaState() const
{
    return m4xMsaaState;
}

/**
 * @brief 开启或禁用4X MSAA功能
 * 
 */
void D3DApp::Set4XMsaaState() const(bool value)
{
    if (m4xMsaaState != value){
        m4xMsaaState = value;
        // Recreate the swapchain and buffers with new multisample settings.
        // 使用新的多采样设置重新创建扫描链和缓冲区
        CreateSwapChain();
        OnResize();
    }
}

/**
 * @brief 封装应用的消息循环
 * 
 * @return int 
 */
int D3DApp::Run()
{
    MSG msg = {0};
    mTimer.Reset();

    while (msg.message != WM_QUIT){
        // 如果存在Window消息，则对其进行处理。
		// If there are Window messages then process them.
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // 否则，做动画/游戏的东西
        // Otherwise, do animation/game stuff.
        else{
            mTimer.Tick();

            if (!mAppPaused){
                CalculateFrameStats();
                Update(mTimer);
                Draw(mTimer);
            }
            else{
                Sleep(100);
            }
        }
    }
}

/**
 * @brief 初始化代码: 分配资源、初始化对象和建立3D场景
 * 
 * @return true 
 * @return false 
 */
bool D3DApp::Initialize()
{
    if (!InitMainWindow()){
        return false;
    }
    if (!InitDirect3D()){
        return false;
    }
    // 执行初始调整大小代码
    // Do the initial resize code.
    OnResize();
    return true;
}


/* 操作基本功能 */
/**
 * @brief 创建描述符堆
 * 
 */
void D3DApp::CreateRtvAndDsvDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    ThrowIfFailed(md3dDevice -> CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    ThrowIfFailed(md3dDevice -> CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
}

void D3DApp::OnResize()
{
    assert(md3dDevice);
    assert(mSwapChain);
    assert(mDirectCmdListAlloc);

    // 在更改任何资源之前刷新
    // Flush before changing any resources.
    FlushCommandQueue();

    ThrowIfFailed(mCommandList -> Reset(mDirectCmdListAlloc.Get(), nullptr));

    // 释放我们将重新创建的先前资源
    // Release the previous resources we will be recreating.
    for (int i = 0; i < SwapChainBufferCount; ++ i){
        mSwapChainBuffer[i].Reset();
    }
    mDepthStencilBuffer.Reset();

    // 调整交换链的大小
    // Resize the swap chain.
    ThrowIfFailed(mSwapChain -> ResizeBuffers(
        SwapChainBufferCount,
        mClientWidth, mClientHeight,
        mBackBufferFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    ));

    mCurrBackBuffer = 0;
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap -> GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < SwapChainBufferCount; ++ i){
        ThrowIfFailed(mSwapChain -> GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
        md3dDevice -> CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, mRtvDescriptorSize);
    }

    // 创建深度/模板缓冲区和视图
    // Create the depth/stencil buffer and view.
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = mClientWidth;
    depthStencilDesc.Height = mClientHeight;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = mDepthStencilFormat;
    depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = mDepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;
    ThrowIfFailed(md3dDevice -> CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())
    ));

    // 使用资源的格式创建描述符以 mip 级别 0 的整个资源
    // Create descriptor to mip level 0 of entire resource using the format of the resource.
    md3dDevice -> CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, DepthStencilView());

    // 将资源从其初始状态转换为深度缓冲区
    // Transition the resource from its initial state to be used as a depth buffer.
    mCommandList -> ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE    
    ));

    // 执行调整大小命令
    // Execute the resize commands.
    ThrowIfFailed(mCommandList -> close());
    ID3D12CommandList* cmdsLists[] = {mCommandList.Get()};
    mCommandQueue -> ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until resize is complete.
	FlushCommandQueue();

    // 更新视口变换以覆盖客户区
    // Update the viewport transform to cover the client area.
    mScreenViewport.TopLeftX = 0;
    mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

    mScissorRect = {0, 0, mClientWidth, mClientHeight};
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::CurrentBackBufferView() const {
    // CD3DX12 构造函数根据给定的偏移量找到当前后台缓冲区RTV
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(mRtvHeap -> GetCPUDescriptorHandleForHeapStart(),
                                        mCurrBackBuffer,
                                        mRtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::DepthStencilView() const{
    return mDsvHeap -> GetCPUDescriptorHandleForHeapStart();
}


/* Direct3D功能 */
/**
 * @brief 初始化应用程序主窗口
 * 
 * @return true 
 * @return false 
 */
bool D3DApp::InitMainWindow()
{
    WNDCLASS = wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
	wc.hInstance  = mhAppInst;
	wc.hIcon  = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

    if( !RegisterClass(&wc) )
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

    // 根据请求的客户区尺寸计算窗口矩形尺寸
    // Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, mClientWidth, mClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	mhMainWnd = CreateWindow(L"MainWnd", mMainWndCaption.c_str(), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, 0); 
	if( !mhMainWnd )
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}
    ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
    return true;
}

/**
 * @brief 完成Direct3D的初始化
 * 
 * @return true 
 * @return false 
 */
bool D3DApp::InitDirect3D()
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
{
	ComPtr<ID3D12Debug> debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
}
#endif

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

#ifdef _DEBUG
    LogAdapters();
#endif

    CreateCommandObjects();
    CreateSwapChain();
    CreateRtvAndDsvDescriptorHeaps();
    return true;
}

/**
 * @brief 创建命令队列、命令列表分配器和命令列表
 * 
 */
void D3DApp::CreateCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    ThrowIfFailed(md3dDevice -> CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
    ThrowIfFailed(md3DDevice -> CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf()));
    ThrowIfFailed(md3dDevice -> CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, 
                // 关联命令分配器
                mDirectCmdListAlloc.Get(),
                // 初始化流水线状态对象
                nullptr,
                IID_PPV_ARGS(mCommandList.GetAddressOf())));
    // 首先要将命令列表置于关闭状态.
    // 第一次引用命令列表时，需要对它进行重置
    // 而在调用重置方法之前又需先将其关闭
    mCommandList -> close();
}

/**
 * @brief 创建交换链
 * 
 */
void D3DAPP::CreateSwapChain()
{
    // 释放之前所创的交换链，随后进行重建
    mSwapChain.Reset();

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = mClientWidth;
    sd.BufferDesc.Height = mClientHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = mBackBufferFormat;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = SwapChainBufferCount;
    sd.OutputWindow = mhMainWnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    // 交换链需要通过命令队列对其进行刷新
    ThrowIfFailed(mdxgiFactory -> CreateSwapChain(
        mCommandQueue.Get(),
        &sd,
        mSwapChain.GetAddressOf()
    ));
}

/**
 * @brief 使用fence刷新命令队列
 * 
 */
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
 * @brief 返回交换链中当前后台缓冲区的ID3D12Resource
 * 
 * @return ID3D12Resource* 
 */
ID3D12Resource* D3DApp::CurrentBackBuffer() const
{
    return mSwapChainBuffer[mCurrBackBuffer].Get();
}

/**
 * @brief 返回当前缓冲区的RTV
 * 
 * @return D3D12_CPU_DESCRIPTOR_HANDLE 
 */
D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::CurrentBackBufferView() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
        mCurrBackBuffer,
        mRtvDescriptorSize
    );
}

/**
 * @brief 返回主深度/模板缓冲区的DSV
 * 
 * @return D3D12_CPU_DESCRIPTOR_HANDLE 
 */
D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::DepthStencilView()const
{
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

/**
 * @brief 计算每秒的平均帧数以及每帧平均的毫秒时长
 * 
 */
void D3DApp::CalculateFrameStats()
{

    // 代码计算每秒的平均帧数，以及
    // 渲染一帧的平均时间。 这些统计
    // 附加到窗口标题栏。
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
    
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

    // 计算一秒内的平均值
	// Compute averages over one second period.
	if( (mTimer.TotalTime() - timeElapsed) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

        wstring fpsStr = to_wstring(fps);
        wstring mspfStr = to_wstring(mspf);

        wstring windowText = mMainWndCaption +
            L"    fps: " + fpsStr +
            L"   mspf: " + mspfStr;

        SetWindowText(mhMainWnd, windowText.c_str());
		
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

/**
 * @brief 枚举一个系统中的所有适配器
 * 
 */
void D3DApp::LogAdapter()
{
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
void D3DApp::LogAdapterOutputs(IDXGIAdapter* adapter)
{
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
void D3DApp::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{

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