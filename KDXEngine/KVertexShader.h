#pragma once
#include "KGameResource.h"
#include "KShader.h"
#include <KGAMESTRING.h>
#include <KGAMEFILE.h>

class KRenderManager;
class KVertexShader : public KGameResource<KVertexShader>, public KShader
{
public:
	friend KRenderManager;

public:
	static KPTR<KVertexShader> Load(const KGameString& _path, const KGameString& _funcName, unsigned int _VH, unsigned int _VL)
	{
		KVertexShader* pNewShader = new KVertexShader();
		pNewShader->m_Path = _path;
		pNewShader->SetName(_funcName);
		pNewShader->Load(_funcName, _VH, _VL);
		pNewShader->InsertResource();

		return pNewShader;
	}

public:
	class KInputLayout : public KGameReference
	{
		friend KVertexShader;
	private:
		KVertexShader* m_pParent;
		UINT m_Offset;
		ID3D11InputLayout* m_LayOut;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_ArrInputLayOutData;

	public:
		void AddLayout(
			const char* _semanticName,
			unsigned int _index,
			DXGI_FORMAT _format,
			unsigned int _inputSlot,
			unsigned int _instDataStepRate = 0,
			D3D11_INPUT_CLASSIFICATION _InputClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);

		void EndLayout(
			const char* _semanticName,
			unsigned int _index,
			DXGI_FORMAT _format,
			unsigned int _inputSlot,
			unsigned int _instDataStepRate = 0,
			D3D11_INPUT_CLASSIFICATION _InputClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);

		void CreateLayout();

	public:
		KInputLayout() : m_Offset(0), m_LayOut(nullptr), m_pParent(nullptr)
		{

		}

		~KInputLayout()
		{
			if (nullptr != m_LayOut)
			{
				m_LayOut->Release();
				m_LayOut = nullptr;
			}

		}


	};

	friend KInputLayout;


private:
	ID3D11VertexShader* m_ShaderPtr;

public:
	KInputLayout InputLayOut;

private:
	void Load(const KGameString& _FuncName, unsigned int _VH, unsigned int _VL);
	void CheckInputLayout(const KGameString& _shaderName);

public:
	void Setting() override;

public:
	KVertexShader() : m_ShaderPtr(nullptr)
	{
		m_Type = SHADER_TYPE::ST_VS;
	}

	~KVertexShader()
	{
		if (nullptr != m_ShaderPtr)
		{
			m_ShaderPtr->Release();
			m_ShaderPtr = nullptr;
		}
	}

};

