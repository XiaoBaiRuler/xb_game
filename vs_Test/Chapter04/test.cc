// #include "../../common/d3dUtil.h"
// int main() {
// 	ComPtr<ID3D12RootSignature> mRootSignature;
// 	// ����ָ��˵ײ�COM�ӿڵ�ָ��
// 	mRootSignature.Get();
// 	// ����ָ��˵ײ�COM�ӿ�ָ��ĵ�ַ
// 	mRootSignature.GetAddressOf();
// 	// ����Ϊnull���ͷ���֮��ص���������(���ٵײ�COM�ӿڵ����ü���)
// 	mRootSignature.Reset();

// 	// �����б�COM�ӿ�
// 	ComPtr<ID3D12GraphicsCommandList> mRootCommandList;

// 	// ��ѯ��Ӧ����������
// 	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
// 	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
// 	msQualityLevels.Format = mBackBufferFormat;
// 	msQualityLevels.SampleCount = 4;
// 	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
// 	msQualityLevels.NumQualityLevels = 0;

// 	ThrowIfFailed()
// 	return EXIT_SUCCESS;
// }