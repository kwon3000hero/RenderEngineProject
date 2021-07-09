#pragma once
#include "KTransform.h"
#include <unordered_set>
#include <functional>

class KCollision : public KTransform
{
public:
	friend KGameScene;

private:
	KCollisionType m_Type;
	std::unordered_set<KCollision*> m_OtherCol;

public:
	std::list<std::function<void(KPTR<KCollision>, KPTR<KCollision>)>> m_EnterFunc;
	std::list<std::function<void(KPTR<KCollision>, KPTR<KCollision>)>> m_StayFunc;
	std::list<std::function<void(KPTR<KCollision>, KPTR<KCollision>)>> m_ExitFunc;

	bool IsDeath() override
	{
		return KUpdateBase::IsDeath() || IsDeath();
	}

	bool IsUpdate() override
	{
		return KUpdateBase::IsUpdate() && IsUpdate();
	}


public:

	template<typename T>
	void EnterFunc(void(T::* _Func)(KPTR<KCollision>, KPTR<KCollision>), T* _Obj)
	{
		if (nullptr == _Func)
		{
			AssertMsg(L"NULLPTR을 콜백으로 사용할 수는 없습니다.");
		}

		m_EnterFunc.push_back(std::bind(_Func, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	void EnterFunc(void(*_Func)(KPTR<KCollision>, KPTR<KCollision>))
	{
		if (nullptr == _Func)
		{
			AssertMsg(L"NULLPTR을 콜백으로 사용할 수는 없습니다.");
		}

		m_EnterFunc.push_back(std::bind(_Func, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void StayFunc(void(T::* _Func)(KPTR<KCollision>, KPTR<KCollision>), T* _Obj)
	{
		if (nullptr == _Func)
		{
			AssertMsg(L"NULLPTR을 콜백으로 사용할 수는 없습니다.");
		}

		m_StayFunc.push_back(std::bind(_Func, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	void StayFunc(void(*_Func)(KPTR<KCollision>, KPTR<KCollision>))
	{
		if (nullptr == _Func)
		{
			AssertMsg(L"NULLPTR을 콜백으로 사용할 수는 없습니다.");
		}

		m_StayFunc.push_back(std::bind(_Func, std::placeholders::_1, std::placeholders::_2));
	}


	template<typename T>
	void ExitFunc(void(T::* _Func)(KPTR<KCollision>, KPTR<KCollision>), T* _Obj)
	{
		if (nullptr == _Func)
		{
			AssertMsg(L"NULLPTR을 콜백으로 사용할 수는 없습니다.");
		}

		m_ExitFunc.push_back(std::bind(_Func, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	void ExitFunc(void(*_Func)(KPTR<KCollision>, KPTR<KCollision>))
	{
		if (nullptr == _Func)
		{
			AssertMsg(L"NULLPTR을 콜백으로 사용할 수는 없습니다.");
		}

		m_ExitFunc.push_back(std::bind(_Func, std::placeholders::_1, std::placeholders::_2));
	}

public:
	void StartData(int _Order = 0);
	void StartData(KCollisionType _Type, int _Order = 0);
	void Init();

	void DebugRender() override;

private:
	void ColCheck(KPTR<KCollision> _OtherCol);

	void ColEnter(KPTR<KCollision> _Other);
	void ColStay(KPTR<KCollision> _Other);
	void ColExit(KPTR<KCollision> _Other);
};

