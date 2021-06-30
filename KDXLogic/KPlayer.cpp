#include "KPlayer.h"
#include <KCuttingRender.h>
#include <KFrameAnimator.h>
#include "KLogicMath.h"
#include "KLogicValue.h"
#include <KCollision.h>
// KLogicValue::MainCam(PCAM);

void KPlayer::Init()
{
	/*MoveSpeed = 5.0f;

	if (false == KGAMEINPUT::IsExistKey("Move"))
	{
		KGAMEINPUT::CreateKey("Move", VK_LBUTTON);
	}


	KPTR<KCuttingRender> PRENDER = Actor()->CreateCom<KCuttingRender>(L"DIA1.png");
	PRENDER->CutIndex(3);

	ANIPTR = Actor()->CreateCom<KFrameAnimator>();

	for (int i = 0; i < 8; i++)
	{
		wchar_t Arr[10];

		_itow_s(i, Arr, 10);

		KGAMESTRING Str = Arr;
		Str += L"RUN";

		ANIPTR->CreateAni(L"DIA1.png", Str, 7 + (22 * i), 14 + (22 * i), 0.1f);
	}
	ANIPTR->ChangeAni(L"0RUN");


	PRENDER->RatioSetting(RATIOOP::RO_H);
	PRENDER->LineOn();
	PRENDER->LineColor(KVECTOR::WHITE);

	KGAMESTRING Str = KGAMESTRING(std::to_string(KLogicMath::DirAniIndex(KGAMEINPUT::MousePos())).c_str());
	Str += L"RUN";
	ANIPTR->ChangeAni(Str);

	{
		KPTR<KCuttingRender> PRENDER2 = Actor()->CreateCom<KCuttingRender>(L"DIA1.png");
		PRENDER2->LPOS({ 0.0f, 0.5f, -0.5F });
	}

	KPTR<KCol> Col = Actor()->CreateCom<KCol>(KCOLTYPE::OBB2D, 0);

	Col->ExitFunc(&KPlayer::PlayerColFunc, this);
	Col->ExitFunc(&KPlayer::PlayerColFunc2, this);*/

}

void KPlayer::Update()
{
	/*KGameDebug3D::DebugRect(Trans());

	KGAMESTRING Str = KGAMESTRING(std::to_string(KLogicMath::DirAniIndex(KGAMEINPUT::MousePos())).c_str());
	Str += L"RUN";
	if (Str != ANIPTR->CurAniName())
	{
		ANIPTR->ChangeAni(Str);
	}

	if (true == KGAMEINPUT::IsPress("Move"))
	{
		Trans()->WMOVE(KGAMEINPUT::MousePosNormal() * KGAMETIME::DeltaTime(MoveSpeed));
		KLogicValue::MainCamTrans()->WPOS(Trans()->WPOS() + KVECTOR::BACK * 20.0F);
	}*/

}

void KPlayer::PlayerColFunc(KCollision* _this, KCollision* _Other) 
{

}

void KPlayer::PlayerColFunc2(KCollision* _this, KCollision* _Other) 
{
	_Other->Actor()->Death();
}