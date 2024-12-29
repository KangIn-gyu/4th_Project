#pragma once
class IndexBuffer
{
public:
	IndexBuffer() {};
	~IndexBuffer() {};

	void Create(const std::vector<DWORD>& _indices);
private:

public:
	std::vector<DWORD> indices;

private:
	ComPtr<ID3D11Buffer> indexBuffer;
};

