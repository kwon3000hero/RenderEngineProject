#include "KGAMESCENE.h"
#include "KGAMEACTOR.h"
#include "KRENDERTARGET.h"
#include "KRENDERPIPELINE.h"
#include "KTransform.h"
#include <assert.h>
#include "KGAMEDEVICE.h"
#include "KRENDER.h"
#include "KCamera.h"
#include "KCollision.h"
#include "KGameDebug3D.h"
#include "KRenderTarget.h"
#include "KLight.h"

bool KGameScene::LightSort(KPTR<KLightManager>& _Left, KPTR<KLightManager>& _Right)
{
	return _Left->Order() > _Right->Order();
}

bool KGameScene::ZSORT(KPTR<KRenderManager>& _Left, KPTR<KRenderManager>& _Right)
{
	return _Left->WorldPosition().z > _Right->WorldPosition().z;
}


KPTR<KGameScene> KGameScene::m_CurrentScene = static_cast<KPTR<KGameScene>>(0);
KPTR<KGameScene> KGameScene::m_NextScene = static_cast<KPTR<KGameScene>>(0);

void KGameScene::ChangeScene(const KGameString& _Name)
{
	m_NextScene = KGameResourceManager<KGameScene>::Instance().Find(_Name);
	if (nullptr == m_NextScene.get())
	{
		assert(false);
	}
}

void KGameScene::Progress()
{
	if (nullptr != m_NextScene.get())
	{
		if (nullptr == m_CurrentScene.get())
		{
			m_CurrentScene = m_NextScene;
			m_NextScene = static_cast<KPTR<KGameScene>>(0);
		}
		else
		{
			m_CurrentScene = m_NextScene;
			m_NextScene = static_cast<KPTR<KGameScene>>(0);
		}
	}

	if (nullptr == m_CurrentScene.get())
	{
		assert(false);
	}

	m_CurrentScene->PrevUpdate();
	m_CurrentScene->Update();
	m_CurrentScene->NextUpdate();
	m_CurrentScene->UpdateTransform();
	m_CurrentScene->Collision();
	m_CurrentScene->PrevRender();
	m_CurrentScene->Render();
	m_CurrentScene->Release();
}

KPTR<KGameScene> KGameScene::CurrentScene()
{
	return m_CurrentScene;
}

KPTR<KCamera> KGameScene::MainCam()
{
	return m_CurrentScene->m_MainCam;
}

KGameScene::KGameScene()
{
}

KGameScene::~KGameScene()
{
	for (auto lightData : m_LightDataBufferContainer)
	{
		delete lightData.second;
	}
}

KPTR<KGameActor> KGameScene::CreateActor()
{
	KPTR<KGameActor> NewActor = make_KPTR<KGameActor>();
	NewActor->SetScene(KPTR<KGameScene>(this));
	NewActor->CreateComponent<KTransform>();
	m_ActorList.push_back(NewActor);
	return NewActor;
}

void KGameScene::PrevUpdate()
{
	KGameComponentManager::PrevUpdate();
	for (auto& Actor : m_ActorList)
	{
		if (false == Actor->IsUpdate())
		{
			continue;
		}
		Actor->PrevUpdate();
	}
}
void KGameScene::Update()
{
	KGameComponentManager::Update();
	for (auto& Actor : m_ActorList)
	{
		if (false == Actor->IsUpdate())
		{
			continue;
		}
		Actor->Update();
	}
}
void KGameScene::NextUpdate()
{
	KGameComponentManager::NextUpdate();
	for (auto& Actor : m_ActorList)
	{
		if (false == Actor->IsUpdate())
		{
			continue;
		}
		Actor->NextUpdate();
	}
}

void KGameScene::UpdateTransform()
{
	for (auto& trans : m_transformContainer)
	{
		trans->UpdateTransform();
	}

	for (auto& camGroup : m_cameraGroupContainer)
	{
		for (auto& cam : camGroup.second)
		{
			cam->UpdateViewAndProjection();
		}
	}
}

void KGameScene::PrevRender()
{
	KGameComponentManager::PrevRender();
	for (auto& actor : m_ActorList)
	{
		if (false == actor->IsUpdate())
		{
			continue;
		}
		actor->PrevRender();
	}
}

void KGameScene::Release()
{
	for (auto& ColGroup : m_collisionContainer)
	{
		std::list<KPTR<KCollision>> ::iterator StartIter = ColGroup.second->m_AllCol.begin();;
		std::list<KPTR<KCollision>> ::iterator EndIter = ColGroup.second->m_AllCol.end();
		for (; StartIter != EndIter;)
		{
			if (false == (*StartIter)->IsDeath())
			{
				++StartIter;
				continue;
			}

			StartIter = ColGroup.second->m_AllCol.erase(StartIter);
		}
	}

	for (auto& RenderGroup : m_RenderManagerGroupContainer)
	{
		std::list<KPTR<KRenderManager>> ::iterator StartIter = RenderGroup.second.begin();
		std::list<KPTR<KRenderManager>> ::iterator EndIter = RenderGroup.second.end();
		for (; StartIter != EndIter;)
		{
			if (false == (*StartIter)->IsDeath())
			{
				++StartIter;
				continue;
			}

			StartIter = RenderGroup.second.erase(StartIter);
		}
	}

	std::list<KPTR<KGameActor>>::iterator StartIter = m_ActorList.begin();
	std::list<KPTR<KGameActor>>::iterator EndIter = m_ActorList.end();

	for (; StartIter != EndIter;)
	{
		if (false == (*StartIter)->IsDeath())
		{
			++StartIter;
			continue;
		}

		StartIter = m_ActorList.erase(StartIter);
	}
}

void KGameScene::ResetLight()
{
	for (auto Var : m_LightDataBufferContainer)
	{
		Var.second->LightCount[0] = 0;
	}
	for (auto& LightGroup : m_LightManagerContainer)
	{
		m_ShadowRenderTargetContainer[LightGroup.first].clear();

		//LightGroup.second.sort(LightSort);
		for (auto& Light : LightGroup.second)
		{
			m_LightDataBufferContainer[LightGroup.first]->LightDataArray[m_LightDataBufferContainer[LightGroup.first]->LightCount[0]] = Light->m_LightData;
			m_ShadowRenderTargetContainer[LightGroup.first].push_back(Light->ShadowTarget());
			m_LightDataBufferContainer[LightGroup.first]->LightCount[0] += 1;
		}
	}
}

void KGameScene::Render()
{
	KRenderPipeline::Reset();

	KGameDevice::SetMainRenderTarget();

	ResetLight();

	for (auto& camGroup : m_cameraGroupContainer)
	{
		for (auto& cam : camGroup.second)
		{
			cam->SetForwardCameraTarget();

			for (int RenderGroupOrder : cam->m_ViewList)
			{
				std::map<int, std::list<KPTR<KRenderManager>>>::iterator renderGroupIter = m_RenderManagerGroupContainer.find(RenderGroupOrder);

				if (m_RenderManagerGroupContainer.end() == renderGroupIter)
				{
					continue;
				}

				for (int i = 0; i < m_LightDataBufferContainer[RenderGroupOrder]->LightCount[0]; ++i)
				{
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition = cam->Actor()->Transform()->WorldPosition();
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition = cam->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition, 1.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir = cam->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir, 0.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir.Normalize();
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv = cam->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv, 0.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv.Normalize();
				}

				if (0 >= renderGroupIter->second.size())
				{
					continue;
				}

				for (auto& Renderer : renderGroupIter->second)
				{
					if (false == Renderer->IsUpdate())
					{
						continue;
					}

					Renderer->ForwardRender(cam);
				}
			}
		}

		for (auto Var : m_LightDataBufferContainer)
		{
			Var.second->LightCount[0] = 0;
		}
		for (auto& LightGroup : m_LightManagerContainer)
		{
			for (auto& Light : LightGroup.second)
			{
				m_LightDataBufferContainer[LightGroup.first]->LightDataArray[m_LightDataBufferContainer[LightGroup.first]->LightCount[0]] = Light->m_LightData;
				m_LightDataBufferContainer[LightGroup.first]->LightCount[0] += 1;
			}
		}

		ResetLight();
		for (auto& camera : camGroup.second)
		{
			camera->SetGlobalCameraTarget();
			for (int RenderGroupOrder : camera->m_ViewList)
			{
				std::map<int, std::list<KPTR<KRenderManager>>>::iterator renderGroupIter = m_RenderManagerGroupContainer.find(RenderGroupOrder);

				if (m_RenderManagerGroupContainer.end() == renderGroupIter)
				{
					continue;
				}

				if (0 >= renderGroupIter->second.size())
				{
					continue;
				}

				for (int i = 0; i < m_LightDataBufferContainer[RenderGroupOrder]->LightCount[0]; ++i)
				{
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition = camera->Actor()->Transform()->WorldPosition();
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition = camera->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition, 1.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir = camera->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir, 0.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir.Normalize();
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv = camera->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv, 0.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv.Normalize();
				}

				for (auto& Renderer : renderGroupIter->second)
				{
					if (false == Renderer->IsUpdate())
					{
						continue;
					}

					Renderer->DefferdRender(camera);
				}
			}
		}
	}

	for (auto& lightGroup : m_LightManagerContainer)
	{
		for (auto& light : lightGroup.second)
		{
			light->SetShadowTarget();

			std::map<int, std::list<KPTR<KRenderManager>>>::iterator renderGroupIter = m_RenderManagerGroupContainer.find(lightGroup.first);
			for (auto& renderer : renderGroupIter->second)
			{
				renderer->ShadowRender(light);
			}
		}
	}

	for (auto& cameraGroup : m_cameraGroupContainer)
	{
		for (auto& camera : cameraGroup.second)
		{
			camera->LightTarget()->Clear();
		}
	}

	KRenderPipeline::Reset();

	ResetLight();

	for (auto& cameraGroup : m_cameraGroupContainer)
	{
		for (auto& camera : cameraGroup.second)
		{
			camera->LightPrevEffect();
			for (int RenderGroupOrder : camera->m_ViewList)
			{
				std::map<int, std::list<KPTR<KRenderManager>>>::iterator renderGroupIter = m_RenderManagerGroupContainer.find(RenderGroupOrder);

				if (m_RenderManagerGroupContainer.end() == renderGroupIter)
				{
					continue;
				}

				if (0 >= renderGroupIter->second.size())
				{
					continue;
				}

				for (int i = 0; i < (m_LightDataBufferContainer[RenderGroupOrder]->LightCount[0]); i++)
				{
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition = camera->Actor()->Transform()->WorldPosition();
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition = camera->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CameraPosition, 1.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir = camera->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir, 0.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDir.Normalize();
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv = camera->m_ViewMat.Multi(m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv, 0.0f);
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].LightDirInv.Normalize();
					m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CamInversView = camera->View().Inverse();
					//m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CamInversView = camera->View();
					//m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i].CamInversView.Inverse();

					camera->CalculateDefferdLight(&m_LightDataBufferContainer[RenderGroupOrder]->LightDataArray[i], m_ShadowRenderTargetContainer[RenderGroupOrder][i]);
				}

			}
			camera->LightNextEffect();
		}
	}

	for (auto& cameraGroup : m_cameraGroupContainer)
	{
		for (auto& camera : cameraGroup.second)
		{
			camera->MergeDefferd();
		}
	}

	for (auto& cameraGroup : m_cameraGroupContainer)
	{
		for (auto& camera : cameraGroup.second)
		{
			camera->MergePrevEffect();
			camera->MergeTargets();
			camera->MergeNextEffect();
		}
	}

	KRenderPipeline::ResetAll();
	KGameDevice::SetMainRenderTarget();

	for (auto& cameraGroup : m_cameraGroupContainer)
	{
		for (auto& camera : cameraGroup.second)
		{
			KGameDevice::MainGameDevice()->MainTarget->AlwaysMerge(camera->CurrentCameraTarget());
		}
	}

	KGameComponentManager::DebugRender();
	for (auto& Actor : m_ActorList)
	{
		if (false == Actor->IsUpdate())
		{
			continue;
		}
		Actor->DebugRender();
	}

	KGameDebug3D::DebugRender();
	KRenderPipeline::ResetAll();
	KGameDevice::MainGameDevice()->DeviceRenderEnd();
}

void KGameScene::PushCamera(KPTR<KCamera> _pCam)
{
	if (m_cameraGroupContainer.end() == m_cameraGroupContainer.find(_pCam->Order()))
	{
		m_cameraGroupContainer[_pCam->Order()] = std::list<KPTR<KCamera>>();
	}

	m_cameraGroupContainer[_pCam->Order()].push_back(_pCam);

	if (nullptr == m_MainCam)
	{
		m_MainCam = _pCam;
	}
}

void KGameScene::PushLightManager(KPTR<KLightManager> _pLight)
{
	for (auto& Var : _pLight->m_ViewGroup)
	{
		if (m_RenderManagerGroupContainer.end() == m_RenderManagerGroupContainer.find(Var))
		{
			m_RenderManagerGroupContainer[Var] = std::list<KPTR<KRenderManager>>();
			m_LightDataBufferContainer[Var] = new LightDataBuffer();
		}

		if (m_LightManagerContainer.end() == m_LightManagerContainer.find(Var))
		{
			m_LightManagerContainer[Var] = std::list<KPTR<KLightManager>>();
		}
	}

	for (auto& Var : _pLight->m_ViewGroup)
	{
		m_LightManagerContainer[Var].push_back(_pLight);
	}
}

void KGameScene::PushRenderManager(KPTR<KRenderManager> _pRenderManager)
{
	if (m_RenderManagerGroupContainer.end() == m_RenderManagerGroupContainer.find(_pRenderManager->Order()))
	{
		m_RenderManagerGroupContainer[_pRenderManager->Order()] = std::list<KPTR<KRenderManager>>();
		m_LightDataBufferContainer[_pRenderManager->Order()] = new LightDataBuffer();
	}

	m_RenderManagerGroupContainer[_pRenderManager->Order()].push_back(_pRenderManager);
}

///////////////////////////////// Col

void KGameScene::PushCol(KPTR<KCollision> _collision)
{
	if (m_collisionContainer.end() == m_collisionContainer.find(_collision->Order()))
	{
		m_collisionContainer[_collision->Order()] = make_KPTR<KColGroup>();
		m_collisionContainer[_collision->Order()]->Order = _collision->Order();
	}

	m_collisionContainer[_collision->Order()]->m_AllCol.push_back(_collision);
}

void KGameScene::LinkCollision(int Left, int Right)
{
	if (m_collisionContainer.end() == m_collisionContainer.find(Left))
	{
		m_collisionContainer[Left] = make_KPTR<KColGroup>();
		m_collisionContainer[Left]->Order = Left;
	}

	if (m_collisionContainer[Left]->m_OtherCol.end() != m_collisionContainer[Left]->m_OtherCol.find(Right))
	{
		AssertMsg(L"이미 링크한 충돌그룹끼리 다시 링크 시킬수 없습니다.");
	}

	m_collisionContainer[Left]->m_OtherCol.insert(Right);
}

void KGameScene::Collision()
{
	for (auto& _ColGroup : m_collisionContainer)
	{
		for (auto& _ColIndex : _ColGroup.second->m_OtherCol)
		{
			std::unordered_map<int, KPTR<KColGroup>>::iterator Other = m_collisionContainer.find(_ColIndex);

			if (Other == m_collisionContainer.end())
			{
				continue;
			}

			if (0 >= Other->second->m_AllCol.size())
			{
				continue;
			}

			for (auto& _ThisCol : _ColGroup.second->m_AllCol)
			{
				for (auto& _OtherCol : Other->second->m_AllCol)
				{
					_ThisCol->ColCheck(_OtherCol);
				}
			}
		}
	}
}