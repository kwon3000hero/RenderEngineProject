#include "KShader.h"
#include <KGAMEFILE.h>
#include <KGAMESTRING.h>
#include "KVertexShader.h"
#include "KPixelShader.h"

void KShader::SetTexture(const KGameString& _NAME, unsigned int _Reg)
{
	InsertShaderData(_NAME, KShaderData({ nullptr, 0, m_Type, SHADERRES_TYPE::SR_TEXTURE,_Reg }));
}
void KShader::SetSampler(const KGameString& _NAME, unsigned int _Reg)
{
	InsertShaderData(_NAME, KShaderData({ nullptr, 0, m_Type, SHADERRES_TYPE::SR_SAMPLER,_Reg }));
}

void KShader::ParsingToShaderData(const KGameString& _path)
{
	KGameFile file = KGameFile(_path, L"rt");

	KGameString ShaderInfo = file.AllText();

	std::list<StringParsingData> AllPos = ShaderInfo.FindAllPosition(L"(");

	std::list<StringParsingData> NameList;
	std::list<StringParsingData> ParaList;


	for (auto& _Value : AllPos)
	{
		NameList.push_back(ShaderInfo.ReversTrimToTrimFind(_Value.StartPos - 1));
		ParaList.push_back(ShaderInfo.TrimToTrimFind(_Value.StartPos + 1));
	}

	{
		std::list<StringParsingData>::iterator NameStartIter = NameList.begin();
		std::list<StringParsingData>::iterator NameEndIter = NameList.end();
		std::list<StringParsingData>::iterator ParaStartIter = ParaList.begin();
		std::list<StringParsingData>::iterator ParaEndIter = ParaList.end();

		//Ư�� ���� ����... ���� �ʿ�.
		for (; NameStartIter != NameEndIter;)
		{
			if (
				(std::wstring::npos != NameStartIter->Name.FindPos(L"(", 0))
				||
				(std::wstring::npos != ParaStartIter->Name.FindPos(L"(", 0)))
			{
				NameStartIter = NameList.erase(NameStartIter);
				ParaStartIter = ParaList.erase(ParaStartIter);
			}			
			else if
				(
					(std::wstring::npos != NameStartIter->Name.FindPos(L")", 0))
					||
					(std::wstring::npos != ParaStartIter->Name.FindPos(L")", 0))
					)
			{
				NameStartIter = NameList.erase(NameStartIter);
				ParaStartIter = ParaList.erase(ParaStartIter);
			}
			else if
				(
					(std::wstring::npos != NameStartIter->Name.FindPos(L"\\", 0))
					||
					(std::wstring::npos != ParaStartIter->Name.FindPos(L"\\", 0))
					)
			{
				NameStartIter = NameList.erase(NameStartIter);
				ParaStartIter = ParaList.erase(ParaStartIter);
			}
			else if
				(
					(std::wstring::npos != NameStartIter->Name.FindPos(L"\t", 0))
					||
					(std::wstring::npos != ParaStartIter->Name.FindPos(L"\t", 0))
					)
			{
				NameStartIter = NameList.erase(NameStartIter);
				ParaStartIter = ParaList.erase(ParaStartIter);
			}

			else if
				(
					(std::wstring::npos != NameStartIter->Name.FindPos(L"/", 0))
					||
					(std::wstring::npos != ParaStartIter->Name.FindPos(L"/", 0))
					)
			{
				NameStartIter = NameList.erase(NameStartIter);
				ParaStartIter = ParaList.erase(ParaStartIter);
			}

			else if
				(
					(std::wstring::npos != NameStartIter->Name.FindPos(L",", 0))
					||
					(std::wstring::npos != ParaStartIter->Name.FindPos(L",", 0))
					)
			{
				NameStartIter = NameList.erase(NameStartIter);
				ParaStartIter = ParaList.erase(ParaStartIter);
			}

			else if
				(
					(std::wstring::npos != NameStartIter->Name.FindPos(L".", 0))
					||
					(std::wstring::npos != ParaStartIter->Name.FindPos(L".", 0))
					)
			{
				NameStartIter = NameList.erase(NameStartIter);
				ParaStartIter = ParaList.erase(ParaStartIter);
			}

			else
			{
				++NameStartIter;
				++ParaStartIter;
			}
		}
	}

	{
		std::list<StringParsingData>::iterator NameStartIter = NameList.begin();
		std::list<StringParsingData>::iterator NameEndIter = NameList.end();
		std::list<StringParsingData>::iterator ParaStartIter = ParaList.begin();
		std::list<StringParsingData>::iterator ParaEndIter = ParaList.end();

		for (; ParaEndIter != ParaStartIter;)
		{
			if (L"register" == (*NameStartIter).Name || L"" == (*NameStartIter).Name)
			{
				NameStartIter = NameList.erase(NameStartIter);
				ParaStartIter = ParaList.erase(ParaStartIter);
			}
			else if (L"float4" == (*ParaStartIter).Name)
			{
				++NameStartIter;
				++ParaStartIter;
			}
			else
			{
				std::list<StringParsingData> FindParaList = ShaderInfo.ReversFindAllPosition(ParaStartIter->Name, ParaStartIter->StartPos);

				if (0 == FindParaList.size())
				{
					if (std::wstring::npos != ParaStartIter->StartPos)
					{
						StringParsingData dataStart = ShaderInfo.FindStringParsingData(":", ParaStartIter->StartPos);
						StringParsingData dataEnd = ShaderInfo.FindStringParsingData(")", ParaStartIter->StartPos);

						KGameString text = ShaderInfo.Cut(dataStart.EndPos, dataEnd.StartPos);

						if (std::wstring::npos != text.ToUpper().FindPos(L"SV_VERTEXID", 0))
						{
							++NameStartIter;
							++ParaStartIter;
						}
						else {
							NameStartIter = NameList.erase(NameStartIter);
							ParaStartIter = ParaList.erase(ParaStartIter);
						}
					}
					else
					{
						NameStartIter = NameList.erase(NameStartIter);
						ParaStartIter = ParaList.erase(ParaStartIter);
					}
				}
				else if (0 != FindParaList.size())
				{
					StringParsingData Data;
					for (auto& _ShaderFind : FindParaList)
					{
						Data = ShaderInfo.ReversTrimToTrimFind(_ShaderFind.StartPos - 1);

						if (L"struct" == Data.Name)
						{
							break;
						}
					}
					if (L"struct" == Data.Name)
					{
						++NameStartIter;
						++ParaStartIter;
					}
					else
					{
						NameStartIter = NameList.erase(NameStartIter);
						ParaStartIter = ParaList.erase(ParaStartIter);
					}
				}
				else
				{
					++NameStartIter;
					++ParaStartIter;
				}
			}
		}
	}

	{
		if (true == NameList.empty())
		{
			AssertMsg(L"�Լ��� �ϳ��� ������� ���� ������ ������ �Ϸ��� �߽��ϴ�.");
			return;
		}

		if (NameList.size() != ParaList.size())
		{
			AssertMsg(L"�Լ��� �Ķ������ ������ �ٸ��ϴ�.");
			return;
		}

		std::list<StringParsingData>::iterator NameStartIter = NameList.begin();
		std::list<StringParsingData>::iterator NameEndIter = NameList.end();
		std::list<StringParsingData>::iterator ParaStartIter = ParaList.begin();
		std::list<StringParsingData>::iterator ParaEndIter = ParaList.end();

		for (; NameStartIter != NameEndIter; ++NameStartIter)
		{
			StringParsingData ReturnData = ShaderInfo.ReversTrimToTrimFind(NameStartIter->StartPos - 1);

			if (L"float4" == ReturnData.Name)
			{
				size_t FindStartPos = ShaderInfo.FindPos(L")", ReturnData.EndPos);
				FindStartPos = ShaderInfo.FindPos(L":", FindStartPos);

				StringParsingData SementicData = ShaderInfo.TrimToTrimFind(FindStartPos + 1);

				if (0 == SementicData.Name.ToUpper().FindPos(L"SV_POSITION", 0))
				{
					KVertexShader::Load(_path, NameStartIter->Name, 5, 0);
					// ���ؽ� ���̴��̴�.
				}
				else if (0 == SementicData.Name.ToUpper().FindPos(L"SV_TARGET", 0))
				{
					KPixelShader::Load(_path, NameStartIter->Name, 5, 0);
					// �ȼ� ���̴� �̴�.
				}
				else
				{
					AssertMsg(L"���̴� Ÿ���� �����Ҽ� ���� ��Ȳ�Դϴ�.   " + SementicData.Name);
				}
			}
			else
			{
				std::list<StringParsingData> FindReturnList = ShaderInfo.ReversFindAllPosition(ReturnData.Name, ReturnData.StartPos);

				if (0 == FindReturnList.size())
				{
					// #include ã�Ƽ� �� ������ �� ��� �� ���ο��� ã�ƺ��� �Ѵ�.
					// #include���� Ÿ���鼭 ��͸� �ؾ� �Ѵ�.

					AssertMsg(L"�������ϱ��� ��� ã�ƺ����ϴ� ��Ȳ�Դϴ�. Ȥ�� �Լ��̸� �м� �����Դϴ�.");
				}
				else if (0 != FindReturnList.size())
				{
					StringParsingData Data;
					bool Check = false;
					for (auto& _ShaderFind : FindReturnList)
					{
						Data = ShaderInfo.ReversTrimToTrimFind(_ShaderFind.StartPos - 1);

						if (L"struct" == Data.Name)
						{
							size_t LastPos = ShaderInfo.FindPos(L"}", Data.EndPos);
							KGameString StructText = ShaderInfo.Cut(Data.EndPos, LastPos);

							if (std::wstring::npos != StructText.ToUpper().FindPos(L"SV_POSITION", 0))
							{
								KVertexShader::Load(_path, NameStartIter->Name, 5, 0);
								// ���ؽ� ���̴���.
							}
							else if (std::wstring::npos != StructText.ToUpper().FindPos(L"SV_TARGET", 0))
							{
								KPTR<KPixelShader> ptr = KPixelShader::Load(_path, NameStartIter->Name, 5, 0);
								if (std::wstring::npos != StructText.ToUpper().FindPos(L"DEFFERDCOLOR", 0))
								{
									ptr->m_IsForward = false;
								}
							}

							Check = true;
						}
						else
						{
							AssertMsg(NameStartIter->Name + L"� ���̴����� ������ �� �� �����ϴ�. 11 ");
						}
					}

					if (false == Check)
					{
						AssertMsg(NameStartIter->Name + L"� ���̴����� ������ �� �� �����ϴ�.  222");
					}
				}
			}

		}
	}
}

void KShader::SetAutoShaderResource(const  KGameString& _ShaderName)
{
	ID3D11ShaderReflection* Reflector = nullptr;

	if (S_OK != D3DReflect(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&Reflector))
	{
		AssertMsg(L"���̴� ���ҽ� ������ �Ұ��� �մϴ�.");
	}

	D3D11_SHADER_DESC ShaderDesc;
	Reflector->GetDesc(&ShaderDesc);

	D3D11_SHADER_INPUT_BIND_DESC ResDesc;
	for (unsigned int i = 0; i < ShaderDesc.BoundResources; i++)
	{
		Reflector->GetResourceBindingDesc(i, &ResDesc);

		switch (ResDesc.Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* Ptr = Reflector->GetConstantBufferByName(ResDesc.Name);

			D3D11_SHADER_BUFFER_DESC SBD;
			Ptr->GetDesc(&SBD);
			InsertShaderData(ResDesc.Name, KShaderData({ nullptr, SBD.Size, m_Type, SHADERRES_TYPE::SR_CBUFFER, ResDesc.BindPoint }));
			break;
		}
		case D3D_SIT_SAMPLER:
		{
			SetSampler(ResDesc.Name, ResDesc.BindPoint);
			break;
		}
		case D3D_SIT_TEXTURE:
		{
			SetTexture(ResDesc.Name, ResDesc.BindPoint);
			break;
		}
		default:
			AssertMsg(L"�Ǵ��Ҽ� ���� ���̴� ���ҽ� Ÿ���Դϴ�.");
			break;
		}
	}
}