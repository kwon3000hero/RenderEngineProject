#pragma once
#include <KGAMEMATH.h>
#include <KGameWindow.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include "KSwapChainManager.h"

class KRenderTarget;
class KGameDeviceManager;
class KGameDevice final : public KGameReference
{
//private:
//	static Microsoft::WRL::ComPtr<ID3D11Device> m_pMainDevice;
//	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pMainContext;
//	static KGameDevice* m_pMainGameDevice;
//
//public:
//	static Microsoft::WRL::ComPtr<ID3D11Device> MainDevice();
//	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> MainContext();
//	static KGameDevice* MainGameDevice();
//
//	static void SetMainRenderTarget();

private:
	friend KGameDeviceManager;

private:
	KPTR<KGameWindow> m_GameWindow;

	KSwapChainManager<SwapChainBuildVersion> m_swapChainManager;
	KSwapChain<SwapChainBuildVersion>* m_pSwapChain;

//private:
//	static std::map<KGameString, KPTR<KGameDevice>> m_deviceContainer;
//
//public:
//	static KPTR<KGameDevice> Find(const KGameString& _DeviceName);
//	static void Create(const KGameString& _WindowName, KVector _ClearColor);

private:
	UINT m_MultiQualityLevel;
	UINT m_MultiSamplerCounter;

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;

	D3D11_VIEWPORT m_ViewPort;

public:
	KPTR<KRenderTarget> MainTarget;

public:
	void DeviceRenderEnd();


private:
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

