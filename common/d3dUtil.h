﻿#pragma execution_character_set("utf-8")
#pragma once



// WIN
#include <windows.h>
#include <wrl.h>
// DIRECTX
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <d3d12.h>

// C++
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>

// namespace
using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

// 链接动态库
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

inline std::wstring AnsiToWString(const std::string& str) {
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

	std::wstring ToString()const;

	HRESULT ErrorCode = S_OK;
	std::wstring FunctionName;
	std::wstring Filename;
	int LineNumber = -1;
};


#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
	HRESULT hr__ = (x);                                               \
	std::wstring wfn = AnsiToWString(__FILE__);                       \
	if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x -> Release(); x = 0; } }
#endif