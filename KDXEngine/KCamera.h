#pragma once
#include "KGameComponent.h"
#include <KGAMEMATH.h>
#include <KObjectManager.h>
#include "KRenderTarget.h"
#include "KRender.h"

class KCamera;
class KRenderPlayer;
class KRenderTarget;
class KTransform;
class KRenderTarget;
class KFxaa;
struct LightData;
class KPostEffect : public KGameReference
{
private:
	KPTR<KCamera> m_pCamera;

protected:
	KPTR<KRenderPlayer> m_RenderPlayer;
	KPTR<KRenderTarget> m_EffectTarget;

public:
	friend KCamera;

public:
	KPTR<KCamera> CameraPtr()
	{
		return m_pCamera;
	}

private:
	bool m_IsUpdate;

public:
	bool IsON()
	{
		return  m_IsUpdate;
	}
	void On() 
	{
		m_IsUpdate = true;
	}
	void Off() 
	{
		m_IsUpdate = false;
	}

	void OnOffChange() 
	{
		m_IsUpdate = !m_IsUpdate;
	}

public:
	KPostEffect();
	virtual ~KPostEffect();


public:
	virtual void Init() {}
	virtual void MergeNextEffect() {}
	virtual void MergePrevEffect() {}
	virtual void LightPrevEffect() {}
	virtual void LightNextEffect() {}
};

class KCamera : public KGameComponent, public KObjectManager<KPostEffect>
{
public:

	friend KTransform;
	friend KGameScene;
	friend KFxaa;

	enum class CAMMODE
	{
		ORTH,
		PROJ,
	};

private:
	KMatrix m_ViewMat;
	KMatrix m_ProjMat;

public:
	KMatrix View() { return m_ViewMat; }
	KMatrix Proj() { return m_ProjMat; }

private:
	float	m_OriFov;
	float	m_CurFov;

	KVector m_OriSize;
	KVector m_CurSize;
	float	m_Near;
	float	m_Far;

	CAMMODE m_eMode;

public:
	KCamera();	
	virtual ~KCamera();

public:
	template<typename ... STARTDATA>
	KCamera(int _Order, STARTDATA ... _Arg) : KCamera()
	{
		SetOrder(_Order);
		PushViewGroup(_Arg...);
	}


public:
	void SetFov(const float& _Value) { m_CurFov = _Value; m_OriFov = _Value; };
	void SetSize(const KVector& _Value) { m_OriSize = _Value; m_CurSize = _Value; };
	void SetNear(const float& _Value) { m_Near = _Value; };
	void SetFar(const float& _Value) { m_Far = _Value; };

	void SetMode(CAMMODE _Value) { m_eMode = _Value; };
	CAMMODE Mode() { return m_eMode; };

	void SetZoomIn(float Scale)
	{
		m_CurFov *= (1.0f - Scale);
		m_CurSize *= (1.0f - Scale);
	}
	void SetZoomOut(float Scale)
	{
		m_CurFov *= (1.0f + Scale);
		m_CurSize *= (1.0f + Scale);
	};

	void SetZoomOri()
	{
		m_CurFov = m_OriFov;
		m_CurSize = m_OriSize;
	};


private:
	std::set<int> m_ViewList;


private:
	template<typename ... STARTDATA>
	void PushViewGroup(int _GroupOrder, STARTDATA ... _Arg)
	{
		m_ViewList.insert(_GroupOrder);
	}

	void PushViewGroup()
	{
	}

public:
	void Init() override;
	void UpdateViewAndProjection();
	void MergeTargets();

private:
	KPTR<KRenderTarget> m_currentCameraTarget;
	KPTR<KRenderTarget> m_forwardCameraTarget;
	KPTR<KRenderTarget> m_defferdCameraTarget;

	KPTR<KRenderTarget> m_globalBufferCameraTarget;
	KPTR<KRenderTarget> m_lightCameraTarget;

	KPTR<KRenderPlayer> m_defferdLightPlayer;
	KPTR<KRenderPlayer> m_defferdMergePlayer;

public:
	KPTR<KRenderTarget> CurrentCameraTarget();
	KPTR<KRenderTarget> GlobalBufferTarget();
	KPTR<KRenderTarget> DefferdCameraTarget();
	KPTR<KRenderTarget> LightTarget();

public:
	void SetForwardCameraTarget();
	void SetDeffedCameraTarget();
	void SetGlobalCameraTarget();

public:
	void LightPrevEffect();
	void LightNextEffect();

	void MergePrevEffect();
	void MergeNextEffect();
	void MergeDefferd();

	void CalculateDefferdLight(LightData* _pLightData, KPTR<KRenderTarget> _shadowRenderTargets);

public:
	template<typename T>
	KPTR<T> CreateEffect()
	{
		KPTR<T> newObject = KObjectManager::CreateObject<T>();
		newObject->m_pCamera = KPTR<KCamera>(this);
		newObject->Init();
		return newObject;
	}
};

