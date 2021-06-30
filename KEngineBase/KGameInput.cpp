#include "KGameInput.h"
#include "KGAMEWIN.h"

KVector KGAMEINPUT::m_MouseDir;
KVector KGAMEINPUT::m_PrevPos;
KVector KGAMEINPUT::m_CurPos;

std::map<KGameString, KGAMEINPUT::KKey*> KGAMEINPUT::m_allKeyContainer;
KGAMEINPUT::KGameInputCreator KGAMEINPUT::m_instance;


KGAMEINPUT::KGameInputCreator::KGameInputCreator()
{

}

KGAMEINPUT::KGameInputCreator::~KGameInputCreator()
{
	DestroyInput();
}

void KGAMEINPUT::DestroyInput()
{
	std::map<KGameString, KGAMEINPUT::KKey*>::iterator iter = m_allKeyContainer.begin();
	std::map<KGameString, KGAMEINPUT::KKey*>::iterator end = m_allKeyContainer.end();

	for (; iter != end; ++iter)
	{
		delete iter->second;
	}
}


bool KGAMEINPUT::KKey::CheckKeyState()
{
	for (size_t i = 0; i < m_keyContainer.size(); ++i)
	{
		if (0 == GetAsyncKeyState(m_keyContainer[i]))
		{
			return false;
		}
	}

	return true;
}

KGAMEINPUT::KKey::KKey() : m_pressTime(0.0f), m_freeTime(0.0f)
{
	m_keyState[0] = false;
	m_keyState[1] = false;
	m_keyState[2] = false;
	m_keyState[3] = true;
}
KGAMEINPUT::KKey::~KKey()
{

}

void KGAMEINPUT::KKey::Update()
{
	//모든 키가 눌렸을 때
	if (true == CheckKeyState())
	{
		//그 전까지 떼어졌다면
		if (true == m_keyState[(size_t)KEY_STATE::KEY_FREE])
		{
			m_keyState[(size_t)KEY_STATE::KEY_DOWN] = true;
			m_keyState[(size_t)KEY_STATE::KEY_PRESS] = true;
			m_keyState[(size_t)KEY_STATE::KEY_UP] = false;
			m_keyState[(size_t)KEY_STATE::KEY_FREE] = false;
		}
		else
		{
			m_keyState[(size_t)KEY_STATE::KEY_DOWN] = false;
			m_keyState[(size_t)KEY_STATE::KEY_PRESS] = true;
			m_keyState[(size_t)KEY_STATE::KEY_UP] = false;
			m_keyState[(size_t)KEY_STATE::KEY_FREE] = false;
		}
	}
	else
	{
		if (true == m_keyState[(size_t)KEY_STATE::KEY_PRESS])
		{
			m_keyState[(size_t)KEY_STATE::KEY_DOWN] = false;
			m_keyState[(size_t)KEY_STATE::KEY_PRESS] = false;
			m_keyState[(size_t)KEY_STATE::KEY_UP] = true;
			m_keyState[(size_t)KEY_STATE::KEY_FREE] = true;
		}
		else
		{
			m_keyState[(size_t)KEY_STATE::KEY_DOWN] = false;
			m_keyState[(size_t)KEY_STATE::KEY_PRESS] = false;
			m_keyState[(size_t)KEY_STATE::KEY_UP] = false;
			m_keyState[(size_t)KEY_STATE::KEY_FREE] = true;
		}
	}
}

void KGAMEINPUT::Update()
{
	m_CurPos = ScreenMousePos();

	m_CurPos = m_CurPos - KGameWindow::MainWindow()->Size().HalfVector2D();
	m_CurPos.y = -m_CurPos.y;

	m_MouseDir = m_CurPos - m_PrevPos;
	m_PrevPos = m_CurPos;

	std::map<KGameString, KGAMEINPUT::KKey*>::iterator iter = m_allKeyContainer.begin();
	std::map<KGameString, KGAMEINPUT::KKey*>::iterator end = m_allKeyContainer.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update();
	}
}


KVector KGAMEINPUT::ScreenMousePos()
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(KGameWindow::MainWindow()->WINHWND(), &pt);
	KVector Pos = { (float)pt.x, (float)pt.y };
	return Pos;
}

KVector KGAMEINPUT::MousePosNormal()
{
	return m_CurPos.NormalToReturn();
}


KVector KGAMEINPUT::MousePos()
{
	return m_CurPos;
}

KVector KGAMEINPUT::MouseDir()
{
	return m_MouseDir;
}