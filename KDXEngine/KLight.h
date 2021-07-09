#pragma once
#include "KCamera.h"
#include "KTransform.h"
#include "KShader.h"
#include "KRenderPipeline.h"
#include "KMesh.h"

struct LightData
{
	KVector DiffuseColor;
	KVector SpecularColor;
	KVector AmbientColor;
	KVector LightPower; // X´Â DiffuseColor Y SpecularColor Z AmbientColor 
	KVector LightColor;

	KVector LightPos;
	KVector LightDir;
	KVector LightDirInv;
	KVector LightRange;

	KVector CameraPosition;

	KVector ShadowOption;
	KVector Dummy;
	KMatrix CamInversView;
	KMatrix LightView;
	KMatrix LightProj;
	KMatrix LightVP;
};

struct LightDataBuffer
{
	int LightCount[4];
	LightData LightDataArray[128];
};

class KGameScene;
class KLightManager : public KGameComponent
{
public:
	friend KGameScene;

private:
	KVector m_ShadowTargetSize;
	LightData m_LightData;
	std::set<int> m_ViewGroup;

public:
	bool IsView(int _groupIndex)
	{
		if (m_ViewGroup.end() == m_ViewGroup.find(_groupIndex))
		{
			return false;
		}

		return true;
	}

private:
	template<typename ... STARTDATA>
	void PushViewGroup(int _GroupOrder, STARTDATA ... _Arg) {
		m_ViewGroup.insert(_GroupOrder);
	}
	void PushViewGroup() {	}

public:
	void Init() override;
	void PrevRender() override;

public:
	KLightManager();
	
	template<typename ... STARTDATA>
	KLightManager(int _Order, STARTDATA ... _Arg) : KLightManager()
	{
		SetOrder(_Order);
		PushViewGroup(_Arg...);
	}
	virtual ~KLightManager();

public:
	class ShadowRenderData
	{
	public:
		KMatrix LightVP;
		KMatrix InverseView;
		KVector m_ShadowData;
	};

private:
	KPTR<KRenderTarget> m_ShadowTarget;
	ShadowRenderData m_ShadowRenderData;

public:
	KMatrix View();
	KMatrix Projection();
	void SetShadowTarget();

	KPTR<KRenderTarget> ShadowTarget()
	{
		return m_ShadowTarget;
	}
};

