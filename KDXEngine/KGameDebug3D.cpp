#include "KGameDebug3D.h"
#include "KGameScene.h"
#include "KGameFont.h"

KRenderer KGameDebug3D::RP;
KPTR<KMesh> KGameDebug3D::RectMesh;

int KGameDebug3D::DebugSize = 0;
std::vector<KGameDebug3D::KDebugInfo> KGameDebug3D::m_Info = std::vector<KGameDebug3D::KDebugInfo>(100);

int KGameDebug3D::m_DebugTextSize = 0;
std::vector<KGameDebug3D::KDebugTextInfo> KGameDebug3D::m_textInfo = std::vector<KGameDebug3D::KDebugTextInfo>(100);

int KGameDebug3D::m_DebugTextureSize = 0;
std::vector<KGameDebug3D::KTextureInfo> KGameDebug3D::m_TextureInfo = std::vector<KGameDebug3D::KTextureInfo>(100);

KPTR<KRenderer> KGameDebug3D::m_TextureRenderPlayer;
KPTR<KRenderer> KGameDebug3D::m_BackbufferRenderPlayer;

TransformMatrix KGameDebug3D::m_DebugTextureMatrix;

KGameDebug3D::KTextureInfo KGameDebug3D::m_FullInfo;

KVector KGameDebug3D::m_CenterVector;
KVector KGameDebug3D::m_BackColor;

float KGameDebug3D::m_basicSize;

void KGameDebug3D::Init()
{
#ifdef _DEBUG
	RectMesh = KGameResourceManager<KMesh>::Instance().Find(L"RECTDEBUG");
	RP.SetRenderPipeline(L"HDEBUG");
	RP.m_MeshRenderType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

	KGameFont::Create(L"±¼¸²");

	m_DebugTextureMatrix.Init();
	m_DebugTextureMatrix.VIEW.View({ 0,0,-10, 1 }, { 0,0,1,1 }, { 0,1,0,1 });
	m_DebugTextureMatrix.PROJ.Orth(KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y, 0, 1000.0F);

	m_CenterVector = KGameWindow::MainWindow()->Size().HalfVector2D();
	m_CenterVector.x *= -1.0f;
	m_TextureRenderPlayer = make_KPTR<KRenderer>(L"DEBUGTEXRECT", L"DEBUGIMAGE");
	m_TextureRenderPlayer->SetSampler(L"Smp", L"PCSMP");
	m_TextureRenderPlayer->SetConstantBuffer(L"TRANSFORMDATA", &m_DebugTextureMatrix, ConstantBufferMode::Link);

	m_BackbufferRenderPlayer = make_KPTR <KRenderer>(L"DEBUGTEXRECT", L"DEBUGBACK");
	m_BackbufferRenderPlayer->SetConstantBuffer(L"TRANSFORMDATA", &m_DebugTextureMatrix, ConstantBufferMode::Link);
	m_BackbufferRenderPlayer->SetConstantBuffer(L"BACKCOLOR", &m_BackColor, ConstantBufferMode::Link);
#endif
}

KGameDebug3D::STATICINIT::STATICINIT()
{

}

void KGameDebug3D::DebugRender()
{
#ifdef _DEBUG
	for (size_t i = 0; i < DebugSize; i++)
	{
		switch (m_Info[i].m_Type)
		{
		case KDEBUG_DRAW_TYPE::KDT_RECT:
			RP.m_mesh = RectMesh;
			break;
		case KDEBUG_DRAW_TYPE::KDT_BOX:
			break;
		case KDEBUG_DRAW_TYPE::KDT_LINE:
			break;
		case KDEBUG_DRAW_TYPE::KDT_CIRCLE:
			break;
		default:
			break;
		}

		if (nullptr != m_Info[i].Trans)
		{
			TransformMatrix DATA = m_Info[i].Trans->MatrixData();
			DATA.WVP.ArrVector[3].z -= 0.001f;
			RP.SetConstantBuffer(L"TRANSFORMDATA", &DATA, ConstantBufferMode::Link);
			RP.SetConstantBuffer(L"DEBUGCOLOR", (void*)&m_Info[i].Color, ConstantBufferMode::Link);
			RP.Render();
		}
		else
		{

		}
	}

	if (-1 != m_DebugTextureSize)
	{
		for (size_t i = 0; i < m_DebugTextureSize; i++)
		{
			m_DebugTextureMatrix.Final.World(m_TextureInfo[i].Size, KVector::ZERO, m_TextureInfo[i].Pos + m_CenterVector);
			m_DebugTextureMatrix.CalculateTransformMatrix();
			if (m_TextureInfo[i].BackColor != KVector::ZERO)
			{
				m_BackColor = m_TextureInfo[i].BackColor;
				m_BackbufferRenderPlayer->Render();
			}

			m_TextureRenderPlayer->SetTexture(L"DebugTex", m_TextureInfo[i].Texture);
			m_TextureRenderPlayer->Render();
		}
	}
	else
	{
		m_DebugTextureMatrix.Final.World(m_FullInfo.Size, KVector::ZERO, m_FullInfo.Pos + m_CenterVector);
		m_DebugTextureMatrix.CalculateTransformMatrix();
		if (m_FullInfo.BackColor != KVector::ZERO)
		{
			m_BackColor = m_FullInfo.BackColor;
			m_BackbufferRenderPlayer->Render();
		}

		m_TextureRenderPlayer->SetTexture(L"DebugTex", m_FullInfo.Texture);
		m_TextureRenderPlayer->Render();
	}

	KPTR<KGameFont> DefFont = KGameResourceManager<KGameFont>::Instance().Find(L"±¼¸²");

	for (size_t i = 0; i < m_DebugTextSize; i++)
	{
		DefFont->DrawString(m_textInfo[i].Text, m_textInfo[i].Size, m_textInfo[i].Pos, m_textInfo[i].Color);
	}

	m_DebugTextureSize = 0;
	m_DebugTextSize = 0;
	DebugSize = 0;
#endif
}

void KGameDebug3D::DebugRect(KPTR<KTransform> _Ptr, KPTR<KCamera> _Cam)
{
#ifdef _DEBUG
	DebugDraw(KDEBUG_DRAW_TYPE::KDT_RECT, _Ptr, _Cam);
#endif
}

void KGameDebug3D::DebugDraw(KDEBUG_DRAW_TYPE _Type, KPTR<KTransform> _Ptr, KPTR<KCamera> _Cam)
{
#ifdef _DEBUG
	m_Info[DebugSize].m_Type = _Type;
	m_Info[DebugSize].Trans = _Ptr;

	if (nullptr != _Cam.get())
	{
		m_Info[DebugSize].Cam = _Cam;
	}
	else {
		m_Info[DebugSize].Cam = KGameScene::MainCam();
	}
	m_Info[DebugSize].Color = KVector::RED;
	++DebugSize;
#endif
}

void KGameDebug3D::DebugTexture(KPTR<KRenderTarget> _renderTarget, const KVector& _pos, const KVector& _size, const KVector& _color)
{
	DebugTexture(_renderTarget->Texture(0), _pos, _size, _color);
}

void KGameDebug3D::DebugTexture(KPTR<KTexture> _texture, const KVector& _pos, const KVector& _size, const KVector& _color)
{
#ifdef _DEBUG
	if (-1 == m_DebugTextureSize)
	{
		return;
	}

	m_TextureInfo[m_DebugTextureSize].Texture = _texture;
	m_TextureInfo[m_DebugTextureSize].Pos = _pos;
	m_TextureInfo[m_DebugTextureSize].Size = _size;
	m_TextureInfo[m_DebugTextureSize].BackColor = _color;

	++m_DebugTextureSize;
#endif
}

void KGameDebug3D::DebugFullTexture(KPTR<KTexture> _texture, const KVector& _pos, const KVector& _size, const KVector& _color)
{
#ifdef _DEBUG
	m_FullInfo.Texture = _texture;
	m_FullInfo.Pos = KVector::ZERO;
	m_FullInfo.Size = KGameWindow::MainWindow()->Size();
	m_FullInfo.BackColor = _color;
	m_DebugTextureSize = -1;
#endif
}

void KGameDebug3D::DebugText(const KGameString& _str, float _size, const KVector& _pos, const KVector& _color)
{
#ifdef _DEBUG
	m_textInfo[m_DebugTextSize].Text = _str;
	m_textInfo[m_DebugTextSize].Size = _size;
	m_textInfo[m_DebugTextSize].Pos = _pos;
	m_textInfo[m_DebugTextSize].Color = _color;

	++m_DebugTextSize;
#endif
}