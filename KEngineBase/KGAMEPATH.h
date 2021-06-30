#pragma once
#include "KGAMESTRING.h"
#include <iostream>

class KGAMEPATH
{
public:
	KGameString m_Path;

public:
	static bool IsExist(const KGameString& _Path)
	{
		return 0 == _waccess_s(_Path, 0);
	}

	static KGameString GetFileName(const KGameString& _Path)
	{
		return _Path.Cut(_Path.LastToFind(L"\\") + 1, _Path.Size());
	}

	static KGameString GetExt(const KGameString& _Path)
	{
		return _Path.Cut(_Path.LastToFind(L".") + 1, _Path.Size());
	}


public:
	KGAMEPATH() {}

	KGAMEPATH(const KGameString& _Path) : m_Path(_Path)
	{}

	virtual ~KGAMEPATH() {}
};

