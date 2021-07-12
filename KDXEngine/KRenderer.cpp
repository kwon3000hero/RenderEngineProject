#include "KRendererManager.h"
#include "KRenderer.h"
#include "KMESH.h"
#include "KShader.h"

#include "KTransform.h"

#include "KSAMPLER.h"
#include "KTEXTURE.h"
#include "KVertexShader.h"
#include "KPixelShader.h"
#include "KConstantBuffer.h"
#include "KGAMEINPUT.h"

#include "KRENDERPIPELINE.h"
#include "KFBX.h"

void KRenderer::Init(const KGameString& _MeshName, const KGameString& _RPName)
{
	SetMesh(_MeshName);
	SetRenderPipeline(_RPName);
}

bool KRenderer::IsExistTexture(const KGameString& _name)
{
	for (size_t i = 0; i < m_TextureContainer.size(); i++)
	{
		for (auto& _Setter : m_TextureContainer[i])
		{
			if (_Setter.first == _name)
			{
				return true;
			}
		}
	}

	return false;
}

bool KRenderer::IsExistSampler(const KGameString& _name)
{
	for (size_t i = 0; i < m_SamplerContainer.size(); i++)
	{
		for (auto& _Setter : m_SamplerContainer[i])
		{
			if (_Setter.first == _name)
			{
				return true;
			}
		}
	}

	return false;
}

bool KRenderer::IsExistContantBuffer(const KGameString& _name)
{
	for (size_t i = 0; i < m_ConstantBufferContainer.size(); i++)
	{
		for (auto& _Setter : m_ConstantBufferContainer[i])
		{
			if (_Setter.first == _name)
			{
				return true;
			}
		}
	}

	return false;
}

std::list<KRenderer::KTextureSetter*> KRenderer::TextureSetterList(const KGameString& _name)
{
	std::list<KRenderer::KTextureSetter*> List;

	for (size_t i = 0; i < m_TextureContainer.size(); i++)
	{
		for (auto& _Setter : m_TextureContainer[i])
		{
			if (_Setter.first == _name)
			{
				List.push_back(&_Setter.second);
			}
		}
	}

	return List;
}

std::list<KRenderer::KSamplerSetter*> KRenderer::SamplerSetterList(const KGameString& _name)
{
	std::list<KRenderer::KSamplerSetter*> List;

	for (size_t i = 0; i < m_SamplerContainer.size(); i++)
	{
		for (auto& _Setter : m_SamplerContainer[i])
		{
			if (_Setter.first == _name)
			{
				List.push_back(&_Setter.second);
			}
		}
	}

	return List;
}

std::list<KRenderer::KConstantBufferSetter*> KRenderer::ConstantBufferSetterList(const KGameString& _name)
{
	std::list<KRenderer::KConstantBufferSetter*> List;

	for (size_t i = 0; i < m_ConstantBufferContainer.size(); i++)
	{
		for (auto& _Setter : m_ConstantBufferContainer[i])
		{
			if (_Setter.first == _name)
			{
				List.push_back(&_Setter.second);
			}
		}
	}

	return List;
}

void KRenderer::SetConstantBuffer(const KGameString& _name, void* pData, ConstantBufferMode _Mode)
{
	if (false == IsExistContantBuffer(_name))
	{
		AssertMsg(L"쉐이더에 세팅할 상수버퍼중 " + _name + "은 존재하지 않습니다..");
	}

	for (size_t i = 0; i < m_ConstantBufferContainer.size(); ++i)
	{
		if (m_ConstantBufferContainer[i].end() != m_ConstantBufferContainer[i].find(_name))
		{
			m_ConstantBufferContainer[i][_name].m_Mode = _Mode;

			switch (_Mode)
			{
			case ConstantBufferMode::Link:
				m_ConstantBufferContainer[i][_name].m_pSettingData = pData;
				break;
			case ConstantBufferMode::New:
			{
				if (nullptr == m_ConstantBufferContainer[i][_name].m_pSettingData)
				{
					m_ConstantBufferContainer[i][_name].m_pSettingData = new char[m_ConstantBufferContainer[i][_name].m_pShaderData->m_Size];
				}
				memcpy_s(m_ConstantBufferContainer[i][_name].m_pSettingData, m_ConstantBufferContainer[i][_name].m_pShaderData->m_Size, pData, m_ConstantBufferContainer[i][_name].m_pShaderData->m_Size);
				break;
			}
			default:
				break;
			}
		}
	}
}

void KRenderer::SetSampler(const KGameString& _name, const KGameString& _samplerName)
{
	if (false == IsExistSampler(_name))
	{
		AssertMsg(L"존재하지 않는 샘플러 세팅입니다 : " + _samplerName);
	}

	for (size_t i = 0; i < m_SamplerContainer.size(); ++i)
	{
		if (m_SamplerContainer[i].end() != m_SamplerContainer[i].find(_name))
		{
			KPTR<KSampler> finded = KGameResourceManager<KSampler>::Instance().Find(_samplerName);
			m_SamplerContainer[i][_name].m_Sampler = finded;
		}
		else
		{
			int a = 0;
		}
	}
}

void KRenderer::SetTexture(const KGameString& _name, const KPTR<KTexture>& _texture)
{
	if (nullptr == _texture.get())
	{
		AssertMsg(L"존재하지 않는 텍스처 세팅입니다 : " + _name);
	}

	for (size_t i = 0; i < m_TextureContainer.size(); ++i)
	{
		if (m_TextureContainer[i].end() != m_TextureContainer[i].find(_name))
		{
			m_TextureContainer[i][_name].m_Texture = _texture;
		}
	}
}

void KRenderer::SetTexture(const KGameString& _name, const KGameString& _textureName)
{
	if (false == IsExistTexture(_name))
	{
		AssertMsg(L"존재하지 않는 텍스처 세팅입니다 1: " + _name);
	}

	KPTR<KTexture> Res = KGameResourceManager<KTexture>::Instance().Find(_textureName);

	for (size_t i = 0; i < m_TextureContainer.size(); i++)
	{
		if (m_TextureContainer[i].end() != m_TextureContainer[i].find(_name))
		{
			m_TextureContainer[i][_name].m_Texture = Res;
		}
	}
}

void KRenderer::SetRenderConstantBuffer(const KGameString& _name)
{
	for (size_t i = 0; i < m_ConstantBufferContainer.size(); i++)
	{
		std::map<KGameString, KConstantBufferSetter>::iterator StartIter = m_ConstantBufferContainer[i].find(_name);

		if (m_ConstantBufferContainer[i].end() == StartIter)
		{
			continue;
		}

		StartIter->second.m_ConstantBuffer->ValueSetting(StartIter->second.m_pShaderData->m_Info, StartIter->second.m_pSettingData);
		StartIter->second.m_ConstantBuffer->Setting();
	}
}

void KRenderer::SetRenderSampler(const KGameString& _name)
{
	for (size_t i = 0; i < m_SamplerContainer.size(); i++)
	{
		std::map<KGameString, KSamplerSetter>::iterator StartIter = m_SamplerContainer[i].find(_name);

		if (m_SamplerContainer[i].end() == StartIter)
		{
			continue;
		}

		StartIter->second.m_Sampler->Setting(StartIter->second.m_Data->SHADERTYPE, StartIter->second.m_Data->REG);
	}
}

void KRenderer::SetRenderTexture(const KGameString& _name)
{
	for (size_t i = 0; i < m_TextureContainer.size(); i++)
	{
		std::map<KGameString, KTextureSetter>::iterator StartIter = m_TextureContainer[i].find(_name);

		if (m_TextureContainer[i].end() == StartIter)
		{
			continue;
		}

		StartIter->second.m_Texture->Setting(StartIter->second.m_Data->SHADERTYPE, StartIter->second.m_Data->REG);
	}
}

void KRenderer::SetterInit()
{
	m_TextureContainer.resize((int)SHADER_TYPE::ST_MAX);
	m_SamplerContainer.resize((int)SHADER_TYPE::ST_MAX);
	m_ConstantBufferContainer.resize((int)SHADER_TYPE::ST_MAX);
}

void KRenderer::ShadowOn()
{
	m_Parent->m_ShadowRenderPlayerContainer.push_back(KPTR<KRenderer>(this));
}

void KRenderer::ShadowRender()
{
	SetRenderConstantBuffer(L"TRANSFORMDATA");
	if (false == m_RenderOption[static_cast<int>(RenderOption::Animation)])
	{
		KPTR<KRenderPipeline> RPL = KGameResourceManager<KRenderPipeline>::Instance().Find(L"SHADOW");
		RPL->Setting();
	}
	else
	{
		SetRenderTexture(L"FrameAnimationTexture");
		KPTR<KRenderPipeline> RPL = KGameResourceManager<KRenderPipeline>::Instance().Find(L"SHADOWANI");
		RPL->Setting();
	}

	m_mesh->Render(m_MeshRenderType);
}

void KRenderer::SetMesh(const KGameString& _Name)
{
	m_mesh = KGameResourceManager<KMesh>::Instance().Find(_Name);
	if (nullptr == m_mesh.get())
	{
		AssertMsg(L"매쉬가 존재하지 않습니다. =" + _Name);
	}

}

void KRenderer::SetMesh(const KPTR<KMesh>& _mesh)
{
	if (nullptr == _mesh.get())
	{
		AssertMsg(L"없는 mesh 입니다.");
	}

	m_mesh = _mesh;
}

void KRenderer::SetRenderPipeline(const KGameString& _Name)
{
	m_CurrentRenderPipeline = KGameResourceManager<KRenderPipeline>::Instance().Find(_Name);

	if (nullptr == m_CurrentRenderPipeline.get())
	{
		AssertMsg(L"랜더링 파이프라인이 존재하지 않습니다 = " + _Name);
	}

	auto& AllShader = m_CurrentRenderPipeline->ShaderContainer();

	for (size_t i = 0; i < AllShader.size(); i++)
	{
		for (auto& ShaderData : AllShader[i]->m_ShaderData)
		{
			switch (ShaderData.second.SHADERRESTYPE)
			{
			case SHADERRES_TYPE::SR_TEXTURE:
				m_TextureContainer[(int)ShaderData.second.SHADERRESTYPE].insert(std::map<KGameString, KRenderer::KTextureSetter>::value_type(ShaderData.first, KRenderer::KTextureSetter(&ShaderData.second, static_cast<KPTR<KTexture>>(0))));
				break;
			case SHADERRES_TYPE::SR_SAMPLER:
				m_SamplerContainer[(int)ShaderData.second.SHADERRESTYPE].insert(std::map<KGameString, KRenderer::KSamplerSetter>::value_type(ShaderData.first, KRenderer::KSamplerSetter(&ShaderData.second, static_cast<KPTR<KSampler>>(0))));
				break;
			case SHADERRES_TYPE::SR_CBUFFER:
				m_ConstantBufferContainer[(int)ShaderData.second.SHADERRESTYPE].insert(std::map<KGameString, KRenderer::KConstantBufferSetter>::value_type(ShaderData.first, KRenderer::KConstantBufferSetter(ShaderData.first, &ShaderData.second, static_cast<KPTR<KConstantBuffer>>(0))));

				m_ConstantBufferContainer[(int)ShaderData.second.SHADERRESTYPE][ShaderData.first].m_ConstantBuffer = make_KPTR<KConstantBuffer>();
				m_ConstantBufferContainer[(int)ShaderData.second.SHADERRESTYPE][ShaderData.first].m_ConstantBuffer->m_DataInfo = ShaderData.second.m_Info;
				m_ConstantBufferContainer[(int)ShaderData.second.SHADERRESTYPE][ShaderData.first].m_ConstantBuffer->CreateCB(
					m_ConstantBufferContainer[(int)ShaderData.second.SHADERRESTYPE][ShaderData.first].m_pShaderData->m_Size,
					m_ConstantBufferContainer[(int)ShaderData.second.SHADERRESTYPE][ShaderData.first].m_pShaderData->SHADERTYPE,
					m_ConstantBufferContainer[(int)ShaderData.second.SHADERRESTYPE][ShaderData.first].m_pShaderData->REG,
					D3D11_USAGE::D3D11_USAGE_DYNAMIC);

				if (nullptr != m_Parent && L"TRANSFORMDATA" == ShaderData.first)
				{
					SetConstantBuffer(ShaderData.first, (void*)&(m_Parent->m_Matrix), ConstantBufferMode::Link);
				}

				if (nullptr != m_Parent && L"LIGHTDATABUFFER" == ShaderData.first)
				{
					LightDataBuffer* LightData = m_Parent->Actor()->Scene()->m_LightDataBufferContainer[m_Parent->Order()];
					SetConstantBuffer(ShaderData.first, (void*)LightData, ConstantBufferMode::Link);
				}

				if (L"RenderOption" == ShaderData.first)
				{
					memset(m_RenderOption, 0, sizeof(bool) * 4);
					SetConstantBuffer(ShaderData.first, (void*)m_RenderOption, ConstantBufferMode::Link);
				}
				break;
			default:
				break;
			}
		}
	}
}

KRenderer::KRenderer() : m_MeshRenderType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), m_mesh(nullptr), m_CurrentRenderPipeline(nullptr)
{
	SetterInit();
}

KRenderer::KRenderer(const KGameString& _meshName, const KGameString& _renderPipelineName) : m_MeshRenderType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), m_mesh(nullptr), m_CurrentRenderPipeline(nullptr)
{
	SetterInit();
	Init(_meshName, _renderPipelineName);
}
void KRenderer::Render()
{
	for (int i = 0; i < m_TextureContainer.size(); ++i)
	{
		for (auto& Setter : m_TextureContainer[i])
		{
			if (nullptr == Setter.second.m_Texture.get())
			{
				if (true == Setter.second.m_IsExistTexture)
				{
					continue;
				}

				AssertMsg(L"랜더링을 위한 텍스처를 세팅해주지 않았습니다. = " + Setter.first);
			}

			Setter.second.m_Texture->Setting(Setter.second.m_Data->SHADERTYPE, Setter.second.m_Data->REG);
		}
	}

	for (int i = 0; i < m_SamplerContainer.size(); ++i)
	{
		for (auto& Setter : m_SamplerContainer[i])
		{
			if (nullptr == Setter.second.m_Sampler.get())
			{
				AssertMsg(L"랜더링을 위한 샘플러를 세팅해주지 않았습니다. = " + Setter.first);
			}

			Setter.second.m_Sampler->Setting(Setter.second.m_Data->SHADERTYPE, Setter.second.m_Data->REG);
		}
	}

	for (int i = 0; i < m_ConstantBufferContainer.size(); ++i)
	{
		for (auto& Setter : m_ConstantBufferContainer[i])
		{
			if (nullptr == Setter.second.m_ConstantBuffer.get())
			{
				AssertMsg(L"랜더링을 위한 상수버퍼가 만들어지지 않았습니다. = " + Setter.first);
			}

			if (nullptr == Setter.second.m_pSettingData)
			{
				AssertMsg(L"랜더링을 위한 상수버퍼를 세팅해주지 않았습니다. = " + Setter.first);
			}

			Setter.second.m_ConstantBuffer->ValueSetting(Setter.second.m_pShaderData->m_Info, Setter.second.m_pSettingData);
			Setter.second.m_ConstantBuffer->Setting();
		}
	}

	m_CurrentRenderPipeline->Setting();
	m_mesh->Render(m_MeshRenderType);
}

KPTR<KRenderer> KRenderManager::CreateRenderPlayer(const KGameString& _MeshName, const KGameString& _RenderPipeLineName)
{
	m_RenderPlayerContainer.resize(m_RenderPlayerContainer.size() + 1);
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1] = make_KPTR<KRenderer>();
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->m_Parent = this;
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->SetMesh(_MeshName);
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->SetRenderPipeline(_RenderPipeLineName);
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->m_Name = _MeshName;

	if (true == m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->m_CurrentRenderPipeline->PixelShader()->IsForward())
	{
		m_ForwardRenderPlayerContainer.push_back(m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]);
	}
	else
	{
		m_DefferdRenderPlayerContainer.push_back(m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]);
	}

	return m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1];
}

KPTR<KRenderer> KRenderManager::CreateRenderPlayer(const KPTR<KMesh>& _mesh, const KGameString& _RenderPipeLineName)
{
	m_RenderPlayerContainer.resize(m_RenderPlayerContainer.size() + 1);
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1] = make_KPTR<KRenderer>();
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->m_Parent = this;
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->SetMesh(_mesh);
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->SetRenderPipeline(_RenderPipeLineName);
	m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->m_Name = L"unknown";

	if (true == m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]->m_CurrentRenderPipeline->PixelShader()->IsForward())
	{
		m_ForwardRenderPlayerContainer.push_back(m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]);
	}
	else
	{
		m_DefferdRenderPlayerContainer.push_back(m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1]);
	}

	return m_RenderPlayerContainer[m_RenderPlayerContainer.size() - 1];
}

std::vector<KPTR<KRenderer>> KRenderManager::CreateRenderPlayerToFbx(const KGAMEDIR& _dir, const KGameString& _FbxName, const KGameString& _MaterialName, const KGameString& _SamplerName)
{
	std::vector<KPTR<KRenderer>> renderPlayerList;

	KPTR<KFBX> fbx = KGameResourceManager<KFBX>::Instance().Find(_FbxName);

	size_t meshFbxSize = fbx->m_MeshContainer.size();
	for (size_t i = 0; i < meshFbxSize; ++i)
	{
		KPTR<KRenderer> renderPlayer = CreateRenderPlayer(fbx->m_MeshContainer[i], _MaterialName);

		KGameString diffuseTextureName = KGAMEPATH::GetFileName(fbx->m_UserMaterialDataContainer[i].DiffuseTexture);

		KPTR<KTexture> diffuseTexture = KGameResourceManager<KTexture>::Instance().Find(diffuseTextureName);
		if (nullptr == diffuseTexture.get())
		{
			KTexture::Load(_dir.m_Path + "\\" + diffuseTextureName);
			diffuseTexture = KGameResourceManager<KTexture>::Instance().Find(diffuseTextureName);
		}

		KGameString specularTextureName = KGAMEPATH::GetFileName(fbx->m_UserMaterialDataContainer[i].SpecularTexture);
		KPTR<KTexture> specularTexture = KGameResourceManager<KTexture>::Instance().Find(specularTextureName);
		if (nullptr == specularTexture.get())
		{
			KTexture::Load(_dir.m_Path + "\\" + specularTextureName);
			specularTexture = KGameResourceManager<KTexture>::Instance().Find(specularTextureName);
		}

		KGameString normalTextureName = KGAMEPATH::GetFileName(fbx->m_UserMaterialDataContainer[i].NormalTexture);
		KPTR<KTexture> normalTexture = KGameResourceManager<KTexture>::Instance().Find(normalTextureName);
		if (nullptr == normalTexture.get())
		{
			KTexture::Load(_dir.m_Path + "\\" + normalTextureName);
			normalTexture = KGameResourceManager<KTexture>::Instance().Find(normalTextureName);
		}

		{
			std::list<KRenderer::KTextureSetter*> List = m_RenderPlayerContainer[i]->TextureSetterList(L"EmiTex");
			for (auto Setter : List)
			{
				Setter->m_IsExistTexture = true;
			}
		}

		{
			std::list<KRenderer::KTextureSetter*> List = m_RenderPlayerContainer[i]->TextureSetterList(L"SpcTex");
			for (auto Setter : List)
			{
				Setter->m_IsExistTexture = true;
			}
		}

		{
			std::list<KRenderer::KTextureSetter*> List = renderPlayer->TextureSetterList(L"NormalTex");
			for (auto Setter : List)
			{
				Setter->m_IsExistTexture = true;
			}
		}


		if (true == renderPlayer->IsExistTexture(L"DifTex"))
		{
			renderPlayer->SetTexture(L"DifTex", diffuseTextureName);
		}

		renderPlayerList.push_back(renderPlayer);

		if (true == renderPlayer->IsExistTexture(L"SpcTex"))
		{
			if (nullptr != specularTexture.get())
			{
				renderPlayer->m_RenderOption[static_cast<int>(RenderOption::Specular)] = true;
				renderPlayer->SetTexture(L"SpcTex", specularTextureName);
			}
		}

		if (true == renderPlayer->IsExistTexture(L"NormalTex"))
		{
			if (nullptr != normalTexture.get())
			{
				renderPlayer->m_RenderOption[static_cast<int>(RenderOption::Normal)] = true;
				renderPlayer->SetTexture(L"NormalTex", normalTextureName);
			}
		}

		if (true == renderPlayer->IsExistSampler(L"Smp"))
		{
			renderPlayer->SetSampler(L"Smp", _SamplerName);
		}

		if (0 >= fbx->m_UserAnimationContainer.size())
		{
			renderPlayer->m_RenderOption[static_cast<int>(RenderOption::Animation)] = false;
			std::list<KRenderer::KTextureSetter*> list = renderPlayer->TextureSetterList(L"FrameAnimationTexture");
			for (auto setter : list)
			{
				setter->m_IsExistTexture = true;
			}
		}


	}

	return renderPlayerList;
}