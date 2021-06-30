#pragma once
#include "KGAMEFILE.h"

class KGameFileBase
{
private:
	KPTR<KGameFile> m_File;

public:
	void File(const KGameFile& _File)
	{
		m_File = new KGameFile(_File);
	}



public:
	KGameFileBase() : m_File(nullptr) {}
	~KGameFileBase() {}

};

