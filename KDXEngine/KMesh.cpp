#include "KMesh.h"
#include "KVertexBuffer.h"
#include "KIndexBuffer.h"


void KMesh::Create(const KGameString& _VBName, const KGameString& _IBName)
{
	AddVertexBuffer(_VBName);
	AddIndexBuffer(_IBName);
}

void KMesh::AddVertexBuffer(const KGameString& _vertexBufferName)
{
	KPTR<KVertexBuffer> VBPTR = KVertexBuffer::Find(_vertexBufferName);

	if (nullptr == VBPTR.get())
	{
		AssertMsg(L"존재하지 않는 버텍스 버퍼를 사용하려고 했습니다.");
	}

	m_vertexBufferContainer.push_back(VBPTR);
}

void KMesh::AddVertexBuffer(const KPTR<KVertexBuffer>& _vertexBuffer)
{
	if (nullptr == _vertexBuffer.get())
	{
		AssertMsg(L"if (nullptr == _vertexBuffer)");
	}

	m_vertexBufferContainer.push_back(_vertexBuffer);
}

void KMesh::AddIndexBuffer(const KGameString& _indexBufferName)
{
	KPTR<KIndexBuffer> indexBufferPtr = KIndexBuffer::Find(_indexBufferName);

	if (nullptr == indexBufferPtr.get())
	{
		AssertMsg(L"존재하지 않는 인덱스 버퍼를 사용하려고 했습니다.");
	}

	m_indexBufferContainer.push_back(indexBufferPtr);
}

void KMesh::AddIndexBuffer(const KPTR<KIndexBuffer>& _indexBuffer)
{
	if (nullptr == _indexBuffer.get())
	{
		AssertMsg(L"if (nullptr == _indexBuffer)");
	}

	m_indexBufferContainer.push_back(_indexBuffer);
}


void KMesh::Render(D3D11_PRIMITIVE_TOPOLOGY _TOPLOGY, size_t _VBIndex, size_t _IBIndex)
{
	m_vertexBufferContainer[_VBIndex]->Setting();
	m_indexBufferContainer[_IBIndex]->Setting();
	KGameDevice::MainContext()->IASetPrimitiveTopology(_TOPLOGY);
	KGameDevice::MainContext()->DrawIndexed(m_indexBufferContainer[0]->Count(), 0, 0);
}