#pragma once
#include "KGameResource.h"
#include "KComManager.h"

#include <set>
#include <list>
#include <map>
#include <unordered_map>

#include "KGameComponentManager.h"

class KCollision;
class KCamera;
class KTransform;
class KRenderManager;
class KRenderPlayer;
class KRenderTarget;
class KGameActor;
class KLightManager;
struct LightDataBuffer;
class KGameScene : public KGameComponentManager, public KGameResourceBase<KGameScene>
{
public:
	friend KTransform;
	friend KRenderPlayer;

private:
	static KPTR<KGameScene> m_CurrentScene;
	static KPTR<KGameScene> m_NextScene;

public:
	static KPTR<KGameScene> CurrentScene();
	static KCamera* MainCam();
	static void ChangeScene(const KGameString& _Name);
	static void Progress();

private:
	KCamera* m_MainCam;
	std::list<KPTR<KGameActor>> m_ActorList;


public:
	static KPTR<KGameScene> Create(const KGameString& _Name = L"")
	{
		KPTR<KGameScene> NewT = KGameResourceBase<KGameScene>::Create(_Name);
		return NewT;
	}

	template<typename STARTCOM>
	static KPTR<KGameScene> Create(const KGameString& _Name = L"")
	{
		KPTR<KGameScene> NewT = KGameResourceBase<KGameScene>::Create(_Name);
		NewT->CreateComponent<STARTCOM>();
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
	//std::map<int, std::list<KPTR<KRenderManager>>>::iterator renderGroupIter;

	std::map<int, std::list<KPTR<KLightManager>>> m_LightManagerContainer;
	std::map<int, LightDataBuffer*> m_LightDataBufferContainer;

	std::map<int, std::vector<KPTR<KRenderTarget>>> m_ShadowRenderTargetContainer;

private:
	static bool LightSort(KPTR<KLightManager>& _Left, KPTR<KLightManager>& _Right);
	static bool ZSORT(KPTR<KRenderManager>& _Left, KPTR<KRenderManager>& _Right);

	void PushRenderManager(KRenderManager* _Render);
	void PushCamera(KCamera* _Render);
	void PushLightManager(KLightManager* _pLight);

private:
	class KColGroup : public KGameReference
	{
	public:
		int Order;
		std::set<int> m_OtherCol;
		std::list<KPTR<KCollision>> m_AllCol;
	};

	std::unordered_map<int, KPTR<KColGroup>> m_AllCol;

public:
	void LinkCollision(int Left, int Right);
	void PushCol(KCollision* _Render);

public:
	KGameScene();
	~KGameScene();
};