#pragma once
#include "KGAMESTRING.h"

class KGameName
{
protected:
	KGameString m_Name;

public:
	void SetName(const KGameString& _Name)
	{
		m_Name = _Name;
	}

	KGameString Name()
	{
		return m_Name;
	}


public:
	KGameName() {}
	virtual ~KGameName() {}
};
