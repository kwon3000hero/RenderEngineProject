#include "KCuttingRender.h"
#include "KCuttingTexture.h"

SMPOP KCuttingRender::DEFOP = SMPOP::SO_POINT;

KCuttingRender::KCuttingRender() : m_Tex(nullptr), m_fadeProgress({ 0,0,0,0 })
{
	DATA.MULCOLOR.a = 1.0f;
}
KCuttingRender::~KCuttingRender()
{

}

void KCuttingRender::StartData(int _Order)
{
	KCuttingRender::SetOrder(_Order);
	m_RD = KCuttingRender::CreateRenderPlayer(L"RECT", L"2DSPRITE");
	SetSamplerMode(DEFOP);
	SetConstantBuffer(L"CUTDATA", m_CutData, ConstantBufferMode::Link);
	SetConstantBuffer(L"TEXDATA", DATA, ConstantBufferMode::Link);
}
void KCuttingRender::StartData(const KGameString& _CuttingTexName, int _Order /*= 0*/)
{
	StartData(_Order);
	CuttingTex(_CuttingTexName);
}

void KCuttingRender::SetSamplerMode(SMPOP _OP)
{
	switch (_OP)
	{
	case SMPOP::SO_POINT:
		SetSampler(L"Smp", L"PCSMP");
		break;
	case SMPOP::SO_LINER:
		SetSampler(L"Smp", L"LCSMP");
		break;
	default:
		break;
	}
}

void KCuttingRender::CuttingTex(const KGameString& _CuttingTexName, size_t _Index)
{
	SetTexture(L"DifTex", _CuttingTexName);
	m_Tex = KCuttingTexture::Find(_CuttingTexName);

	DATA.LINEUV.x = m_Tex->PixelUvSize().x;
	DATA.LINEUV.y = m_Tex->PixelUvSize().y;

	CutIndex(_Index);
}

void KCuttingRender::LineOn()
{
	DATA.LINEUV.w = 1.0f;
}

void KCuttingRender::LineOff()
{
	DATA.LINEUV.w = 0.0f;
}

void KCuttingRender::LineColor(const KVector& _Vector)
{
	DATA.LINECOLOR = _Vector;
}

void KCuttingRender::SetFadeProgress(float _progress)
{
	m_fadeProgress.x = _progress;
	m_fadeProgress.y = _progress;
	m_fadeProgress.z = _progress;
}

void KCuttingRender::CutIndex(size_t _Index)
{
	if (nullptr == m_Tex.get())
	{
		AssertMsg(L"컷팅 텍스처가 세팅되지 않았습니다.");
	}
	m_CutData = m_Tex->CutData(_Index);
}

void KCuttingRender::RatioSetting(RATIOOP _RO)
{
	KVector Scale = LocalScale();

	switch (_RO)
	{
	case RATIOOP::RO_W:
		Scale.y *= m_CutData.w / m_CutData.z;
		break;
	case RATIOOP::RO_H:
		Scale.x *= m_CutData.z / m_CutData.w;
		break;
	default:
		break;
	}


	Scale.z = 1.0f;

	LocalScale(Scale);
}

void KCuttingRender::Update()
{
	//DATA;
}