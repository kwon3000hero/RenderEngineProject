#include "KCollision.h"
#include "KGameDebug3D.h"

void KCollision::StartData(int _Order)
{
	SetOrder(_Order);
}
void KCollision::StartData(KCollisionType _Type, int _Order)
{
	SetOrder(_Order);
	m_Type = _Type;
}

void KCollision::Init()
{
	KPTR<KTransform> PTR = Actor()->GetComponent<KTransform>();

	if (nullptr == PTR.get())
	{
		assert(false);
	}

	Parent(PTR);
	Scene()->PushCol(KPTR<KCollision>(this));
}

void KCollision::ColEnter(KPTR<KCollision> _Other)
{
	for (auto& Auto : m_EnterFunc)
	{
		Auto(KPTR<KCollision>(this), _Other);
	}
}
void KCollision::ColStay(KPTR<KCollision> _Other)
{
	for (auto& Auto : m_StayFunc)
	{
		Auto(KPTR<KCollision>(this), _Other);
	}
}
void KCollision::ColExit(KPTR<KCollision> _Other)
{
	for (auto& Auto : m_ExitFunc)
	{
		Auto(KPTR<KCollision>(this), _Other);
	}

}


void KCollision::ColCheck(KPTR<KCollision> _OtherCol)
{
	if (true == Col(m_Type, _OtherCol->m_Type, static_cast<KPTR<KTransform>>(_OtherCol.get())))
	{
		if (m_OtherCol.end() == m_OtherCol.find(_OtherCol.get()))
		{
			// 충돌후
			_OtherCol->m_OtherCol.insert(this);
			m_OtherCol.insert(_OtherCol.get());

			ColEnter(_OtherCol);
			_OtherCol->ColEnter(KPTR<KCollision>(this));
		}
		else
		{
			ColStay(_OtherCol);
			_OtherCol->ColStay(KPTR<KCollision>(this));
		}
	}
	else
	{
		// 충돌중
		if (m_OtherCol.end() != m_OtherCol.find(_OtherCol.get()))
		{
			ColExit(_OtherCol);
			_OtherCol->ColExit(KPTR<KCollision>(this));

			m_OtherCol.erase(_OtherCol.get());
			_OtherCol->m_OtherCol.erase(this);
			// 충돌 끝
		}
	}
}


void KCollision::DebugRender()
{
	SetCameraMatrix(KGameScene::MainCam());

	switch (m_Type)
	{
	case KCollisionType::OBB2D:
		KGameDebug3D::DebugRect(KPTR<KTransform>(this));
		break;
	case KCollisionType::ABB2D:
		break;
	case KCollisionType::SPHERE2D:
		break;
	case KCollisionType::POINT2D:
		break;
	case KCollisionType::OBB3D:
		break;
	case KCollisionType::ABB3D:
		break;
	case KCollisionType::SPHERE3D:
		break;
	case KCollisionType::POINT3D:
		break;
	default:
		return;
	}
}