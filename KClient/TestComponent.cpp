#include "TestComponent.h"
#include <KGAMEDIR.h>
#include <KFBX.h>
#include <KGameInput.h>
#include <KFreeCamera.h>
#include <KBloomEffect.h>
#include <KLogicValue.h>
#include <KHeightTerrain.h>
#include <KLight.h>
#include <K3DPlayer.h>
#include <KMeshAnimator.h>

void TestComponent::Init()
{
	KGAMEDIR Dir;
	Dir.MoveParent(L"RenderEngineProject");
	Dir.Move(L"RES");
	Dir.Move(L"Mesh");
	KFBX::LoadFromUserFormat(Dir.FilePath(L"Monster3.FBX"), Dir.FilePath(L"Monster3.UserFbx"));

	{
		KGAMEDIR Dir;
		Dir.MoveParent(L"RenderEngineProject");
		Dir.Move(L"RES");
		Dir.Move(L"TEX");

		std::list<KGameFile> FileList = Dir.DirAllFile();

		for (auto& File : FileList)
		{
			KTexture::Load(File.m_Path);
		}
	}


	KGameDebug::ConsoleOn();

	if (false == KGAMEINPUT::IsExistKey(L"FLOORCLICK"))
	{
		KGAMEINPUT::CreateKey(L"TEST0", '0');
		KGAMEINPUT::CreateKey(L"TEST1", '1');
		KGAMEINPUT::CreateKey(L"TEST2", '2');
		KGAMEINPUT::CreateKey(L"TEST3", '3');
		KGAMEINPUT::CreateKey(L"TEST4", '4');
		KGAMEINPUT::CreateKey(L"TEST5", '5');
		KGAMEINPUT::CreateKey(L"TEST6", '6');
		KGAMEINPUT::CreateKey(L"TEST7", '7');
		KGAMEINPUT::CreateKey(L"TEST8", '8');
		KGAMEINPUT::CreateKey(L"TEST9", '9');
	}

	Scene()->LinkCollision(0, 1);


	KPTR<KGameActor> cameraActor = Scene()->CreateActor();
	KPTR<KCamera> PCAM = cameraActor->CreateComponent<KCamera>(0, 0);
	KPTR<KFreeCamera> FCAM = cameraActor->CreateComponent<KFreeCamera>(PCAM, 100.0f);
	
	{
		KPTR<KGameActor> NEWACTOR = Scene()->CreateActor();
		NEWACTOR->Transform()->WorldPosition({ 0.0f, 0.0f, -20.0f });

		PCAM->SetSize({ 12.8f, 7.2f });
		PCAM->SetMode(KCamera::CAMMODE::PROJ);
		PCAM->CreateEffect<KBloomEffect>();
		{
			KPTR<KGameActor> SKY = Scene()->CreateActor();
			SKY->Transform()->WorldPosition({ 0.0f, 0.0f, 0.0f });
			SKY->Transform()->WorldScale({ 10000.0F, 10000.F, 10000.f });
			SKY->Transform()->WorldRotate({ 0.0f, 0.0f, 0.0f });

			SkyRender = SKY->CreateComponent<KRenderManager>(L"SPHERE3D", L"3DSKY", 0);
			SkyRender->SetTexture(L"DifTex", L"SkyBox.jpg");
			SkyRender->SetSampler(L"Smp", L"LWSMP");
		}

		{
			KPTR<KGameActor> GRID = Scene()->CreateActor();
			GRID->Transform()->WorldPosition({ 0.0f, 0.0f, 0.0f });
			GRID->Transform()->WorldScale({ 10000.0F, 1.F, 10000.f });
			GRID->Transform()->WorldRotate({ 0.0f, 0.0f, 0.0f });

			SkyRender = GRID->CreateComponent<KRenderManager>(L"GRIDRECT", L"GRID", 0);
			SkyRender->SetConstantBuffer(L"CAMPOSMAT", NEWACTOR->Transform()->CWorldPosition(), ConstantBufferMode::Link);
		}
	}



	{
		KPTR<KGameActor> NEWACTOR = Scene()->CreateActor();
		NEWACTOR->Transform()->WorldScale({ 1.0f, 1.0f, 5.0f });

		KVector Foward = KVector::FORWARD;
		KVector Rot = { 45.0f, 0.0f, 0.0f };
		NEWACTOR->Transform()->WorldRotate(Rot);
		Foward.RotationToDegree(Rot);
		Foward *= -100.0f;

		NEWACTOR->Transform()->WorldPosition(Foward);

		KPTR<KRenderManager> Render = NEWACTOR->CreateComponent<KRenderManager>(L"BOX", L"COLOR", 0);

		TESTLIGHT = NEWACTOR->CreateComponent<KLightManager>(0, 0);
	}

	{
		KPTR<KGameActor> NEWACTOR = Scene()->CreateActor();
		NEWACTOR->Transform()->WorldScale({ 1.0f, 1.0f, 5.0f });

		KVector Foward = KVector::FORWARD;
		KVector Rot = { 135.f, 0.0f, 0.0f };
		NEWACTOR->Transform()->WorldRotate(Rot);
		Foward.RotationToDegree(Rot);
		Foward *= -100.0f;

		NEWACTOR->Transform()->WorldPosition(Foward);

		KPTR<KRenderManager> Render = NEWACTOR->CreateComponent<KRenderManager>(L"BOX", L"COLOR", 0);		

		TESTLIGHT = NEWACTOR->CreateComponent<KLightManager>(0, 0);
	}


	{
		KPTR<KGameActor> NEWACTOR(Scene()->CreateActor());
		NEWACTOR->Transform()->WorldScale({ 2.0f, 2.0f, 2.0f });
		NEWACTOR->Transform()->WorldPosition({ 3.0f, 3.0f, 3.0f });

		KPTR<KRenderManager> Render = NEWACTOR->CreateComponent<KRenderManager>(L"SPHERE3D", L"3DCOLORDEFFERD", 0);
		Render->ShadowOn();
	}

	{
		BOX = Scene()->CreateActor();
		BOX->Transform()->WorldScale({ 0.5, 10, 0.5 });
		BOXRENDER = BOX->CreateComponent<KRenderManager>(L"BOX", L"COLOR", 0);
	}


	{
		KPTR<KGameActor> ACTOR = Scene()->CreateActor();
		MAINPLAYER = ACTOR;
		KPTR<KGameActor> PIVOT = Scene()->CreateActor();
		PIVOT->Transform()->LocalScale({ 0.1f, 0.1f , 0.1f });
		PIVOT->Transform()->LocalRotate({ 0.0f, -90.0f , 0.0f });
		PIVOT->Transform()->Parent(ACTOR->Transform()); // ÇÇº¿ÀÌ 0


		PLAYERCOM = ACTOR->CreateComponent<K3DPlayer>();

		FCAM->FollowTransform(KPTR<KTransform>(ACTOR->Transform()), KVector(0, 10, -8), KVector(30, 0, 0));
	}
	{
		KPTR<KGameActor> ACTOR = Scene()->CreateActor();
		ACTOR->Transform()->LocalPosition({ 0.0f, 0.1f, 0.0f });
		ACTOR->Transform()->LocalScale({ 1000.0f, 100.0f, 1000.0f });
		HMAP = ACTOR->CreateComponent<KHeightTerrain>();
		HMAP->CreateGround(100, 100, L"TILE_01.png", L"TILE02_S.png");
		HMAP->AddFloor(L"BUMPTEST.png", L"TILE02_Sp.png");
	}



	Angle = 60.0f;

}

void TestComponent::NextUpdate()
{
}

void TestComponent::PrevUpdate()
{
	KMeshAnimator::BoneFrameData HANDMAT = PLAYERCOM->MeshAnimator()->BoneFrameDataMatrix(L"Bip01-R-Hand");
	KMatrix Scale;
	Scale.Scale({ 10.0f, 100.0f, 10.0f });
	BOX->Transform()->SetFinal(Scale * HANDMAT.AnimationWorldMatrix);


	//KGameDebug3D::DebugText({ 0,0 }, 20.f, KVector::RED, L"AniPos : %f %f %f", HANDMAT.AnimationPosition.x, HANDMAT.AnimationPosition.y, HANDMAT.AnimationPosition.z);
	//KGameDebug3D::DebugText({ 0,20 }, 20.f, KVector::RED, L"AniRot : %f %f %f", HANDMAT.AnimationEuler.x, HANDMAT.AnimationEuler.y, HANDMAT.AnimationEuler.z);
	//KGameDebug3D::DebugText({ 0,40 }, 20.f, KVector::RED, L"Pos : %f %f %f", HANDMAT.Pos.x, HANDMAT.Pos.y, HANDMAT.Pos.z);
	//KGameDebug3D::DebugText({ 0,60 }, 20.f, KVector::RED, L"Rot : %f %f %f", HANDMAT.Euler.x, HANDMAT.Euler.y, HANDMAT.Euler.z);

	//KMatrix Mat;
	//Mat.Pos(-Scene()->MainCam()->Actor()->Transform()->LocalPosition());

	float X = (MAINPLAYER->Transform()->WorldPosition().x / HMAP->MapData().x);
	float Z = (MAINPLAYER->Transform()->WorldPosition().z / HMAP->MapData().z);


	KVector Pos = MAINPLAYER->Transform()->WorldPosition();
	Pos.y = HMAP->MapY(Pos);
	KGameDebug3D::DebugText({ 0,1000.0f }, 20.f, KVector::RED, L"%f", Pos.y);
	MAINPLAYER->Transform()->WorldPosition(Pos);

	KGameDebug3D::DebugTexture(Scene()->MainCam()->GlobalBufferTarget()->Texture(0), { 1280 - 256, 0 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);
	KGameDebug3D::DebugTexture(Scene()->MainCam()->GlobalBufferTarget()->Texture(1), { 1280 - 256, -144 * 1 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);
	KGameDebug3D::DebugTexture(Scene()->MainCam()->GlobalBufferTarget()->Texture(2), { 1280 - 256, -144 * 2 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);
	KGameDebug3D::DebugTexture(Scene()->MainCam()->GlobalBufferTarget()->Texture(3), { 1280 - 256, -144 * 3 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);

	KGameDebug3D::DebugTexture(Scene()->MainCam()->LightTarget()->Texture(0), { 1280 - 512, 0 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);
	KGameDebug3D::DebugTexture(Scene()->MainCam()->LightTarget()->Texture(1), { 1280 - 512, -144 * 1 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);
	KGameDebug3D::DebugTexture(Scene()->MainCam()->LightTarget()->Texture(2), { 1280 - 512, -144 * 2 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);
	KGameDebug3D::DebugTexture(Scene()->MainCam()->DefferdCameraTarget(), { 1280 - 512, -144 * 3 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);

	KGameDebug3D::DebugText({ 0,140.0f }, 20.f, KVector::BLUE, L"%f %f", KGAMEINPUT::ScreenMousePos().x, KGAMEINPUT::ScreenMousePos().y);

	KGameDebug3D::DebugTexture(TESTLIGHT->ShadowTarget(), { 1280 - 512, -144 * 4 }, KVector{ 128, 72 } *2.0f, KVector::ZERO);
}
