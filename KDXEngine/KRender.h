#pragma once
#include "KCamera.h"
#include "KTransform.h"
#include "KShader.h"
#include "KMesh.h"
#include <KGAMEDIR.h>

enum class CBMode
{
	CM_LINK,
	CM_NEW,
	MAX
};

class KLightManager;
class KMesh;
class KRenderPipeline;
class KTexture;
class KSampler;
class KConstantBuffer;
class KRenderPlayer : public KGameReference, public KUpdateBase
{
private:
	KGameString m_Name;

public:
	KRenderManager* m_Parent;
	D3D11_PRIMITIVE_TOPOLOGY m_MeshRenderType;
	KPTR<KMesh> m_mesh;
	KPTR<KRenderPipeline> m_CurrentRenderPipeline;
	//1: normal, 2: specular
	int m_RenderOption[4];

	friend KRenderManager;

	class KTextureSetter
	{
	private:
		friend class KRenderPlayer;
		friend class KRenderManager;
		const KShaderData* m_Data;
		KPTR<KTexture> m_Texture;
		bool m_IsExistTexture;

	public:
		KTextureSetter() : m_Data(nullptr), m_Texture(nullptr), m_IsExistTexture(false) {		}
		KTextureSetter(KShaderData* _Data, KPTR<KTexture> _Res) : m_Data(_Data), m_Texture(_Res), m_IsExistTexture(true) {}
	};

	class KConstantBufferSetter
	{
		KGameString m_Name;
		CBMode m_Mode;
		friend class KRenderPlayer;
		friend class KRenderManager;
		const KShaderData* m_pShaderData;
		KPTR<KConstantBuffer> m_ConstantBuffer;

		void* m_pSettingData;

	public:
		KConstantBufferSetter() : m_pShaderData(nullptr), m_ConstantBuffer(nullptr), m_pSettingData(nullptr), m_Mode(CBMode::CM_LINK) {		}
		KConstantBufferSetter(KGameString _name, KShaderData* _Data, KPTR<KConstantBuffer> _Res) : m_pShaderData(_Data), m_ConstantBuffer(_Res), m_pSettingData(nullptr), m_Mode(CBMode::CM_LINK)
		{
			m_Name = _name;
		}

		~KConstantBufferSetter()
		{
			if (m_Mode == CBMode::CM_NEW)
			{
				if (nullptr != m_pSettingData)
				{
					delete m_pSettingData;
					m_pSettingData = nullptr;
				}
			}
		}
	};

	class KSamplerSetter
	{
		friend class KRenderPlayer;
		friend class KRenderManager;
		const KShaderData* m_Data;
		KPTR<KSampler> m_Sampler;

	public:
		KSamplerSetter() : m_Data(nullptr), m_Sampler(nullptr) {		}
		KSamplerSetter(KShaderData* _Data, KPTR<KSampler> _Res) : m_Data(_Data), m_Sampler(_Res) {		}
	};

private:
	std::vector<std::map<KGameString, KTextureSetter>> m_TextureContainer;
	std::vector<std::map<KGameString, KSamplerSetter>> m_SamplerContainer;
	std::vector<std::map<KGameString, KConstantBufferSetter>> m_ConstantBufferContainer;

public:
	void SetterInit();
	void ShadowOn();
	void ShadowRender();

	void SetMesh(const KGameString& _Name);
	void SetMesh(const KPTR<KMesh>& _Name);
	void SetRenderPipeline(const KGameString& _Name);

public:
	void Init(const KGameString& _MeshName, const KGameString& _RPName);

	bool IsExistTexture(const KGameString& _name);
	bool IsExistSampler(const KGameString& _name);
	bool IsExistContantBuffer(const KGameString& _name);

	std::list<KTextureSetter*> TextureSetterList(const KGameString& _name);
	std::list<KSamplerSetter*> SamplerSetterList(const KGameString& _name);
	std::list<KConstantBufferSetter*> ConstantBufferSetterList(const KGameString& _name);

	void SetConstantBuffer(const KGameString& _name, void* pData, CBMode _Mode);
	void SetSampler(const KGameString& _name, const KGameString& _samplerName);
	void SetTexture(const KGameString& _name, const KPTR<KTexture>& _texture);
	void SetTexture(const KGameString& _name, const KGameString& _textureName);

	void SetRenderConstantBuffer(const KGameString& _name);
	void SetRenderSampler(const KGameString& _name);
	void SetRenderTexture(const KGameString& _name);

public:
	void Render();

public:
	KRenderPlayer();
	KRenderPlayer(const KGameString& _meshName, const KGameString& _renderPipelineName);
};

class KCamera;
class KRenderManager : public KTransform
{
private:
	friend KRenderPlayer;

private:
	std::vector<KPTR<KRenderPlayer>> m_RenderPlayerContainer;
	std::vector<KPTR<KRenderPlayer>> m_ForwardRenderPlayerContainer;
	std::vector<KPTR<KRenderPlayer>> m_DefferdRenderPlayerContainer;
	std::vector<KPTR<KRenderPlayer>> m_ShadowRenderPlayerContainer;

public:
	void SetTexture(const KGameString& _NAME, const KGameString& _RESNAME, int _PlayerIndex = 0);
	void SetSampler(const KGameString& _NAME, const KGameString& _RESNAME, int _PlayerIndex = 0);

	template<typename T>
	void SetConstantBuffer(const KGameString& _NAME, T& Data, CBMode _Mode = CBMode::CM_LINK, int _PlayerIndex = 0)
	{
		SetConstantBuffer(_NAME, (void*)&Data, _Mode, _PlayerIndex);
	}

	template<typename T>
	void SetAllConstantBuffer(const KGameString& _name, T& Data, CBMode _Mode = CBMode::CM_LINK)
	{
		for (int i = 0; i < m_RenderPlayerContainer.size(); i++)
		{
			SetConstantBuffer(_name, (void*)&Data, _Mode, i);
		}
	}

	void SetConstantBuffer(const KGameString& _NAME, void* Data, CBMode _Mode = CBMode::CM_LINK, int _PlayerIndex = 0);

public:
	KPTR<KRenderPlayer> CreateRenderPlayer(const KGameString& _MeshName, const KGameString& _RenderPipeLineName);
	KPTR<KRenderPlayer> CreateRenderPlayer(const KPTR<KMesh>& _mesh, const KGameString& _RenderPipeLineName);

	std::vector<KPTR<KRenderPlayer>> CreateRenderPlayerToFbx(const KGAMEDIR& _dir, const KGameString& _FbxName, const KGameString& _MaterialName, const KGameString& _SamplerName);

public:
	void StartData(int _Order = 0);
	void StartData(const KGameString& _MeshName, const KGameString& _RenderPipeLineName, int _Order = 0);
	void StartData(const KGAMEDIR& _dir, const KGameString& _fbxName, const KGameString& _MaterialName, const KGameString& _SamplerName, int _order = 0);

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

