#pragma once
#include "KGameDevice.h"
#include "KRenderTarget.h"
#include "KGameDebug3D.h"
#include "KGameFont.h"
#include "KGameMacros.h"
#include "KRenderPipeline.h"

template<typename T>
KGameDevice<T>::KGameDevice() : m_GameWindow(nullptr), m_MultiQualityLevel(0)
, m_MultiSamplerCounter(0), m_pDevice(nullptr), m_pContext(nullptr), m_ViewPort{ 0 }
{
}


template<typename T>
KGameDevice<T>::~KGameDevice()
{
	KGameFont::End();
}

template<typename T>
bool KGameDevice<T>::CreateBackBuffer(KVector _ClearColor)
{
	ID3D11Texture2D* BackTex = nullptr;

	if (S_OK != m_pSwapChain->SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackTex))
	{
		assert(false);
		return false;
	}

	MainTarget = new KRenderTarget();
	MainTarget->Create(BackTex, _ClearColor, D3D11_BIND_RENDER_TARGET);
	MainTarget->CreateDepth();

	return true;
}

template<typename T>
void KGameDevice<T>::DeviceRenderEnd()
{
	m_pSwapChain->SwapChain()->Present(0, 0);
	KRenderPipeline::ResetAll();
}

#if DeviceVersion == DeviceWrapperVer5
template<>
void KGameDevice<KDeviceWrapper5>::Create(KPTR<KGameWindow> _Window, KVector _ClearColor)
{
	if (nullptr == _Window.get())
	{
		assert(false);
	}

	m_GameWindow = _Window;

	m_swapChainManager.SearchAdapterAndOutput();

	unsigned int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	KSwapChain<SwapChainSelectedWrapper>* pSwapChain = m_swapChainManager.GetOutput(0, 0);
	m_pSwapChain = pSwapChain;

	D3D_FEATURE_LEVEL ReturneLv = D3D_FEATURE_LEVEL_9_1;


	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = m_pSwapChain->Adapter();
	HRESULT HR = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
		nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, reinterpret_cast<ID3D11Device**>(device.Get()), &ReturneLv, 
		reinterpret_cast<ID3D11DeviceContext**>(deviceContext.Get()));

	if (S_OK != HR || nullptr == device.Get() || nullptr == deviceContext.Get())
	{
		ShowHResultString(HR, "CreateDevcie");
	}

	Microsoft::WRL::ComPtr<ID3D11Device1> device1;
	device->QueryInterface(IID_PPV_ARGS(&device1));

	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> deviceContext1;
	deviceContext->QueryInterface(IID_PPV_ARGS(&deviceContext1));

	Microsoft::WRL::ComPtr<ID3D11Device2> device2;
	device1->QueryInterface(IID_PPV_ARGS(&device2));

	Microsoft::WRL::ComPtr<ID3D11DeviceContext2> deviceContext2;
	deviceContext1->QueryInterface(IID_PPV_ARGS(&deviceContext2));

	Microsoft::WRL::ComPtr<ID3D11Device3> device3;
	device2->QueryInterface(IID_PPV_ARGS(&device3));

	Microsoft::WRL::ComPtr<ID3D11DeviceContext3> deviceContext3;
	deviceContext2->QueryInterface(IID_PPV_ARGS(&deviceContext3));

	Microsoft::WRL::ComPtr<ID3D11Device4> device4;
	device3->QueryInterface(IID_PPV_ARGS(&device4));
	
	deviceContext3->QueryInterface(IID_PPV_ARGS(&m_pContext));
	
	device4->QueryInterface(IID_PPV_ARGS(&m_pDevice));
	

	m_MultiSamplerCounter = 4;
	HR = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
		m_MultiSamplerCounter, &m_MultiQualityLevel);

	if (S_OK != HR)
	{
		m_MultiSamplerCounter = 1;
		m_MultiQualityLevel = 0;
		HR = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MultiSamplerCounter, &m_MultiQualityLevel);
		if (S_OK != HR)
		{
			ShowHResultString(HR, "CheckMultisampleQualityLevels");
		}
	}

	if (nullptr == KGameDeviceManager::MainDevice().Get())
	{
		KGameDeviceManager::SetMainDevice(m_pDevice);
		KGameDeviceManager::SetMainContext(m_pContext);
		KGameDeviceManager::SetMainGameDevice(this);


		InitDefaultMesh();
		InitBlendState();
		InitSamplerState();
		InitDepthStencilState();
		InitShader();
		InitRasterizer();
		InitRenderPipeline();
		KGameFont::Init();
		KGameDebug3D::Init();
		KRenderTarget::Init();
	}


	if (false == m_pSwapChain->CreateSwapChain(m_pDevice, m_GameWindow))
	{
		assert(false);
	}

	if (false == CreateBackBuffer(_ClearColor))
	{
		assert(false);
	}

}
#endif

#if DeviceVersion == DeviceWrapperVer0  
template<>
void KGameDevice<KDeviceWrapper0>::Create(KPTR<KGameWindow> _Window, KVector _ClearColor)
{
	if (nullptr == _Window.get())
	{
		assert(false);
	}

	m_GameWindow = _Window;

	m_swapChainManager.SearchAdapterAndOutput();

	unsigned int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	KSwapChain<SwapChainSelectedWrapper>* pSwapChain = m_swapChainManager.GetOutput(0, 0);
	m_pSwapChain = pSwapChain;

	D3D_FEATURE_LEVEL ReturneLv = D3D_FEATURE_LEVEL_9_1;


	Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = m_pSwapChain->Adapter();
	HRESULT HR = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
		nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &ReturneLv, &m_pContext);

	if (S_OK != HR || nullptr == m_pDevice || nullptr == m_pContext)
	{
		ShowHResultString(HR, "CreateDevcie");
	}

	m_MultiSamplerCounter = 4;
	HR = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
		m_MultiSamplerCounter, &m_MultiQualityLevel);

	if (S_OK != HR)
	{
		m_MultiSamplerCounter = 1;
		m_MultiQualityLevel = 0;
		HR = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MultiSamplerCounter, &m_MultiQualityLevel);
		if (S_OK != HR)
		{
			ShowHResultString(HR, "CheckMultisampleQualityLevels");
		}
	}

	if (nullptr == KGameDeviceManager::MainDevice().Get())
	{
		KGameDeviceManager::SetMainDevice(m_pDevice);
		KGameDeviceManager::SetMainContext(m_pContext);
		KGameDeviceManager::SetMainGameDevice(this);


		InitDefaultMesh();
		InitBlendState();
		InitSamplerState();
		InitDepthStencilState();
		InitShader();
		InitRasterizer();
		InitRenderPipeline();
		KGameFont::Init();
		KGameDebug3D::Init();
		KRenderTarget::Init();
	}


	if (false == m_pSwapChain->CreateSwapChain(m_pDevice, m_GameWindow))
	{
		assert(false);
	}

	if (false == CreateBackBuffer(_ClearColor))
	{
		assert(false);
	}

}
#endif