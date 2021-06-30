#pragma once
#include "KGameResource.h"
#include "KVertexBuffer.h"
#include "KIndexBuffer.h"

class KVertexBuffer;
class KIndexBuffer;
class KMesh : public KGameResource<KMesh>
{
private:
	std::vector<KPTR<KVertexBuffer>> m_vertexBufferContainer;
	std::vector<KPTR<KIndexBuffer>> m_indexBufferContainer;

public:
	static void Create(const KGameString& _Name)
	{
		KMesh* NewBuffer = new KMesh();
		NewBuffer->SetName(_Name);
		NewBuffer->Create(_Name, _Name);
		NewBuffer->InsertResource();
	}

	static void Create(const KGameString& _Name, const KGameString& _VBName, const KGameString& _IBName)
	{
		KMesh* NewBuffer = new KMesh();
		NewBuffer->SetName(_Name);
		NewBuffer->Create(_VBName, _IBName);
		NewBuffer->InsertResource();
	}

public:
	void AddVertexBuffer(const KGameString& _vertexBufferName);
	void AddVertexBuffer(const KPTR<KVertexBuffer>& _vertexBuffer);
	void AddIndexBuffer(const KGameString& _indexBufferName);
	void AddIndexBuffer(const KPTR<KIndexBuffer>& _indexBuffer);

public:
	void Render(D3D11_PRIMITIVE_TOPOLOGY _TOPLOGY, size_t _VBIndex = 0, size_t _IBIndex = 0);

public:
	void Create(const KGameString& _VBName, const KGameString& _IBName);

};

