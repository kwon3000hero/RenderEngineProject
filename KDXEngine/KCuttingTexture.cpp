#include "KCuttingTexture.h"
#include "KTexture.h"


void KCuttingTexture::Create(int _W, int _H)
{
	KPTR<KTexture> Ptr = KTexture::Find(m_Path);

	if (nullptr == Ptr)
	{
		AssertMsg(L"텍스처가 존재하지 않습니다.");
	}

	Create({ 0,0 }, Ptr->Size(), _W, _H);
}

void KCuttingTexture::Create(const KVector& _Start, const KVector& _Size, int _W, int _H)
{
	KPTR<KTexture> Ptr = KTexture::Find(m_Path);

	if (nullptr == Ptr)
	{
		AssertMsg(L"텍스처가 존재하지 않습니다.");
	}

	KVector StartRatio;

	m_PixelUvSize.x = 1.0f / Ptr->Size().x;
	m_PixelUvSize.y = 1.0f / Ptr->Size().y;

	StartRatio.x = _Start.x / Ptr->Size().x;
	StartRatio.y = _Start.y / Ptr->Size().y;

	KVector Cut;
	Cut.SizeX = (_Size.x / Ptr->Size().x) / _W;
	Cut.SizeY = (_Size.y / Ptr->Size().y) / _H;

	for (size_t y = 0; y < _H; y++)
	{
		for (size_t x = 0; x < _W; x++)
		{
			// 시작 위치
			Cut.x = (Cut.z * x) + StartRatio.x;
			Cut.y = (Cut.w * y) + StartRatio.y;
			m_CutData.push_back(Cut);
		}
	}


}