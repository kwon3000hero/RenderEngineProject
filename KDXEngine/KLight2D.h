#pragma once
#include "KCam.h" 

class KLight2D : public KPostEffect
{
private:
	KVector m_screenRatio;

public:
	KVector SetScreenRatio(const KVector& _ratio)
	{
		m_screenRatio = _ratio;
	}
	KVector ScreenRatio() const
	{
		return m_screenRatio;
	}

public:
	void Init() override;
	void MergeNextEffect() override;
};