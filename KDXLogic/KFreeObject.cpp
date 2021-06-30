#include "KFreeObject.h"

KFreeObject::KFreeObject() : Speed(1.0f)
{
}

void KFreeObject::StartData(float _Value)
{
	Speed = _Value;
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
	if (true == KGAMEINPUT::IsPress(L"L")) { Actor()->Transform()->LMOVE(KVector::LEFT * KGAMETIME::DeltaTime(Speed)); }
	if (true == KGAMEINPUT::IsPress(L"R")) { Actor()->Transform()->LMOVE(KVector::RIGHT * KGAMETIME::DeltaTime(Speed)); }
	if (true == KGAMEINPUT::IsPress(L"U")) { Actor()->Transform()->LMOVE(KVector::UP * KGAMETIME::DeltaTime(Speed)); }
	if (true == KGAMEINPUT::IsPress(L"D")) { Actor()->Transform()->LMOVE(KVector::DOWN * KGAMETIME::DeltaTime(Speed)); }
	if (true == KGAMEINPUT::IsPress(L"F")) { Actor()->Transform()->LMOVE(KVector::FORWARD * KGAMETIME::DeltaTime(Speed)); }
	if (true == KGAMEINPUT::IsPress(L"B")) { Actor()->Transform()->LMOVE(KVector::BACK * KGAMETIME::DeltaTime(Speed)); }
}