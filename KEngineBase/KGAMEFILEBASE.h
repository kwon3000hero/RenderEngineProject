#pragma once
#include "KGAMEFILE.h"

class KGameFileBase
{
private:
	KGameFile* m_File;

public:
	void SetFile(const KGameFile& _File)
	{
		m_File = new KGameFile(_File);
	}



public:
	KGameFileBase() : m_File(nullptr) {}
	~KGameFileBase() {}

};

