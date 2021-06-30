#pragma once
#include "KGameReference.h"
#include "KGAMEPATH.h"
#include "KGAMEDEBUG.h"

enum class FileOpenMode
{
	Read,
	Write,
	Max
};

class KGameFile : public KGAMEPATH, public KGameReference
{
public:
	FileOpenMode m_mode;
	FILE* m_pFile;

public:
	template<typename T>
	void operator << (const T& _value)
	{
		if (FileOpenMode::Write != m_mode)
		{
			AssertMsg(L"파일읽기 잘못된 모드 사용");
		}

		fwrite(&_value, sizeof(T), 1, m_pFile);
	}

	//KGameString에 대한 크기를 알 수 없으므로...
	void operator << (const KGameString& _value)
	{
		if (FileOpenMode::Write != m_mode)
		{
			AssertMsg(L"String에 대한 파일읽기 잘못된 모드 사용");
		}

		int size = (int)(_value.m_String.size() + 1);
		fwrite(&size, sizeof(int), 1, m_pFile);
		fwrite(_value.m_String.c_str(), sizeof(wchar_t) * size, 1, m_pFile);
	}

	template<typename T>
	void operator >> (T& _value)
	{
		if (FileOpenMode::Read != m_mode)
		{
			AssertMsg(L"파일쓰기 잘못된 모드 사용");
		}

		fread_s(&_value, sizeof(T), sizeof(T), 1, m_pFile);
	}

	//KGameString에 대한 크기를 알 수 없으므로...
	void operator >> (KGameString& _value)
	{
		if (FileOpenMode::Read != m_mode)
		{
			AssertMsg(L"String에 대한 파일쓰기 잘못된 모드 사용");
		}

		int size;
		fread_s(&size, sizeof(int), sizeof(int), 1, m_pFile);

		wchar_t* pString = new wchar_t[size];
		fread_s(pString, size * sizeof(wchar_t), size * sizeof(wchar_t), 1, m_pFile);
		_value = pString;
		delete[] pString;
	}

	void Read(void* _ptr, size_t _readSize)
	{
		if (FileOpenMode::Read != m_mode)
		{
			AssertMsg(L"파일읽기 잘못된 모드 사용");
		}

		fread_s(_ptr, _readSize, _readSize, 1, m_pFile);
	}

	void Close()
	{
		if (nullptr != m_pFile)
		{
			fclose(m_pFile);
			m_pFile = nullptr;
		}
	}

public:
	KGameString AllText();
	KGameString FileName();

public:
	KGameFile(const KGameString& _path, const KGameString& _mode);
	KGameFile(const KGameString& _path);
	~KGameFile();
};

