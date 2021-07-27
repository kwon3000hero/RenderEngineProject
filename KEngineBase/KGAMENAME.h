#pragma once
#include "KGAMESTRING.h"

class KGAMENAME
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
	KGAMENAME() {}
	virtual ~KGAMENAME() {}
};
