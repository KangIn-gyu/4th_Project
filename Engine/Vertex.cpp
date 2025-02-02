#include "pch.h"
#include "Vertex.h"
#include "Helper.h"

void Vertex::LoadAiMeshToVertex(aiMesh* _aiMesh, unsigned int _index)
{
	if (_aiMesh->HasPositions())   // Pos
	{
		position = { _aiMesh->mVertices[_index].x,  _aiMesh->mVertices[_index].y,  _aiMesh->mVertices[_index].z };
	}

	if (_aiMesh->HasNormals()) // ³ë¸»
	{
		normal = { _aiMesh->mNormals[_index].x, _aiMesh->mNormals[_index].y, _aiMesh->mNormals[_index].z };
		if (normal.Length() > 0.0f)
		{
			normal.Normalize();
		}
	}

	if (_aiMesh->HasTangentsAndBitangents())
	{
		tangent = { _aiMesh->mTangents[_index].x , _aiMesh->mTangents[_index].y, _aiMesh->mTangents[_index].z };           // ÅºÁ¨Æ® 
		if (tangent.Length() > 0.0f)
		{
			tangent.Normalize();
		}
		binormal = { _aiMesh->mBitangents[_index].x,_aiMesh->mBitangents[_index].y , _aiMesh->mBitangents[_index].z };     // biÅºÁ¨Æ®
		if (binormal.Length() > 0.0f)
		{
			binormal.Normalize();
		}
	}

	if (_aiMesh->HasVertexColors(0))
	{
		color = { _aiMesh->mColors[0][_index].r, _aiMesh->mColors[0][_index].g, _aiMesh->mColors[0][_index].b, _aiMesh->mColors[0][_index].a }; // ÄÃ·¯
	}

	if (_aiMesh->mTextureCoords[0]) // uv
	{
		uv = { _aiMesh->mTextureCoords[0][_index].x, _aiMesh->mTextureCoords[0][_index].y };
	}
	else
	{
		uv = { 0.0f, 0.0f };
	}
}
