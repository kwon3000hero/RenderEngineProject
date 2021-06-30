#include <d3d11.h>

inline int SafeRelease(IUnknown* pReference)
{
	HRESULT result = S_OK;
	if (nullptr != pReference)
	{
		result = pReference->Release();
		pReference = nullptr;
	}

	return result;
}
