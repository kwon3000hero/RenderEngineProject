#pragma once

#include <comdef.h>
#include <KGAMEDEBUG.h>
//#include <KGAMESTRING.h>

struct IUnknown;

inline int SafeRelease(IUnknown* pReference);

inline void ShowHResultString(HRESULT hresult, KGameString addMessage = "")
{
	_com_error error(hresult);
	KGameString str = error.ErrorMessage();
	str.Append("\n");
	str.Append(addMessage);

	AssertMsg(str);
}