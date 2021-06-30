#include "KGAMEDIR.h"
#include "KGAMEDEBUG.h"
#include <Windows.h>
#include "KGAMEFILE.h"
#include <io.h>


// 내부에서 자신의 경로를 실행된 디렉토리로 변경한다.
KGAMEDIR::KGAMEDIR()
{
	m_Path.ReSize(256);
	GetCurrentDirectoryW((DWORD)m_Path.Size(), &(m_Path.operator[](0)));

	m_Path.CalculateSize();

}

KGameString KGAMEDIR::FolderName() {
	return GetFileName(m_Path);
}

bool KGAMEDIR::MoveParent(const KGameString& _Str/* = L""*/)
{
	if (_Str == L"")
	{
		wchar_t Ch = m_Path[m_Path.Size() - 1];
		if (Ch == L':')
		{
			return false;
		}

		m_Path.Erase(m_Path.LastToFind(L"\\"), m_Path.Len());
		return true;
	}
	else
	{
		KGameString Temp = m_Path;

		while (true)
		{
			KGameString LastFolderName = Temp.Cut(Temp.LastToFind(L"\\") + 1, Temp.Size());

			if (LastFolderName == _Str)
			{
				m_Path = Temp;
				return true;
			}

			wchar_t Ch = Temp[Temp.Size() - 1];
			if (Ch == L':')
			{
				return false;
			}

			Temp.Erase(Temp.LastToFind(L"\\"), Temp.Len());
		}

		return false;
	}
}

bool KGAMEDIR::Move(const KGameString& _Str)
{
	KGameString Temp = m_Path + L"\\" + _Str;

	if (false == IsExist(Temp))
	{
		BOOM;
	}

	m_Path += L"\\";
	m_Path += _Str;

	return true;
}

bool KGAMEDIR::IsFile(const KGameString& _FileName)
{
	if (false == _FileName.IsExist(L'.'))
	{
		return false;
	}


	return IsExist(FilePath(_FileName));
}

KGameString KGAMEDIR::FilePath(const KGameString& _FileName)
{
	return m_Path + L"\\" + _FileName;
}

std::list<KGameFile> KGAMEDIR::DirAllFile(const wchar_t* _Ext, const wchar_t* _Name)
{
	std::list<KGameFile> AllFileList;

	int checkDirFile = 0;

	KGameString dirPath;

	KGameString Ext = _Ext;
	KGameString Name = _Name;

	if (Name != L"")
	{
		dirPath = m_Path.m_String + L"\\" + Name.m_String + L"*";
	}
	else {
		dirPath = m_Path.m_String + L"\\*";
	}

	if (Ext != L"")
	{
		dirPath += L"." + Ext.m_String;
	}
	else {
		dirPath += L".*";
	}

	_wfinddata64i32_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
	intptr_t handle;

	if ((handle = _wfindfirst(dirPath, &fd)) == -1L)
	{
		return AllFileList;
	}

	do //폴더 탐색 반복 시작
	{
		if (fd.attrib & _A_SUBDIR)
		{
			checkDirFile = 0;
		}
		else
		{
			checkDirFile = 1;
		}

		if (fd.name[0] == L'.')
		{
			continue;
		}

		if (checkDirFile == 1)
		{
			KGameString FilePath = m_Path.m_String + L"\\" + fd.name;

			KGameFile NewFile = KGameFile(FilePath);

			AllFileList.push_back(NewFile);
		}

		// 다음 파일이 없으면 -1인가를 리턴할 것이다.
	} while (_wfindnext(handle, &fd) == 0);

	_findclose(handle);


	return AllFileList;
}