#pragma once
#include "KGameResource.h"
#include "K3DBufferBase.h"
#include "KShader.h"
#include "KCuttingRender.h"

class KRenderManager;
class KConstantBuffer : public KGameResource<KConstantBuffer>, public K3DBufferBase
{
public:
	template<typename DATA>
	static void Create(const KGameString& _Name, SHADER_TYPE _eType, unsigned int _Reg, D3D11_USAGE _VTXUSAGE = D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		KConstantBuffer* NewRest = new KConstantBuffer();
		NewRest->SetName(_Name);
		NewRest->DataTypeSetting<DATA>();
		NewRest->CreateCB(sizeof(DATA), _eType, _Reg, _VTXUSAGE);
		NewRest->InsertResource();
	}

public:
	friend KRenderManager;
	friend KRenderer;

private:
	unsigned int uiReg;
	SHADER_TYPE m_eType;
	const type_info* m_DataInfo;

	D3D11_MAPPED_SUBRESOURCE m_Sub;

	void (KConstantBuffer::* m_UpdateFunc)();

public:
	unsigned int Reg() {
		return uiReg;
	}

	void Reg(unsigned int _uiReg)
	{
		uiReg = _uiReg;
	}

	void ShaderType(SHADER_TYPE _Type);

public:
	template<typename T>
	void ValueSetting(const T& _Data)
	{
		if (m_DataInfo != &typeid(_Data))
		{
			assert(false);
		}

		KGameDevice::MainContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_Sub);

		memcpy_s(m_Sub.pData, m_BufferDesc.ByteWidth, &_Data, m_BufferDesc.ByteWidth);


		KGameDevice::MainContext()->Unmap(m_pBuffer, 0);

	}

	void ValueSetting(const type_info* _Info, const void* _pData)
	{
		if (m_DataInfo != _Info)
		{
			assert(false);
		}

		KGameDevice::MainContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_Sub);

		memcpy_s(m_Sub.pData, m_BufferDesc.ByteWidth, _pData, m_BufferDesc.ByteWidth);

		KGameDevice::MainContext()->Unmap(m_pBuffer, 0);

	}


private:
	template<typename T>
	void DataTypeSetting()
	{
		m_DataInfo = &typeid(T);
	}

private:
	void VSetting();
	void HSetting();
	void DSetting();
	void GSetting();
	void PSetting();


public:
	void Setting();

public:
	void CreateCB(size_t _Size, SHADER_TYPE _eType, unsigned int _Reg, D3D11_USAGE _VTXUSAGE);
};
