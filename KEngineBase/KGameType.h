#pragma once
#include <typeinfo>
#include "KGAMESTRING.h"

class KGameType
{
private:
	const type_info* m_Info;

public:
	template<typename T>
	void TypeSetting()
	{
		m_Info = &typeid(T);
	}

	void TypeSetting()
	{
		m_Info = &typeid(*this);
	}

	template<typename T>
	bool Equal() const
	{
		return m_Info == &typeid(T);
	}

	bool Equal(const KGameType& _Other) const
	{
		return m_Info == _Other.m_Info;
	}

	bool Equal(const KGameType* _Other) const
	{
		return m_Info == _Other->m_Info;
	}

	template<typename T>
	KPTR<T> VirtualChildConvert() {
		return dynamic_cast<T*>(this);
	}

	KGameString TypeName()
	{
		return m_Info->name();
	}

public:
	KGameType() : m_Info(nullptr)
	{

	}
	virtual ~KGameType()
	{

	}
};

