#pragma once
#include <KGameComponent.h>

#pragma comment(lib, "KDXLogic.lib")

class K3DPlayer;
class KGameActor;
//class KRenderManager;
//class KCUTTINGRENDER;
class KHeightTerrain;

class TestComponent :
	public KGameComponent
{

	KPTR<KRenderManager> SkyRender;


	float Angle;


	KPTR<KGameActor> MAINPLAYER;
	KPTR<KHeightTerrain> HMAP;
	KPTR<KGameActor> NEWACTOR;
	KPTR<K3DPlayer> PLAYERCOM;
	KPTR<KGameActor> BOX;
	KPTR<KRenderManager> BOXRENDER;
	KPTR<KLightManager> TESTLIGHT;


public:
	virtual void Init();
	void NextUpdate() override;
	void PrevUpdate() override;

public:
	TestComponent() {}
	virtual ~TestComponent() {}
};

