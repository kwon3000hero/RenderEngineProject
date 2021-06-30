#include "TitleCom.h"
#include "KTransform.h"
#include <KFreeObject.h>
#include <KRender.h>
#include <KCamera.h>
#include <KFreeCam.h>
#include <KCuttingTexture.h>
#include <KCuttingRender.h>
#include <KFrameAnimator.h>
#include <KTitleObject.h>
#include <KLogicValue.h>
#include <KCollision.h>
#include <KThread.h>
#include <KGAMEDIR.h>
#include <KFBX.h>

bool bIsLoadFbx = false;
KGameString Str = L"Monster3.fbx";
KGameString UserStr = L"Monster3.userFbx";

void UserMeshLoading(KThread* PTR)
{
	KGAMEDIR Dir;
	Dir.MoveParent(L"KCM3D");
	Dir.Move(L"RES");
	Dir.Move(L"Mesh");
	


	if (true == KGAMEPATH::IsExist(Dir.FilePath(UserStr)))
	{
		KFBX::LoadFromUserFormat(Dir.FilePath(Str), Dir.FilePath(UserStr));
	}
	else
	{
		KFBX::Load(Dir.FilePath(Str));
		bIsLoadFbx = true;
	}

	KGameDebug3D::ConsoleText(UserStr + L" Load End");
}

void TitleCom::Init()
{
	KPTR<KThread> Thread = KThread::Find(L"USERMESHLOADING");
	if (nullptr != Thread)
	{
		Thread->Start(UserMeshLoading);
		return;
	}
	Thread = KThread::Create(L"USERMESHLOADING");
	Thread->SetData(nullptr);
	Thread->Start(UserMeshLoading);

	//UserMeshLoading(nullptr);

}

void TitleCom::PrevUpdate()
{
	if (true == bIsLoadFbx)
	{
		KPTR<KFBX> FBX = KFBX::Find(Str);

		if (nullptr == FBX)
		{
			return;
		}

		FBX->Create(Str);

		KGAMEDIR Dir;
		Dir.MoveParent(L"KCM3D");
		Dir.Move(L"RES");
		Dir.Move(L"Mesh");

		KGameDebug3D::ConsoleText(L"매쉬 완료");
		//FBX->Create(Str);
		FBX->SaveUserFormat(Dir.FilePath(UserStr));
		//KFBX::LoadFromUserFormat(Dir.FilePath(Str), Dir.FilePath(UserStr));

		bIsLoadFbx = false;

		//KGameScene::ChangeScene(L"Test");
	}
}