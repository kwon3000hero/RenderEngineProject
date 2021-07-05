#include "K3DPlayer.h"
#include <KGAMEDIR.h>
#include <KRender.h>
#include <KMeshAnimator.h>
#include <KTransform.h>
void K3DPlayer::Init()
{
	KGAMEDIR Dir;
	Dir.MoveParent(L"RenderEngineProject-main");
	Dir.Move(L"RES");
	Dir.Move(L"Mesh");

	KPTR<KTransform> pivot = Actor()->Transform()->GetChild(0);

	KPTR<KRenderManager> currentRenderManager = pivot->Actor()->CreateComponent<KRenderManager>();
	m_MeshAnimator = pivot->Actor()->CreateComponent<KMeshAnimator>();
	std::vector<KPTR<KRenderPlayer>> Rp = m_MeshAnimator->CreateAnimation(Dir, L"Monster3.FBX", L"Take 001", L"Run", 885, 908, L"3DMESHDEFFERD"/*, 2.0f, true*/);

	Rp[3]->m_RenderOption[static_cast<int>(RenderOption::Specular)] = false;
	Rp[3]->SetTexture(L"EmiTex", L"CH_NPC_MOB_Anashar_Weapon_A01_E_KGW.png");

	m_MeshAnimator->CreateAnimation(Dir, L"Monster3.FBX", L"Take 001", L"Att", 85, 130, L"3DMESHDEFFERD");
	m_MeshAnimator->ChangeAnimation(L"Run");

	currentRenderManager->ShadowOn();

	if (false == KGAMEINPUT::IsExistKey(L"PLAYERF"))
	{
		KGAMEINPUT::CreateKey(L"PLAYERF", 'W');
		KGAMEINPUT::CreateKey(L"PLAYERB", 'S');

		KGAMEINPUT::CreateKey(L"PLAYERLEFTROT", 'Q');
		KGAMEINPUT::CreateKey(L"PLAYERRIGHTROT", 'E');

		KGAMEINPUT::CreateKey(L"ATT", VK_LBUTTON);
	}
}

void K3DPlayer::Update()
{
	if (true == KGAMEINPUT::IsDown(L"ATT"))
	{
		m_MeshAnimator->ChangeAnimation(L"Att");
	}

	if (true == KGAMEINPUT::IsPress(L"PLAYERF"))
	{
		Actor()->Transform()->LMOVE(Actor()->Transform()->WFORWARD() * KGAMETIME::DeltaTime(50.0f));
	}

	if (true == KGAMEINPUT::IsDown(L"PLAYERF"))
	{
		m_MeshAnimator->ChangeAnimation(L"Run");
	}

	if (true == KGAMEINPUT::IsPress(L"PLAYERB"))
	{
		Actor()->Transform()->LMOVE(Actor()->Transform()->WBACK() * KGAMETIME::DeltaTime(50.0f));
	}

	if (true == KGAMEINPUT::IsDown(L"PLAYERB"))
	{
		m_MeshAnimator->ChangeAnimation(L"Run");
	}

	if (true == KGAMEINPUT::IsPress(L"PLAYERLEFTROT"))
	{
		Actor()->Transform()->LROTADDY(-100.0F * KGAMETIME::DeltaTime(1.0f));
	}

	if (true == KGAMEINPUT::IsPress(L"PLAYERRIGHTROT"))
	{
		Actor()->Transform()->LROTADDY(100.0F * KGAMETIME::DeltaTime(1.0f));
	}

}
