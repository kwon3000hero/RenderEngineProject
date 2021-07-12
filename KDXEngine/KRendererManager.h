#pragma once

#include "KTransform.h"
#include <KGAMEDIR.h>
#include "KRenderer.h"



class KMesh;
class KCamera;
class KRenderer;
class KRenderManager : public KTransform
{
private:
	friend KRenderer;

private:
	std::vector<KPTR<KRenderer>> m_RenderPlayerContainer;
	std::vector<KPTR<KRenderer>> m_ForwardRenderPlayerContainer;
	std::vector<KPTR<KRenderer>> m_DefferdRenderPlayerContainer;
	std::vector<KPTR<KRenderer>> m_ShadowRenderPlayerContainer;

public:
	KRenderManager();
	KRenderManager(const KGameString& _MeshName, const KGameString& _RenderPipeLineName, int _Order = 0);
	KRenderManager(const KGAMEDIR& _dir, const KGameString& _fbxName, const KGameString& _MaterialName, const KGameString& _SamplerName, int _order = 0);
	KRenderManager(int _Order);
	virtual ~KRenderManager();

public:
	void SetTexture(const KGameString& _NAME, const KGameString& _RESNAME, int _PlayerIndex = 0);
	void SetSampler(const KGameString& _NAME, const KGameString& _RESNAME, int _PlayerIndex = 0);

	template<typename T>
	void SetConstantBuffer(const KGameString& _NAME, T& Data, ConstantBufferMode _Mode = ConstantBufferMode::Link, int _PlayerIndex = 0)
	{
		SetConstantBuffer(_NAME, (void*)&Data, _Mode, _PlayerIndex);
	}

	template<typename T>
	void SetAllConstantBuffer(const KGameString& _name, T& Data, ConstantBufferMode _Mode = ConstantBufferMode::Link)
	{
		for (int i = 0; i < m_RenderPlayerContainer.size(); i++)
		{
			SetConstantBuffer(_name, (void*)&Data, _Mode, i);
		}
	}

	void SetConstantBuffer(const KGameString& _NAME, void* Data, ConstantBufferMode _Mode = ConstantBufferMode::Link, int _PlayerIndex = 0);

public:
	KPTR<KRenderer> CreateRenderPlayer(const KGameString& _MeshName, const KGameString& _RenderPipeLineName);
	KPTR<KRenderer> CreateRenderPlayer(const KPTR<KMesh>& _mesh, const KGameString& _RenderPipeLineName);

	std::vector<KPTR<KRenderer>> CreateRenderPlayerToFbx(const KGAMEDIR& _dir, const KGameString& _FbxName, const KGameString& _MaterialName, const KGameString& _SamplerName);

public:
	virtual void Init();
	virtual void Render(KPTR<KCamera> _ViewCam);
	virtual void ForwardRender(KPTR<KCamera> _ViewCam);
	virtual void DefferdRender(KPTR<KCamera> _ViewCam);
	virtual void ShadowRender(KPTR<KLightManager> _light);

	bool IsBump();
	void BumpOn();
	void RenderPlayerBumpOn(int _index);
	void BumpOff();
	void RenderPlayerBumpOff(int _index);
	void ShadowOn();
	void RenderPlayerShadowOn(int _index);
};