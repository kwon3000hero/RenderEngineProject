#pragma once
#include "KGameDevice.h"
#include "KGAMEDIR.h"
#include "KVertexShader.h"
#include "KPixelShader.h"
#include "KTransform.h"
#include "KCuttingRender.h"

template<typename T>
void KGameDevice<T>::InitShader()
{
	{
		KGAMEDIR Dir;

		Dir.MoveParent(L"RenderEngineProject");
		Dir.Move(L"RES");
		Dir.Move(L"Shader");

		std::list<KGameFile> shaderFile = Dir.DirAllFile(L"hlsl");

		for (auto& ShaderFile : shaderFile)
		{
			KShader::ParsingToShaderData(ShaderFile.m_Path);
		}
	}
}