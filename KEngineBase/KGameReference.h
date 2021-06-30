#pragma once

class KGameReference
{
private:
	template<typename PTRTYPE>
	friend class KPTR;

private:
	int m_RefCount;

private:
	inline void AddRef()
	{
		++m_RefCount;
	}

	inline void DecRef()
	{
		--m_RefCount;

		if (0 >= m_RefCount)
		{
			delete this;
		}
	}
public:
	KGameReference() : m_RefCount(0)
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
	template<typename WPTRTYPE>
	friend class KWPTR;


private:
	PTRTYPE* m_pType;

public:
	PTRTYPE* operator->() const
	{
		return m_pType;
	}

	operator PTRTYPE* ()
	{
		return m_pType;
	}

	template<typename OtherType>
	operator KPTR<OtherType>()
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
		if (m_pType == _Ptr)
		{
			return *this;
		}

		CheckDecRef();
		m_pType = _Ptr.m_pType;
		CheckAddRef();

		return *this;
	}


	bool operator==(const KPTR& _Ptr) const
	{
		return m_pType == _Ptr.m_pType;
	}


	bool operator==(void* _Ptr) const
	{
		return m_pType == _Ptr;
	}

	bool operator!=(void* _Ptr) const
	{
		return m_pType != _Ptr;
	}

	bool operator!=(PTRTYPE* _Ptr) const
	{
		return m_pType != _Ptr;
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
	KPTR() : m_pType(nullptr)
	{

	}

	KPTR(const KPTR& _Ptr) : m_pType(_Ptr.m_pType)
	{
		CheckAddRef();
	}


	KPTR(PTRTYPE* _Ptr) : m_pType(_Ptr)
	{
		CheckAddRef();
	}

	~KPTR()
	{
		CheckDecRef();
	}
};

template<typename PTRTYPE>
typename bool operator==(void* _Ptr, const KPTR<PTRTYPE>& _PTR)
{
	return _PTR == _Ptr;
}

template<typename PTRTYPE>
typename bool operator!=(void* _Ptr, const KPTR<PTRTYPE>& _PTR)
{
	return _PTR != _Ptr;
}

template<typename PTRTYPE>
class KWeakPTR
{
private:
	PTRTYPE* PTR;

public:
	operator PTRTYPE* ()
	{
		return PTR;
	}

	operator KPTR<PTRTYPE>()
	{
		return PTR;
	}

	PTRTYPE* operator->()
	{
		return PTR;
	}

	KWeakPTR& operator=(PTRTYPE* _Ptr)
	{
		if (PTR == _Ptr)
		{
			return *this;
		}

		PTR = _Ptr;

		return *this;
	}

	KWeakPTR& operator=(const KWeakPTR& _Ptr)
	{
		if (PTR == _Ptr)
		{
			return *this;
		}

		PTR = _Ptr.PTR;

		return *this;
	}


	bool operator==(const KWeakPTR& _Ptr) const
	{
		return PTR == _Ptr.PTR;
	}


	bool operator==(void* _Ptr) const
	{
		return PTR == _Ptr;
	}

	bool operator!=(void* _Ptr) const
	{
		return PTR != _Ptr;
	}

public:
	KWeakPTR() : PTR(nullptr)
	{

	}

	KWeakPTR(const KPTR<PTRTYPE>& _Ptr) : PTR(_Ptr.m_pType)
	{
	}

	KWeakPTR(const KWeakPTR& _Ptr) : PTR(_Ptr.PTR)
	{
	}


	KWeakPTR(PTRTYPE* _Ptr) : PTR(_Ptr)
	{
	}

	~KWeakPTR() {
	}
};

template<typename PTRTYPE>
typename bool operator==(void* _Ptr, const KWeakPTR<PTRTYPE>& _PTR)
{
	return _PTR == _Ptr;
}

template<typename PTRTYPE>
typename bool operator!=(void* _Ptr, const KWeakPTR<PTRTYPE>& _PTR)
{
	return _PTR != _Ptr;
}