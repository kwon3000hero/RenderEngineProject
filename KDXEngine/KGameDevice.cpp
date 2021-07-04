#include "KGameDevice.h"
#include "KRenderTarget.h"
#include "KGameDebug3D.h"
#include "KGameFont.h"
#include "KGameMacros.h"
#include "KRenderPipeline.h"

ID3D11Device* KGameDevice::m_pMainDevice = nullptr;
ID3D11DeviceContext* KGameDevice::m_pMainContext = nullptr;
KGameDevice* KGameDevice::m_pMainGameDevice = nullptr;

std::map<KGameString, KPTR<KGameDevice>> KGameDevice::m_AllDevice;

void KGameDevice::SetMainRenderTarget()
{
	m_pMainGameDevice->MainTarget->Clear();
	m_pMainGameDevice->MainTarget->Setting();
}

KPTR<KGameDevice> KGameDevice::Find(const KGameString& _DeviceName)
{
	if (m_AllDevice.end() == m_AllDevice.find(_DeviceName))
	{
		return nullptr;
	}

	return m_AllDevice[_DeviceName];
}

void KGameDevice::Create(const KGameString& _WindowName, KVector _ClearColor)
{
	if (nullptr != Find(_WindowName).get())
	{
		assert(false);
	}

	KPTR<KGameDevice> NewDevice = new KGameDevice();

	NewDevice->Create(KGameWindow::FindWin(_WindowName), _ClearColor);

	m_AllDevice.insert(std::map<KGameString, KPTR<KGameDevice>>::value_type(_WindowName, NewDevice));
}

KGameDevice::KGameDevice() : m_GameWindow(nullptr), m_MultiQualityLevel(0)
, m_MultiSamplerCounter(0), m_pDevice(nullptr), m_pContext(nullptr), m_pSwapChain(nullptr), m_ViewPort{ 0 }
{
}
KGameDevice::~KGameDevice()
{
	KGameFont::End();
	SafeRelease(m_pSwapChain);
	SafeRelease(m_pContext);
	SafeRelease(m_pDevice);
}

void KGameDevice::Create(KPTR<KGameWindow> _Window, KVector _ClearColor)
{
	if (nullptr == _Window.get())
	{
		assert(false);
	}

	m_GameWindow = _Window;

	unsigned int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL ReturneLv = D3D_FEATURE_LEVEL_9_1;

	HRESULT HR = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &ReturneLv, &m_pContext);

	if (S_OK != HR || nullptr == m_pDevice || nullptr == m_pContext)
	{
		assert(false);
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
			assert(false);
		}
	}

	if (nullptr == m_pMainDevice)
	{
		m_pMainDevice = m_pDevice;
		m_pMainContext = m_pContext;
		m_pMainGameDevice = this;


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


	if (false == CreateSwapChain())
	{
		assert(false);
	}

	if (false == CreateBackBuffer(_ClearColor))
	{
		assert(false);
	}

}


bool KGameDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC SCDECS;
	memset(&SCDECS, 0, sizeof(DXGI_SWAP_CHAIN_DESC));


	SCDECS.BufferDesc.Width = m_GameWindow->Size().UIX();
	SCDECS.BufferDesc.Height = m_GameWindow->Size().UIY();

	SCDECS.BufferDesc.RefreshRate.Numerator = 60;
	SCDECS.BufferDesc.RefreshRate.Denominator = 1;

	SCDECS.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SCDECS.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SCDECS.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SCDECS.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SCDECS.SampleDesc.Quality = 0;
	SCDECS.SampleDesc.Count = 1;
	SCDECS.OutputWindow = m_GameWindow->WINHWND();

	// 이걸 2개로 만들어서 처리
	SCDECS.BufferCount = 2;
	SCDECS.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SCDECS.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SCDECS.Windowed = true;
	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
	if (nullptr == pDXGIDevice)
	{
		assert(false);
		return false;
	}

	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
	if (nullptr == pAdapter)
	{
		assert(false);
		return false;
	}

	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (nullptr == pFactory)
	{
		assert(false);
		return false;
	}

	if (S_OK != pFactory->CreateSwapChain(m_pDevice, &SCDECS, &m_pSwapChain))
	{
		assert(false);
		return false;
	}


	SafeRelease(pDXGIDevice);
	SafeRelease(pAdapter);
	SafeRelease(pFactory);

	return true;
}

bool KGameDevice::CreateBackBuffer(KVector _ClearColor)
{
	ID3D11Texture2D* BackTex = nullptr;

	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackTex))
	{
		assert(false);
		return false;
	}

	MainTarget = new KRenderTarget();
	MainTarget->Create(BackTex, _ClearColor, D3D11_BIND_RENDER_TARGET);
	MainTarget->CreateDepth();

	return true;
}


void KGameDevice::DeviceRenderEnd()
{
	m_pSwapChain->Present(0, 0);
	KRenderPipeline::ResetAll();
}

ID3D11Device* KGameDevice::MainDevice()
{
	return m_pMainDevice;
}

ID3D11DeviceContext* KGameDevice::MainContext()
{
	return m_pMainContext;
}

KGameDevice* KGameDevice::MainGameDevice()
{
	return m_pMainGameDevice;
}