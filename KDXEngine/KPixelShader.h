#pragma once
#include "KGameResource.h"
#include "KShader.h"

class KPixelShader : public KGameResourceBase<KPixelShader>, public KShader
{
	friend KShader;
public:
	static KPTR<KPixelShader> Load(const KGameString& _Path, const KGameString& _FuncName, unsigned int _VH, unsigned int _VL)
	{
		KPTR<KPixelShader> NewShader = new KPixelShader();
		NewShader->m_Path = _Path;
		NewShader->SetName(_FuncName);
		NewShader->Load(_FuncName, _VH, _VL);
		NewShader->InsertResource();

		return NewShader;
	}

private:
	bool m_IsForward;
	ID3D11PixelShader* m_ShaderPtr;
	std::string m_Name;

public:
	bool IsForward() const
	{
		return m_IsForward;
	}

public:
	void Load(const KGameString& _FuncName, unsigned int _VH, unsigned int _VL);

public:
	void Setting();

private:
	KPixelShader() : m_ShaderPtr(nullptr), m_IsForward(true)
	{
		m_Type = SHADER_TYPE::ST_PS;
	}

	~KPixelShader()
	{
		if (nullptr != m_ShaderPtr)
		{
			m_ShaderPtr->Release();
			m_ShaderPtr = nullptr;
		}
	}
};

