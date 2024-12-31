#pragma once
class IndexBuffer
{
public:
	IndexBuffer() {};
	~IndexBuffer() {};

	void Create(const std::vector<DWORD>& _indices);
private:

public:
	

private:
	std::vector<DWORD> indices;
	ComPtr<ID3D11Buffer> indexBuffer;
};

