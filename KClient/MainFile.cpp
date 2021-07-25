#include <Windows.h>
#include <KGameWindow.h>
#include <KGameType.h>
#include <KGAMEDEBUG.h>
#include <KGameReference.h>
#include <KGameDevice.h>
#include <KGAMEDIR.h>
#include <KGAMETIME.h>
#include <KTexture.h>
#include <KCuttingTexture.h>
#include <KSampler.h>
#include <KBlend.h>
#include <KMesh.h>
#include <memory>
#include <KRenderPipeline.h>
#include <KCuttingRender.h>

#include <KGameScene.h>


#include "KVertexHeader.h"
#include "KVertexBuffer.h"
#include "KIndexBuffer.h"
#include "KRasterizer.h"
#include "KDepthStencil.h"
#include "KVertexShader.h"
#include "KPixelShader.h"
#include "KConstantBuffer.h"
#include "KGAMEINPUT.h"
#include <KThread.h>
#include <KFBX.h>
#include <KGameDebug3D.h>

#include "TitleCom.h"
#include "TestComponent.h"

void __stdcall Start()
{
	{
		KGameDevice::Create(L"MainWindow", { 0.0f, 0.0f, 1.0f, 1.0f });

#ifdef _DEBUG
		KGameDebug::ConsoleOn();
#endif

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

	//KGameScene::Create<TitleCom>(L"TITLE");	
	//KGameScene::ChangeScene(L"TITLE");

	KGameScene::Create<TestComponent>(L"TEST");
	KGameScene::ChangeScene(L"TEST");
}



void __stdcall Progress()
{
	KGAMEINPUT::Update();
	KGAMETIME::MainTimeUpdate();
	KGameScene::Progress();
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	KGameDebug::LeakCheck();
	KGameWindow::CreateWin(L"MainWindow");
	KGameWindow::MainWindow()->SetSize({ 1280, 720 });

	KGameWindow::Progress(Progress, Start);

	return 0;
}