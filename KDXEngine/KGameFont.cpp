#include "KGameFont.h"
#include "KGameDeviceManager.h"

IFW1Factory* KGameFont::m_pFontFactory = nullptr;

void KGameFont::Init()
{
	if (nullptr == m_pFontFactory)
	{
		if (S_OK != FW1CreateFactory(FW1_VERSION, &m_pFontFactory))
		{
			AssertMsg(L"��Ʈ ���丮 ������ �����߽��ϴ�.");
		}
	}
}

void KGameFont::End()
{
	if (nullptr != m_pFontFactory)
	{
		m_pFontFactory->Release();
		m_pFontFactory = nullptr;
	}
}

KGameFont::KGameFont() : m_pFontWrapper(nullptr)
{

}
KGameFont::~KGameFont()
{
	if (nullptr != m_pFontWrapper)
	{
		m_pFontWrapper->Release();
		m_pFontWrapper = nullptr;
	}
}



void KGameFont::Create()
{
	if (S_OK != m_pFontFactory->CreateFontWrapper(KGameDeviceManager::MainDevice().Get(), Name(), &m_pFontWrapper))
	{
		AssertMsg(Name() + "��Ʈ ���丮 ������ �����߽��ϴ�.");
	}
}
void KGameFont::DrawString(const KGameString& _str, float _size, const KVector& _pos, const KVector& _color, FW1_TEXT_FLAG _flag/* = FW1_TEXT_FLAG::FW1_TOP*/)
{
	m_pFontWrapper->DrawString(KGameDeviceManager::MainContext().Get(), _str, _size, _pos.x, _pos.y, _color.ColorToUint(), _flag);
}