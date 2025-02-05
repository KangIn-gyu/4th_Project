#pragma once

class AABB
{
public:
	AABB() : Center{ 0,0 }, Extent{ 0,0 } {}
public:
	D2D_VECTOR_2F Center; // 중앙
	D2D_VECTOR_2F Extent; // x,y축 확장값	

	AABB(const AABB&) = default;
	AABB& operator=(const AABB&) = default;
	AABB(AABB&&) = default;
	AABB& operator=(AABB&&) = default;
	~AABB() = default;

	void SetCenter(float x, float y) { Center = { x, y }; }
	void SetExtent(float x, float y) { Extent = { x, y }; }
	float GetMinX() const { return Center.x - Extent.x; }
	float GetMaxX() const { return Center.x + Extent.x; }
	float GetMinY() const { return Center.y - Extent.y; }
	float GetMaxY() const { return Center.y + Extent.y; }
};


