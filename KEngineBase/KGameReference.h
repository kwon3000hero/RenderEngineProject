#pragma once

class KGameReference
{
private:
	template<typename PTRTYPE>
	friend class KPTR;

private:
	int m_ReferenceCount;

private:
	inline void AddRef()
	{
		++m_ReferenceCount;
	}

	inline void DecRef()
	{
		--m_ReferenceCount;

		if (0 >= m_ReferenceCount)
		{
			delete this;
		}
	}
public:
	KGameReference() : m_ReferenceCount(0)
	{

	}

	virtual ~KGameReference()
	{

	}

};

template<typename PTRTYPE>
class KPTR
{
public:
	template<typename type>
	friend class KWeakPTR;


private:
	PTRTYPE* m_pType;

public:
	PTRTYPE* operator->() const
	{
		return m_pType;
	}

	PTRTYPE* get() const
	{
		return m_pType;
	}

	KPTR& operator=(PTRTYPE* _Ptr)
	{
		if (m_pType == _Ptr)
		{
			return *this;
		}

		CheckDecRef();
		m_pType = _Ptr;
		CheckAddRef();

		return *this;
	}

	KPTR& operator=(const KPTR& _Ptr)
	{
		if (m_pType == _Ptr.get())
		{
			return *this;
		}

		CheckDecRef();
		m_pType = _Ptr.m_pType;
		CheckAddRef();

		return *this;
	}

	bool operator!=(const KPTR& _Ptr) const
	{
		return m_pType != _Ptr.m_pType;
	}

public:
	void CheckDecRef()
	{
		if (nullptr == m_pType)
		{
			return;
		}

		m_pType->DecRef();
	}

	void CheckAddRef()
	{
		if (nullptr == m_pType)
		{
			return;
		}

		m_pType->AddRef();
	}
public:
	explicit KPTR() : m_pType(nullptr)
	{

	}

	//move 만들어서 없애자..
	KPTR(const KPTR& _Ptr) : m_pType(_Ptr.m_pType)
	{
		CheckAddRef();
	}

	//수정 필요...
	/*explicit*/ KPTR(PTRTYPE* _Ptr) : m_pType(_Ptr)
	{
		CheckAddRef();
	}

	~KPTR()
	{
		CheckDecRef();
	}
};

template<typename PTRTYPE>
class KWeakPTR
{
private:
	PTRTYPE* m_Type;

public:
	operator PTRTYPE* ()
	{
		return m_Type;
	}

	operator KPTR<PTRTYPE>()
	{
		return KPTR<PTRTYPE>(m_Type);
	}

	PTRTYPE* operator->()
	{
		return m_Type;
	}

	//KWeakPTR& operator=(const KWeakPTR& _Ptr)
	//{
	//	if (_Ptr.m_Type == m_Type)
	//	{
	//		return *this;
	//	}

	//	m_Type = _Ptr.m_Type;

	//	return *this;
	//}

	KWeakPTR& operator=(const KPTR<PTRTYPE>& _Ptr)
	{
		if (_Ptr.get() == m_Type)
		{
			return *this;
		}

		m_Type = _Ptr.m_pType;

		return *this;
	}

	/*bool operator==(const KWeakPTR& _Ptr) const
	{
		return PTR == _Ptr.m_Type;
	}


	bool operator==(void* _Ptr) const
	{
		return PTR == _Ptr;
	}

	bool operator!=(void* _Ptr) const
	{
		return PTR != _Ptr;
	}*/

public:
	explicit KWeakPTR() : m_Type(nullptr)
	{

	}

	KWeakPTR(const KPTR<PTRTYPE>& _Ptr) : m_Type(_Ptr.m_pType)
	{
	}

	explicit KWeakPTR(PTRTYPE* _Ptr) : m_Type(_Ptr)
	{
	}

	~KWeakPTR()
	{
	}
};

//template<typename PTRTYPE>
//typename bool operator==(void* _Ptr, const KWeakPTR<PTRTYPE>& _PTR)
//{
//	return _PTR == _Ptr;
//}
//
//template<typename PTRTYPE>
//typename bool operator!=(void* _Ptr, const KWeakPTR<PTRTYPE>& _PTR)
//{
//	return _PTR != _Ptr;
//}

template<typename T, typename... Ts>
KPTR<T> make_KPTR(Ts&&... params)
{
	return KPTR<T>(new T(std::forward<Ts>(params)...));
}