#pragma once
#include "KGAMEPATH.h"
#include <list>

class KGameFile;
class KGAMEDIR : public KGAMEPATH
{
public:
	bool MoveParent(const KGameString& _Str = L"");

	bool Move(const KGameString& _Str);

	std::list<KGameFile> DirAllFile(const wchar_t* _Ext = L"", const wchar_t* _Name = L"");

	KGameString FolderName();

	bool IsFile(const KGameString& _FileName);

	KGameString FilePath(const KGameString& _FileName);

public:
	KGAMEDIR();
};

