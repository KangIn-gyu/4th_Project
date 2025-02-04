// (2가지 샘플러 생성)
// 각 샘플러 마다의 용도
// Linear(선형 보간) : 부드럽고 고품질 텍스처 샘플링
// WRAP(반복)
// PBR, 타일링 텍스처
SamplerState samLinear : register(s0);

// Point(최근점 샘플링) : 픽셀화, 빠르지만 저품질
// CLAMP(경계 제한)
// 그림자 맵, UI, 스프라이트
SamplerComparisonState samPoint : register(s1);

// 일단 모르겠어서 그냥 하기
SamplerComparisonState shadowSampler : register(s2);