#pragma once
class IndexBuffer
{
public:
	IndexBuffer() {};
	~IndexBuffer();

	void Create(const std::vector<DWORD>& _indices);
	ComPtr<ID3D11Buffer> GetBuffer() { return indexBuffer; }
	int GetIndexCount() { return indexCount; }
private:

public:
	std::vector<DWORD> indices;

private:

	ComPtr<ID3D11Buffer> indexBuffer;
	int indexCount{};
};

