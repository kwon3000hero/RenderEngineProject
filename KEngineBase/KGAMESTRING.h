#pragma once
#include <string>
#include <atlstr.h>
#include <list>
#include <algorithm>

struct StringParsingData;
class KGameStringHash;
class KGameString
{
	friend KGameStringHash;

public:
	std::wstring m_String;

public:
	//static KGameString Empty;
	//static KGameString WhiteSpace;

	//static bool IsWhiteSpaceOrEmpty(const KGameString& _text);

	operator std::wstring() const
	{
		return m_String;
	}

	operator const wchar_t* () const
	{
		return m_String.c_str();
	}

	bool IsEmpty() const
	{
		return m_String.size();
	}

	template<typename FmtType, typename... TemplateTypes>
	void Append(const FmtType& Fmt, TemplateTypes... Args)
	{
		AppendInternal(Fmt, Args...);
	}

	void AppendInternal(const char* fmt, ...);
	void AppendInternal(const wchar_t* fmt, ...);

	size_t IsExist(const wchar_t _Str) const { return m_String.find(_Str) != MAXSIZE_T; }
	size_t IsExist(const KGameString& _Str) const { return m_String.find(_Str.m_String) != MAXSIZE_T; }
	size_t IsExist(const wchar_t* _Str) const { return m_String.find(_Str) != MAXSIZE_T; }

	size_t LastToFind(const wchar_t* _Find) const
	{
		return m_String.rfind(_Find);
	}

	void Erase(size_t _Start, size_t _End)
	{
		m_String.replace(_Start, _End, L"");
	}

	void Change(const KGameString& _Find, const KGameString& _Change)
	{
		size_t Count = m_String.find(_Find.m_String);
		m_String = m_String.replace(Count, Count + _Find.Size(), _Change.m_String);
	}

	KGameString Cut(size_t _Start, size_t _End) const
	{
		return m_String.substr(_Start, _End - _Start + 1);
	}

	KGameString ToUpper() const
	{
		KGameString converted;
		converted.ReSize(m_String.size());
		std::transform(m_String.begin(), m_String.end(), converted.m_String.begin(), std::toupper);
		return converted;
	}

	KGameString ToLower() const
	{
		KGameString converted;
		converted.ReSize(m_String.size());
		std::transform(m_String.begin(), m_String.end(), converted.m_String.begin(), std::tolower);
		return converted;
	}

	size_t Size() const
	{
		return m_String.size();
	}

	size_t Len()
	{
		return m_String.length();
	}

	void CalculateSize()
	{
		size_t CS = 0;
		for (; CS < m_String.size(); ++CS)
		{
			if (0 == m_String[CS])
			{
				break;
			}

		}

		return m_String.resize(CS);
	}

	KGameString FileName();

	void ReSize(size_t _Size)
	{
		m_String.resize(_Size);
	}

	void Reserve(size_t _Size)
	{
		m_String.reserve(_Size);
	}

	wchar_t& operator[](size_t _Index) {
		return m_String[_Index];
	}

public:
	operator std::string() const
	{
		std::string Convert = CW2A(m_String.c_str()/*, CP_UTF*/).m_psz;
		return Convert;
	}

	bool operator>(const KGameString& _Str) const
	{
		return m_String > _Str.m_String;
	}

	bool operator<(const KGameString& _Str) const
	{
		return m_String < _Str.m_String;
	}

	bool operator==(const wchar_t* _Str) const;
	bool operator==(const KGameString& _Str) const;
	bool operator==(const char* _Str) const;

	bool operator!=(const KGameString& _Str) const;
	bool operator!=(const wchar_t* _Str) const;

	KGameString& operator=(const wchar_t* _Str)
	{
		m_String = _Str;

		return *this;
	}

	size_t FindPos(const KGameString& _findText, size_t _offset);
	size_t FindReversePos(const KGameString& _findText, size_t _offset);

	size_t TrimPos(size_t _offset);
	size_t ReverseTrimPos(size_t _offset);

	size_t FindTrimPos(size_t _offset);
	size_t FindReverseTrimPos(size_t _offset);

	StringParsingData FindStringParsingData(const KGameString& _Text, size_t _Offset);
	StringParsingData TrimToTrimFind(size_t _Offset);
	StringParsingData ReversTrimToTrimFind(size_t _Offset);

public:
	std::list<StringParsingData> FindAllPosition(const KGameString& _Text);
	std::list<StringParsingData> ReversFindAllPosition(const KGameString& _Text, size_t _Offset);
	std::list<StringParsingData> FindAllPosition(const KGameString& _Text, size_t _Offset);

	wchar_t At(size_t _index)
	{
		if (Size() <= _index)
		{
			return L'\x0';
		}

		return m_String.at(_index);
	}

	KGameString& operator+=(const KGameString& _Str)
	{
		m_String += _Str.m_String;
		return *this;
	}


	KGameString& operator+=(const wchar_t* _Str)
	{
		m_String += _Str;

		return *this;
	}


	KGameString operator+(const wchar_t* _Str) const
	{
		KGameString Temp;
		Temp.m_String = m_String;
		Temp.m_String += _Str;

		return Temp;
	}

	KGameString operator+(const KGameString& _Str) const
	{
		KGameString Temp;
		Temp.m_String = m_String;
		Temp.m_String += _Str.m_String;

		return Temp;
	}

	KGameString operator+(const int _number) const
	{
		KGameString Temp;
		Temp.m_String = m_String;

		wchar_t temp[20];
		_itow_s(_number, temp, 10);
		Temp.m_String += temp;

		return Temp;
	}


	KGameString& operator=(const char* _Str)
	{
		m_String = CA2W(_Str).m_psz;
		return *this;
	}

public:
	KGameString()
	{
	}

	KGameString(const std::wstring& _Str) : m_String(_Str)
	{
	}

	KGameString(const wchar_t* _Str) : m_String(_Str)
	{
	}

	KGameString(const char* _Str) : m_String(CA2W(_Str).m_psz)
	{
	}

	~KGameString()
	{
	}

};

bool operator==(const wchar_t* _Left, const KGameString& _Right);
KGameString operator+(const wchar_t* _Left, const KGameString& _Right);

struct StringParsingData
{
public:
	KGameString Name;
	size_t StartPos;
	size_t EndPos;
};

class KGameStringHash
{
public:
	std::hash<std::wstring> HashStr;

public:
	size_t operator()(const KGameString& _Str) const
	{
		return HashStr(_Str.m_String);
	}

public:
	KGameStringHash() {	}
};
