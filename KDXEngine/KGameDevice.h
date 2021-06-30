#pragma once
#include <KGAMEMATH.h>
#include <KGAMEWIN.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#pragma comment(lib, "KEngineBase.lib")

class KRenderTarget;
class KGameDevice : public KGameReference
{
private:
	static ID3D11Device* m_pMainDevice;
	static ID3D11DeviceContext* m_pMainContext;
	static KGameDevice* m_pMainGameDevice;

public:
	static ID3D11Device* MainDevice();
	static ID3D11DeviceContext* MainContext();
	static KGameDevice* MainGameDevice();

	static void SetMainRenderTarget();

private:
	KPTR<KGameWindow> m_GameWindow;


private:
	static std::map<KGameString, KPTR<KGameDevice>> m_AllDevice;

public:
	static KPTR<KGameDevice> Find(const KGameString& _DeviceName);
	static void Create(const KGameString& _WindowName, KVector _ClearColor);

private:
	UINT m_MultiQualityLevel; // 멀티샘플링 퀄리티
	UINT m_MultiSamplerCounter; // 멀티샘플링 카운트

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	IDXGISwapChain* m_pSwapChain;

	D3D11_VIEWPORT m_ViewPort;

public:
	KPTR<KRenderTarget> MainTarget;

public:
	void DeviceRenderEnd();


private:
	void Create(KPTR<KGameWindow> _Window, KVector _ClearColor);
	bool CreateSwapChain(); // 스왑체인 만들기
	bool CreateBackBuffer(KVector _ClearColor); // 백버퍼 얻어오기

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

