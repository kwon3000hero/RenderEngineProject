#include "KGameDevice.h"
#include "KRenderTarget.h"
#include "KGameDebug3D.h"
#include "KGameFont.h"
#include "KGameMacros.h"
#include "KRenderPipeline.h"

ID3D11Device* KGameDevice::m_pMainDevice = nullptr;
ID3D11DeviceContext* KGameDevice::m_pMainContext = nullptr;
KGameDevice* KGameDevice::m_pMainGameDevice = nullptr;

std::map<KGameString, KPTR<KGameDevice>> KGameDevice::m_deviceContainer;

void KGameDevice::SetMainRenderTarget()
{
	m_pMainGameDevice->MainTarget->Clear();
	m_pMainGameDevice->MainTarget->Setting();
}

KPTR<KGameDevice> KGameDevice::Find(const KGameString& _DeviceName)
{
	if (m_deviceContainer.end() == m_deviceContainer.find(_DeviceName))
	{
		return nullptr;
	}

	return m_deviceContainer[_DeviceName];
}

void KGameDevice::Create(const KGameString& _WindowName, KVector _ClearColor)
{
	if (nullptr != Find(_WindowName).get())
	{
		assert(false);
	}

	KPTR<KGameDevice> NewDevice = new KGameDevice();
	NewDevice->Create(KGameWindow::FindWin(_WindowName), _ClearColor);

	m_deviceContainer.insert(std::map<KGameString, KPTR<KGameDevice>>::value_type(_WindowName, NewDevice));
}

KGameDevice::KGameDevice() : m_GameWindow(nullptr), m_MultiQualityLevel(0)
, m_MultiSamplerCounter(0), m_pDevice(nullptr), m_pContext(nullptr), m_ViewPort{ 0 }
{
}
KGameDevice::~KGameDevice()
{
	KGameFont::End();
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

	m_swapChainManager.SearchAdapterAndOutput();

	unsigned int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	KSwapChain* pSwapChain = m_swapChainManager.GetOutput(0, 0);
	m_pSwapChain = pSwapChain;

	D3D_FEATURE_LEVEL ReturneLv = D3D_FEATURE_LEVEL_9_1;


	IDXGIAdapter* pAdapter = m_pSwapChain->Adpater();
	HRESULT HR = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
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


	if (false == m_pSwapChain->CreateSwapChain(m_pDevice, m_GameWindow))
	{
		assert(false);
	}

	if (false == CreateBackBuffer(_ClearColor))
	{
		assert(false);
	}

}



bool KGameDevice::CreateBackBuffer(KVector _ClearColor)
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


void KGameDevice::DeviceRenderEnd()
{
	m_pSwapChain->SwapChain()->Present(0, 0);
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