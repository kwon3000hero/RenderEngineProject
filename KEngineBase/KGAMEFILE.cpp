#include "KGAMEFILE.h"

KGameFile::KGameFile(const KGameString& _path, const KGameString& _mode) : KGAMEPATH(_path), m_pFile(nullptr), m_mode(FileOpenMode::Max)
{
	if (-1 != _mode.LastToFind(L"w"))
	{
		m_mode = FileOpenMode::Write;
	}
	else if (-1 != _mode.LastToFind(L"r"))
	{
		m_mode = FileOpenMode::Read;
	}

	_wfopen_s(&m_pFile, _path, _mode);

	if (nullptr == m_pFile)
	{
		AssertMsg(_path + L"파일 오픈 실패");
	}
}

KGameFile::KGameFile(const KGameString& _path) : KGAMEPATH(_path), m_pFile(nullptr), m_mode(FileOpenMode::Max)
{
}

KGameFile::~KGameFile()
{
	if (nullptr != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}

KGameString KGameFile::AllText()
{
	fseek(m_pFile, 0, SEEK_END);
	size_t size = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);
	char* pNewPtr = new char[size + 1];
	Read(pNewPtr, size + 1);

	KGameString data = pNewPtr;
	delete[] pNewPtr;
	return data;
}

KGameString KGameFile::FileName()
{
	return GetFileName(m_Path);
}
