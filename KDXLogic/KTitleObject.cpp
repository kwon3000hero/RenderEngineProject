#include "KTitleObject.h"
#include <KCuttingRender.h>
#include "KLogicMath.h"
#include "KLogicValue.h"

KPTR<KCuttingRender> pRender = nullptr;

KTitleObject::KTitleObject() :m_progress(0.0f)
{

}

KTitleObject::~KTitleObject()
{

}

void KTitleObject::Init()
{
	pRender = Actor()->CreateComponent<KCuttingRender>(L"Title.png");
	pRender->CutIndex(0);
	pRender->RatioSetting(RATIOOP::RO_W);
}

void KTitleObject::Update()
{
	pRender->SetFadeProgress(m_progress);
}