#pragma once
#include "KGameResource.h"
#include "KGameComponentManager.h"

#include <set>
#include <list>
#include <map>
#include <unordered_map>

class KCollision;
class KCamera;
class KTransform;
class KRenderManager;
class KRenderer;
class KRenderTarget;
class KGameActor;
class KLightManager;
struct LightDataBuffer;

class KGameScene : public KGameComponentManager, public KGameResource<KGameScene>
{
public:
	friend KTransform;
	friend KRenderer;

private:
	static KPTR<KGameScene> m_CurrentScene;
	static KPTR<KGameScene> m_NextScene;

public:
	static KPTR<KGameScene> CurrentScene();
	void SetMainCam(KPTR<KCamera> _cam);
	static KPTR<KCamera> MainCam();
	static void ChangeScene(const KGameString& _Name);
	static void Progress();

private:
	KWeakPTR<KCamera> m_MainCam;
	std::list<KPTR<KGameActor>> m_ActorList;


public:
	static KPTR<KGameScene> Create(const KGameString& _Name = L"")
	{
		KPTR<KGameScene> NewT = KGameResourceManager<KGameScene>::Instance().Create(_Name);
		return NewT;
	}

	template<typename T>
	static KPTR<KGameScene> Create(const KGameString& _Name = L"")
	{
		KPTR<KGameScene> NewT = KGameResourceManager<KGameScene>::Instance().Create(_Name);
		NewT->CreateComponent<T>();
		return NewT;
	}

public:
	KPTR<KGameActor> CreateActor();

public:
	void PrevUpdate() override;
	void Update() override;
	void NextUpdate() override;
	void UpdateTransform();
	void PrevRender() override;
	void Render();
	void Collision();
	void Release();

public:
	void ResetLight();

private:
	std::list<KPTR<KTransform>> m_transformContainer;

	void PushTransform(KPTR<KTransform> _Trans)
	{
		m_transformContainer.push_back(_Trans);
	}



private:
	friend KRenderManager;
	friend KCamera;
	friend KLightManager;

	std::map<int, std::list<KPTR<KCamera>>> m_cameraGroupContainer;
	std::map<int, std::list<KPTR<KRenderManager>>> m_RenderManagerGroupContainer;

	std::map<int, std::list<KPTR<KLightManager>>> m_LightManagerContainer;
	std::map<int, LightDataBuffer*> m_LightDataBufferContainer;

	std::map<int, std::vector<KPTR<KRenderTarget>>> m_ShadowRenderTargetContainer;

private:
	static bool LightSort(KPTR<KLightManager>& _Left, KPTR<KLightManager>& _Right);
	static bool ZSORT(KPTR<KRenderManager>& _Left, KPTR<KRenderManager>& _Right);

	void PushRenderManager(KPTR<KRenderManager> _Render);
	void PushCamera(KPTR<KCamera> _Render);
	void PushLightManager(KPTR<KLightManager> _pLight);

private:
	class KColGroup : public KGameReference
	{
	public:
		int Order;
		std::set<int> m_OtherCol;
		std::list<KPTR<KCollision>> m_AllCol;
	};

	std::unordered_map<int, KPTR<KColGroup>> m_collisionContainer;

public:
	void LinkCollision(int Left, int Right);
	void PushCol(KPTR<KCollision> _Render);

public:
	KGameScene();
	~KGameScene();
};