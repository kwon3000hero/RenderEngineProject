#pragma once
#include "KGameDeviceManager.h"
#include "KGameResource.h"

#include <FW1CompileSettings.h>
#include <FW1FontWrapper.h>

#pragma comment(lib, "FW1FontWrapper.lib")

class KGameFont : public KGameResource<KGameFont>
{
private:
	static IFW1Factory* m_pFontFactory;
	IFW1FontWrapper* m_pFontWrapper;

public:
	static void Init();
	static void End();

public:
	static void Create(const KGameString& _str)
	{
		KGameFont* pResource = new KGameFont();
		pResource->SetName(_str);
		pResource->Create();
		pResource->InsertResource();
	}

public:
	void Create();
	void DrawString(const KGameString& _str, float _size, const KVector& _pos, const KVector& _color, FW1_TEXT_FLAG _flag = FW1_TEXT_FLAG::FW1_TOP);

public:
	KGameFont();
	~KGameFont();
};

