#pragma once
#include "KRendererManager.h"

enum class SamplerOption
{
	SO_POINT,
	SO_LINER,
};

enum class RATIOOP
{
	RO_W,
	RO_H,
};

struct TextureRenderData
{
	KVector LINEUV;
	KVector LINECOLOR;
	KVector MULCOLOR;
};

class KCuttingTexture;
class KCuttingRender : public KRenderManager
{
public:
	static SamplerOption DEFOP;

private:
	KVector m_CutData;
	KPTR<KRenderer> m_RD;
	KPTR<KCuttingTexture> m_Tex;
	TextureRenderData DATA;
	KVector m_fadeProgress;

public:
	void CuttingTex(const KGameString& _CuttingTexName, size_t _Index = 0);
	void CutIndex(size_t _Index);
	void SetFadeProgress(float _progress);
	void SetSamplerMode(SamplerOption _OP);
	void RatioSetting(RATIOOP _RO = RATIOOP::RO_W);

	void LineOn();
	void LineOff();

	void LineColor(const KVector& _Vector);

public:
	void Update();

public:
	KCuttingRender();
	KCuttingRender(int _Order);
	KCuttingRender(const KGameString& _CuttingTexName, int _Order = 0);
	~KCuttingRender();
};

