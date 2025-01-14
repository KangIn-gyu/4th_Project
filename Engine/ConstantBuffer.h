#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

	void Create(size_t _size); // 구조체 용량 넣으면 됨.
	ComPtr<ID3D11Buffer> GetBuffer() { return constantBuffer; }

private:

public:

private:
	ComPtr<ID3D11Buffer> constantBuffer;
};

// 상수 버퍼 
// 셰이더에 상수 데이터를 전달하기 위해 사용되는 버퍼, 주로 변하지 않는 데이터를 저장하는데 사용
// 한번 설정하면 여러 셰이더에서 재사용할 수 있어 효울적인 데이터 관리가 가능.

// 나중되면 각 레지스터 슬롯 마다 맞는 버퍼를 사용할텐데
// 이 버퍼가 어떤 타입인줄 알고 몇번 슬롯을 사용할지 모르는 문제가 있다.