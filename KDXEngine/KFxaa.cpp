#include "KFxaa.h"
#include "KGameDebug3D.h"
#include <KGameInput.h>
#include "KRenderTarget.h"
#include "KRenderPipeline.h"

void KFxaa::Init()
{
	m_FXAAPlayer = make_KPTR<KRenderPlayer>(L"FULLRECT", L"FXAA");
	m_FXAAPlayer->SetTexture(L"inputTexture", CameraPtr()->CurrentCameraTarget()->Texture(0));
	m_FXAAPlayer->SetSampler(L"anisotropicSampler", L"LWSMP");

	m_FXAATarget = make_KPTR<KRenderTarget>();
	m_FXAATarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
}

void KFxaa::MergeNextEffect()
{
	m_FXAATarget->Effect(m_FXAAPlayer);
	KRenderPipeline::Reset();
	CameraPtr()->CurrentCameraTarget()->AlwaysCopy(m_FXAATarget);
	// HPTR<HRENDERTARGET>

	// CAM()->m_CamTarget = m_FXAATarget;

	// CAM()->CamTarget()->Effect(0, m_LightBloomPlayer, false, false);

}
