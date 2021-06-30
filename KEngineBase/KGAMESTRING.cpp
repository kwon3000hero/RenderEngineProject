#include "KGAMESTRING.h"
#include "KGAMEPATH.h"

//KGameString KGameString::Empty = "";
//KGameString KGameString::WhiteSpace = " ";
//
//bool KGameString::IsWhiteSpaceOrEmpty(const KGameString& _text)
//{
//	if (Empty == _text || WhiteSpace == _text)
//	{
//		return true;
//	}
//
//	return false;
//}

KGameString KGameString::FileName()
{
	return KGAMEPATH::GetFileName(m_String);
}

bool KGameString::operator==(const wchar_t* _Str) const
{
	return m_String == _Str;
}

bool KGameString::operator==(const KGameString& _Str) const
{
	return m_String == _Str.m_String;
}

bool KGameString::operator==(const char* _Str) const
{
	KGameString Test = CW2A(m_String.c_str()).m_psz;
	return Test == KGameString(_Str);
}

bool KGameString::operator!=(const KGameString& _Str) const
{
	return m_String != _Str.m_String;
}

bool KGameString::operator!=(const wchar_t* _Str) const
{
	return m_String != _Str;
}

bool operator==(const wchar_t* _Left, const KGameString& _Right)
{
	return _Right == _Left;
}

KGameString operator+(const wchar_t* _Left, const KGameString& _Right)
{
	KGameString left = _Left;
	left.operator+=(_Right);

	return left;
}

size_t KGameString::FindPos(const KGameString& _findText, size_t _offset)
{
	return m_String.find(_findText.m_String.c_str(), _offset);
}

size_t KGameString::FindReversePos(const KGameString& _findText, size_t _offset)
{
	return m_String.rfind(_findText.m_String.c_str(), _offset);
}

size_t KGameString::TrimPos(size_t _offset)
{
	size_t endPos = _offset;

	while (L' ' == At(endPos) || L'\n' == At(endPos))
	{
		++endPos;
	}

	return endPos;
}

size_t KGameString::ReverseTrimPos(size_t _offset)
{
	size_t endPos = _offset;

	while (L' ' == At(endPos) || L'\n' == At(endPos))
	{
		--endPos;
	}

	return endPos;
}

size_t KGameString::FindTrimPos(size_t _offset)
{
	size_t endPos = _offset;

	while (L' ' != At(endPos) && L'\n' != At(endPos))
	{
		++endPos;

		if (0 == Size())
		{
			break;
		}
	}

	return endPos;
}

size_t KGameString::FindReverseTrimPos(size_t _offset)
{
	size_t endPos = _offset;

	while (true)
	{
		wchar_t checkChar = At(endPos);

		if (L' ' == checkChar || L'\n' == checkChar)
		{
			return endPos + 1;
		}

		--endPos;

		if (0 == endPos)
		{
			break;
		}
	}

	return endPos;
}

StringParsingData KGameString::FindStringParsingData(const KGameString& _Text, size_t _Offset)
{
	std::list<StringParsingData> m_Data;

	size_t FindSize = m_String.find(_Text.m_String, _Offset + 1);
	return { _Text, FindSize, FindSize + _Text.Size() };

}

StringParsingData KGameString::TrimToTrimFind(size_t _Offset) {

	size_t startPos = TrimPos(_Offset);
	size_t endPos = FindTrimPos(startPos);

	return { Cut(startPos, endPos - 1), startPos , endPos - 1 };
}

StringParsingData KGameString::ReversTrimToTrimFind(size_t _Offset)
{
	size_t endPos = ReverseTrimPos(_Offset);
	size_t startPos = FindReverseTrimPos(endPos);
	return { Cut(startPos, endPos), startPos, endPos };
}


std::list<StringParsingData> KGameString::ReversFindAllPosition(const KGameString& _Text, size_t _Offset)
{
	std::list<StringParsingData> m_Data;

	size_t curSize = _Offset;

	while (true)
	{
		size_t FindSize = m_String.rfind(_Text.m_String, curSize - 1);
		if (FindSize == std::string::npos)
		{
			return m_Data;
		}
		else
		{
			curSize = FindSize;
			m_Data.push_back({ _Text, curSize, curSize + _Text.Size() });
		}
	}
}
std::list<StringParsingData> KGameString::FindAllPosition(const KGameString& _Text, size_t _Offset)
{
	std::list<StringParsingData> m_Data;

	size_t CurSize = _Offset;

	while (true)
	{
		size_t FindSize = m_String.find(_Text.m_String, CurSize + 1);
		if (FindSize == std::string::npos)
		{
			return m_Data;
		}
		else
		{
			CurSize = FindSize;
			m_Data.push_back({ _Text, CurSize, CurSize + _Text.Size() });
		}
	}

}

std::list<StringParsingData> KGameString::FindAllPosition(const KGameString& _Text)
{
	return FindAllPosition(_Text, 0);
}



void KGameString::AppendfInternal(const char* fmt, ...)
{
	int BufferSize = 512;
	char StartingBuffer[512];
	char* Buffer = StartingBuffer;

	// 스택에 할당한 크기로 처리할 수있는지 확인
	va_list ap;
	va_start(ap, fmt);
	int Result = vsprintf_s(Buffer, BufferSize, fmt, ap);
	va_end(ap);

	// 실패하면 메모리에서 할당한다.
	if (Result >= BufferSize)
	{
		Buffer = nullptr;
		while (Result == -1)
		{
			BufferSize *= 2;
			Buffer = (char*)malloc(BufferSize * sizeof(char));
			va_start(ap, fmt);
			Result = vsprintf_s(Buffer, BufferSize, fmt, ap);
			va_end(ap);
		};
	}

	if (nullptr == Buffer)
	{
		return;
	}

	Buffer[Result] = 0;

	m_String += CA2W(Buffer).m_psz;

	if (BufferSize != 512)
	{
		free(Buffer);
	}
}

void KGameString::AppendfInternal(const wchar_t* fmt, ...)
{
	int BufferSize = 512;
	wchar_t	StartingBuffer[512];
	wchar_t* Buffer = StartingBuffer;

	// 스택에 할당한 크기로 처리할 수있는지 확인
	va_list ap;
	va_start(ap, fmt);
	int Result = vswprintf_s(Buffer, BufferSize, fmt, ap);
	va_end(ap);

	// 실패하면 메모리에서 할당한다.
	if (Result >= BufferSize)
	{
		Buffer = nullptr;
		while (Result == -1)
		{
			BufferSize *= 2;
			Buffer = (wchar_t*)malloc(BufferSize * sizeof(wchar_t));
			va_start(ap, fmt);
			Result = vswprintf_s(Buffer, BufferSize, fmt, ap);
			va_end(ap);
		};
	}

	if (nullptr == Buffer)
	{
		return;
	}

	Buffer[Result] = 0;

	m_String += Buffer;

	if (BufferSize != 512)
	{
		free(Buffer);
	}
}
