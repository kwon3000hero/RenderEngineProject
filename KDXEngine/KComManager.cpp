#include "KComManager.h"
#include "KGameComponentManager.h"

void KGameComponentManager::Init()
{
	for (auto& Com : m_componentList)
	{
		Com->Init();
	}
}

void KGameComponentManager::PrevUpdate()
{
	for (auto& Com : m_componentList)
	{
		if (false == Com->IsUpdate()) { continue; }
		Com->PrevUpdate();
	}
}

void KGameComponentManager::Update()
{
	for (auto& Com : m_componentList)
	{
		if (false == Com->IsUpdate()) { continue; }
		Com->Update();
	}
}

void KGameComponentManager::NextUpdate()
{
	for (auto& Com : m_componentList)
	{
		if (false == Com->IsUpdate()) { continue; }
		Com->NextUpdate();
	}
}

void KGameComponentManager::PrevRender()
{
	for (auto& Com : m_componentList)
	{
		if (false == Com->IsUpdate()) { continue; }
		Com->PrevRender();
	}
}

void KGameComponentManager::DebugRender()
{
	for (auto& Com : m_componentList)
	{
		if (false == Com->IsUpdate()) { continue; }
		Com->DebugRender();
	}
}