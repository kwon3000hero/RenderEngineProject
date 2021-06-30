#pragma once
#include "KGameResource.h"
#include <vector>

class KCuttingTexture : public KGameResource<KCuttingTexture>
{
public:
	static void Create(const KGameString& _TexName, int _W, int _H)
	{
		KCuttingTexture* NewRes = new KCuttingTexture();
		NewRes->m_Path = _TexName;
		NewRes->SetName(_TexName);
		NewRes->Create(_W, _H);
		NewRes->InsertResource();
	}

	static void Create(const KGameString& _TexName, const KVector& _Start, const KVector& _Size, int _W, int _H)
	{
		KCuttingTexture* NewRes = new KCuttingTexture();
		NewRes->m_Path = _TexName;
		NewRes->SetName(_TexName);
		NewRes->Create(_Start, _Size, _W, _H);
		NewRes->InsertResource();
	}

private:
	std::vector<KVector> m_CutData;
	KVector m_PixelUvSize;

public:
	const KVector& PixelUvSize()
	{
		return m_PixelUvSize;
	}

public:
	const KVector& CutData(size_t _Index)
	{
		return m_CutData[_Index];
	}


public:
	void Create(int _W, int _H);
	void Create(const KVector& _Start, const KVector& _Size, int _W, int _H);

};

