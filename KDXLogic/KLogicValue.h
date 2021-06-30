#pragma once

#include <KCamera.h>
#include <KTransform.h>

class KLogicValue
{
private:
	static KPTR<KCamera> m_MainCam;
	static KPTR<KTransform> m_MainCamTrans;

public:
	static KPTR<KTransform> MainCamTrans()
	{
		return m_MainCamTrans;
	}

	static KPTR<KCamera> MainCam()
	{
		return m_MainCam;
	}

	static void MainCam(KPTR<KCamera> _Cam)
	{
		m_MainCam = _Cam;
		m_MainCamTrans = m_MainCam->Actor()->Transform();
	}
};