#include "pch.h"
#include "VertexBuffer.h"

int VertexBuffer::GetVertexSize()
{
	if (!vertices.empty())
	{
		return vertices.size();
	}

	if (!boneWeightVertices.empty())
	{
		return boneWeightVertices.size();
	}

	// TODO : 로그시스템으로 추후 처리
	std::cout << "버텍스와 본웨이트버텍스가 둘다 비워있습니다.\n";
	return 0;
}
