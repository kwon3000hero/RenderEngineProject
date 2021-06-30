#pragma once
class KUpdateBase
{
private:
	bool m_bUpdate;
	bool m_bDeath;
	bool m_bDebug;

public:
	virtual bool IsDeath()
	{
		return m_bDeath == true;
	}

	virtual bool IsUpdate()
	{
		return m_bDeath == false && m_bUpdate == true;
	}

	void DebugOn()
	{
		m_bDebug = true;
	}

	void DebugOff()
	{
		m_bDebug = false;
	}

	void Death()
	{
		m_bDeath = true;
	}

	void On()
	{
		m_bUpdate = true;
	}

	void Off()
	{
		m_bUpdate = false;
	}

public:
	virtual void Init() {}

	virtual void Update() {}
	virtual void PrevUpdate() {}
	virtual void NextUpdate() {}
	virtual void PrevRender() {}
	virtual void DebugRender() {}

public:
	KUpdateBase() : m_bUpdate(true), m_bDeath(false), m_bDebug(false)
	{
	}
	virtual ~KUpdateBase()
	{
	}

};

