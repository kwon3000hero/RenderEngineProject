#pragma once
#include "KGAMEDEVICE.h"
#include <KGAMEDEBUG.h>
#include <map>

enum class SHADER_TYPE
{
	ST_VS,
	ST_HS,
	ST_DS,
	ST_GS,
	ST_PS,
	ST_MAX,
};

enum class SHADERRES_TYPE
{
	SR_SAMPLER,
	SR_TEXTURE,
	SR_CBUFFER,
	SR_MAX,
};

class ShaderParsingData
{
public:
	KGameString FuncName;
	SHADER_TYPE ShaderType;
};

class KShaderData
{
public:
	const type_info* m_Info; // ��������� ��� �����е��� � �������� �װ͵��� �ʹ� ���Ƽ�
	size_t m_Size; // ������� ����
	SHADER_TYPE SHADERTYPE; // �ȼ����̴���
	SHADERRES_TYPE SHADERRESTYPE; // �ؽ�ó
	unsigned int REG; // 0��°
};

class KRenderManager;
class KRenderer;
class KShader//: public KGameReference
{
public:
	friend KRenderManager;
	friend KRenderer;

protected:
	std::map<KGameString, KShaderData> m_ShaderData;

private:
	void InsertShaderData(const KGameString& _NAME, const KShaderData& _DATA)
	{
		if (m_ShaderData.end() != m_ShaderData.find(_NAME))
		{
			AssertMsg(L"�̹� ������� �������Դϴ� = " + _NAME);
		}

		m_ShaderData[_NAME] = _DATA;
	}

public:
	void SetTexture(const KGameString& _NAME, unsigned int _Reg);
	void SetSampler(const KGameString& _NAME, unsigned int _Reg);

	template<typename T>
	void CUBFFERDATA(const KGameString& _NAME, unsigned int _Reg)
	{
		InsertShaderData(_NAME, KShaderData({ &typeid(T), sizeof(T), m_Type, SHADERRES_TYPE::SR_CBUFFER,_Reg }));
	}

	static void ParsingToShaderData(const KGameString& _path);
	void SetAutoShaderResource(const KGameString& _shaderName);

protected:
	ID3DBlob* m_Blob;
	ID3DBlob* m_ErrBlob;
	unsigned int m_highVersion;
	unsigned int m_lowVersion;
	SHADER_TYPE m_Type;

public:
	virtual void Setting() {}

public:
	KShader() : m_Blob(nullptr), m_ErrBlob(nullptr), m_Type(SHADER_TYPE::ST_MAX), m_highVersion(0), m_lowVersion(0)
	{
	}
	virtual ~KShader()
	{
		if (nullptr != m_Blob)
		{
			m_Blob->Release();
			m_Blob = nullptr;
		}

		if (nullptr != m_ErrBlob)
		{
			m_ErrBlob->Release();
			m_ErrBlob = nullptr;
		}
	}
};

