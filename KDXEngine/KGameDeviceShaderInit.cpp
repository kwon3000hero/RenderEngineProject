#include "KGAMEDEVICE.h"
#include "KGAMEDIR.h"
#include "KVertexShader.h"
#include "KPixelShader.h"
#include "KTransform.h"
#include "KCuttingRender.h"

void KGameDevice::InitShader()
{
	{
		KGAMEDIR Dir;

		Dir.MoveParent(L"KCM3D");
		Dir.Move(L"RES");
		Dir.Move(L"Shader");

		std::list<KGameFile> shaderFile = Dir.DirAllFile(L"hlsl");

		for (auto& ShaderFile : shaderFile)
		{
			KShader::ParsingToShaderData(ShaderFile.m_Path);
		}
	}
}