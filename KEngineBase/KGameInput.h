#pragma once
#include <vector>
#include <map>
#include "KGAMESTRING.h"
#include "KGAMEDEBUG.h"
#include "KGAMEMATH.h"

class KGAMEINPUT
{
private:
	class KGameInputCreator
	{
	private:
		friend KGAMEINPUT;

	private:
		KGameInputCreator();
		~KGameInputCreator();
	};

private:
	friend KGameInputCreator;

public:
	static void DestroyInput();
	static KGameInputCreator m_instance;

private:
	enum class KEY_STATE
	{
		KEY_DOWN = 0,
		KEY_PRESS,
		KEY_UP,
		KEY_FREE,
		KEY_MAX
	};

	class KKey
	{
	public:
		float m_pressTime;
		float m_freeTime;
		std::vector<int> m_keyContainer;
		bool m_keyState[(size_t)KGAMEINPUT::KEY_STATE::KEY_MAX];

	public:
		bool CheckKeyState();
		void Update();

		float GetPressTime()
		{
			return m_pressTime;
		}


		float GetFreeTime()
		{
			return m_freeTime;
		}

		std::vector<int>& GetKeyContainter()
		{
			return m_keyContainer;
		}

		bool GetKeyState(KGAMEINPUT::KEY_STATE state)
		{
			return m_keyState[(size_t)state];
		}

	public:
		KKey();
		~KKey();
	};

private:
	static std::map<KGameString, KKey*> m_allKeyContainer;

private:
	static KKey* FindKey(const KGameString& name)
	{
		std::map<KGameString, KKey*>::iterator findKey = m_allKeyContainer.find(name);

		if (m_allKeyContainer.end() == findKey)

		{
			return nullptr;
		}

		return findKey->second;
	}

public:
	static bool IsExistKey(const KGameString& name)
	{
		if (m_allKeyContainer.end() == m_allKeyContainer.find(name))
		{
			return false;
		}

		return true;
	}

	template<typename ... Rest>
	static void CreateKey(const KGameString& name, Rest... arg)
	{
		if (true == IsExistKey(name))
		{
			BOOM;
			return;
		}

		KKey* pNewKey = new KKey();
		pNewKey->m_keyContainer.reserve(sizeof...(arg));

		m_allKeyContainer.insert(std::map<KGameString, KKey*>::value_type(name, pNewKey));

		AddKey(pNewKey, arg...);
	}

private:
	template<typename ... Rest>
	static void AddKey(KKey* pNewKey, const int& first, Rest... arg)
	{
		pNewKey->m_keyContainer.push_back(first);
		AddKey(pNewKey, arg...);
	}

	static void AddKey(KKey* pNewKey, const int& value)
	{
		pNewKey->m_keyContainer.push_back(value);
	}

public:
	static bool IsDown(const KGameString& name)
	{
		KKey* pFindKey = FindKey(name);

		if (nullptr == pFindKey)
		{
			BOOM;
			return false;
		}

		return pFindKey->GetKeyState(KEY_STATE::KEY_DOWN);
	}

	static bool IsUp(const KGameString& name)
	{
		KKey* pFindKey = FindKey(name);

		if (nullptr == pFindKey)
		{
			BOOM;
			return false;
		}

		return pFindKey->GetKeyState(KEY_STATE::KEY_UP);
	}

	static bool IsPress(const KGameString& name)
	{
		KKey* pFindKey = FindKey(name);

		if (nullptr == pFindKey)
		{
			BOOM;
			return false;
		}

		return pFindKey->GetKeyState(KEY_STATE::KEY_PRESS);
	}

	static bool IsFree(const KGameString& name)
	{
		KKey* pFindKey = FindKey(name);

		if (nullptr == pFindKey)
		{
			BOOM;
			return false;
		}

		return pFindKey->GetKeyState(KEY_STATE::KEY_FREE);
	}

private:
	static KVector m_CurPos;
	static KVector m_PrevPos;
	static KVector m_MouseDir;

public:
	static KVector ScreenMousePos(); // window
	static KVector MousePos(); // 3D
	static KVector MouseDir();
	static KVector MousePosNormal(); // 3D

public:
	static void Update();
};