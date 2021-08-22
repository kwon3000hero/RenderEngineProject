#pragma once
#include <KGAMEMATH.h>
#include <KGameWindow.h>
#include <d3dcompiler.h>
#include "KDeviceWrapper.h"
#include "KSwapChainManager.h"
#include "KGameFont.h"

class KRenderTarget;
class KGameDeviceManager;

template<typename T>
class KGameDevice final : public KGameReference
{
private:
	friend KGameDeviceManager;

private:
	typedef typename T::DeviceType DeviceType;
	typedef typename T::ContextType ContextType;

private:
	KPTR<KGameWindow> m_GameWindow;

	KSwapChainManager<SwapChainSelectedWrapper> m_swapChainManager;
	KSwapChain<SwapChainSelectedWrapper>* m_pSwapChain;

private:
	UINT m_MultiQualityLevel;
	UINT m_MultiSamplerCounter;

	Microsoft::WRL::ComPtr<DeviceType> m_pDevice;
	Microsoft::WRL::ComPtr<ContextType> m_pContext;

	D3D11_VIEWPORT m_ViewPort;

public:
	KPTR<KRenderTarget> MainTarget;

public:
	void DeviceRenderEnd();


public:
	void Create(KPTR<KGameWindow> _Window, KVector _ClearColor);
	bool CreateBackBuffer(KVector _ClearColor);

public:
	void InitBlendState();
	void InitDepthStencilState();
	void InitSamplerState();
	void InitDefaultMesh();
	void InitRasterizer();
	void InitShader();
	void InitRenderPipeline();

public:
	KGameDevice();
	~KGameDevice();
};