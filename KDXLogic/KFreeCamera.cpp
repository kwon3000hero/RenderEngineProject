#include "KFreeCamera.h"
#include <KGameWindow.h>

KFreeCamera::KFreeCamera(KPTR< KCamera> _camera) : m_translateSpeed(1.0f), m_rotateSpeed(180.0f), m_ZoomSpeed(1.0F), m_Camera(_camera)
{
}

KFreeCamera::KFreeCamera(KPTR< KCamera> _camera, float _Value) : m_translateSpeed(_Value), m_Camera(_camera)
{
}

void KFreeCamera::FollowTransform(KPTR<KTransform> _FollowTransform, KVector _FollowPosition, KVector _FollowRotation)
{
	KWeakPTR<KTransform> transform(Actor()->GetComponent<KTransform>());

	m_FollowTransform = _FollowTransform;
	transform->Parent(m_FollowTransform);

	transform->LocalPosition(KVector::ZERO);
	transform->WorldPosition(KVector::ZERO);

	m_FollowRotation = _FollowRotation;
	transform->LocalRotate(m_FollowRotation);

	m_FollowPosition = _FollowPosition;
	transform->LocalPosition(m_FollowPosition);
}

void KFreeCamera::Init()
{
	if (false == KGAMEINPUT::IsExistKey(L"CAML"))
	{
		KGAMEINPUT::CreateKey(L"CAML", 'A');
		KGAMEINPUT::CreateKey(L"CAMR", 'D');
		KGAMEINPUT::CreateKey(L"CAMF", 'W');
		KGAMEINPUT::CreateKey(L"CAMB", 'S');
		KGAMEINPUT::CreateKey(L"ZOOMIN", 'T');
		KGAMEINPUT::CreateKey(L"ZOOMOUT", 'Y');
		KGAMEINPUT::CreateKey(L"ZOOMORI", 'U');
		KGAMEINPUT::CreateKey(L"CAMROTRESET", 'G');

		KGAMEINPUT::CreateKey(L"MODECHANGE", 'F');
		KGAMEINPUT::CreateKey(L"CAMROT", VK_RBUTTON);
		KGAMEINPUT::CreateKey(L"CAMBOOST", VK_LSHIFT);
	}
}

void KFreeCamera::Update()
{
	if (nullptr == m_FollowTransform.get())
	{
		FreeUpdate();
	}
	else
	{
		FollowUpdate();
	}
}

void KFreeCamera::FreeUpdate()
{
	float CurSpeed = m_translateSpeed;
	float CurRSpeed = m_rotateSpeed;

	//if (true == KGAMEINPUT::IsPress(L"CAMBOOST")) {
	//	CurSpeed = Speed * 10.0f;
	//	// CurRSpeed = RSpeed * 10.0f;
	//}

	KPTR<KTransform> transform = Actor()->GetComponent<KTransform>();

	if (true == KGAMEINPUT::IsPress(L"CAML"))
	{
		transform->LMOVE(transform->WLEFT() * KGAMETIME::DeltaTime(CurSpeed));
	}

	if (true == KGAMEINPUT::IsPress(L"CAMR"))
	{
		transform->LMOVE(transform->WRIGHT() * KGAMETIME::DeltaTime(CurSpeed));
	}

	if (true == KGAMEINPUT::IsPress(L"CAMF"))
	{
		if (KCamera::CAMMODE::ORTH == m_Camera->Mode())
		{
			transform->LMOVE(transform->WUP() * KGAMETIME::DeltaTime(CurSpeed));
		}
		else
		{
			transform->LMOVE(transform->WFORWARD() * KGAMETIME::DeltaTime(CurSpeed));
		}
	}
	if (true == KGAMEINPUT::IsPress(L"CAMB")) {
		if (KCamera::CAMMODE::ORTH == m_Camera->Mode())
		{
			transform->LMOVE(transform->WDOWN() * KGAMETIME::DeltaTime(CurSpeed));
		}
		else {
			transform->LMOVE(transform->WBACK() * KGAMETIME::DeltaTime(CurSpeed));
		}
	}

	//if (true == KGAMEINPUT::IsPress(L"ZOOMIN")) { m_Cam->ZoomIn(KGAMETIME::DeltaTime(ZOOMSpeed)); }// m_Cam->ZoomIn(KGAMETIME::DeltaTime(ZOOMSpeed)); }
	//if (true == KGAMEINPUT::IsPress(L"ZOOMOUT")) { m_Cam->ZoomOut(KGAMETIME::DeltaTime(ZOOMSpeed)); }
	//if (true == KGAMEINPUT::IsPress(L"ZOOMORI")) { m_Cam->ZoomOri(); }
	if (true == KGAMEINPUT::IsPress(L"CAMROTRESET")) { transform->WorldRotate({ 0.0F, 0.0F, 0.0F }); }


	//if (true == KGAMEINPUT::IsDown(L"MODECHANGE"))
	//{ 
	//	if (HCAM::CAMMODE::ORTH == m_Cam->Mode())
	//	{
	//		m_Cam->Mode(HCAM::CAMMODE::PROJ);
	//	}
	//	else 
	//	{
	//		Trans()->WROT({ 0.0F, 0.0F, 0.0F });
	//		m_Cam->Mode(HCAM::CAMMODE::ORTH);
	//	}
	//}

	if (true == KGAMEINPUT::IsPress(L"CAMROT"))
	{
		float y = KGAMEINPUT::MouseDir().x * KGAMETIME::DeltaTime(CurRSpeed);
		transform->WROTADDY(y);

		float x = -KGAMEINPUT::MouseDir().y * KGAMETIME::DeltaTime(CurRSpeed);
		transform->WROTADDX(x);
	}

}

void KFreeCamera::FollowUpdate()
{
	if (nullptr == m_FollowTransform.get())
	{
		AssertMsg(L"NULLPTR인 대상을 쫓을수는 없습니다.");
	}
}
