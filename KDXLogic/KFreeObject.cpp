#include "KFreeObject.h"

KFreeObject::KFreeObject() : m_speed(1.0f)
{
}

KFreeObject::KFreeObject(float _Value) : m_speed(_Value)
{
}

void KFreeObject::Init()
{
	if (false == KGAMEINPUT::IsExistKey(L"L"))
	{
		KGAMEINPUT::CreateKey(L"L", VK_NUMPAD4);
		KGAMEINPUT::CreateKey(L"R", VK_NUMPAD6);
		KGAMEINPUT::CreateKey(L"U", VK_NUMPAD8);
		KGAMEINPUT::CreateKey(L"D", VK_NUMPAD5);
		KGAMEINPUT::CreateKey(L"F", VK_NUMPAD7);
		KGAMEINPUT::CreateKey(L"B", VK_NUMPAD9);
	}
}

void KFreeObject::Update()
{
	KWeakPTR<KTransform> transform(Actor()->Transform());

	if (true == KGAMEINPUT::IsPress(L"L")) { transform->LMOVE(KVector::LEFT * KGAMETIME::DeltaTime(m_speed)); }
	if (true == KGAMEINPUT::IsPress(L"R")) { transform->LMOVE(KVector::RIGHT * KGAMETIME::DeltaTime(m_speed)); }
	if (true == KGAMEINPUT::IsPress(L"U")) { transform->LMOVE(KVector::UP * KGAMETIME::DeltaTime(m_speed)); }
	if (true == KGAMEINPUT::IsPress(L"D")) { transform->LMOVE(KVector::DOWN * KGAMETIME::DeltaTime(m_speed)); }
	if (true == KGAMEINPUT::IsPress(L"F")) { transform->LMOVE(KVector::FORWARD * KGAMETIME::DeltaTime(m_speed)); }
	if (true == KGAMEINPUT::IsPress(L"B")) { transform->LMOVE(KVector::BACK * KGAMETIME::DeltaTime(m_speed)); }
}