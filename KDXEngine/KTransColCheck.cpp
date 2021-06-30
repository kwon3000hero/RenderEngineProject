#include "KTransform.h"

bool KTransform::ColOBB2DToOBB2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->OBB2D().Intersects(_Right->OBB2D());
}
bool KTransform::ColOBB2DToSphere2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->OBB2D().Intersects(_Right->SPHERE2D());
}
bool KTransform::ColOBB2DToAABB2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->OBB2D().Intersects(_Right->AABB2D());
}
bool KTransform::ColSphere2DToOBB2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->SPHERE2D().Intersects(_Right->OBB2D());
}
bool KTransform::ColSphere2DToSphere2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->SPHERE2D().Intersects(_Right->SPHERE2D());
}
bool KTransform::ColSphere2DToAABB2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->SPHERE2D().Intersects(_Right->AABB2D());
}
bool KTransform::ColAABB2DToOBB2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->AABB2D().Intersects(_Right->OBB2D());
}
bool KTransform::ColAABB2DToSphere2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->AABB2D().Intersects(_Right->SPHERE2D());
}
bool KTransform::ColAABB2DToAABB2D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->AABB2D().Intersects(_Right->AABB2D());
}
////////////// 3D

bool KTransform::ColOBB3DToOBB3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->OBB3D().Intersects(_Right->OBB3D());
}
bool KTransform::ColOBB3DToSphere3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->OBB3D().Intersects(_Right->SPHERE3D());
}
bool KTransform::ColOBB3DToAABB3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->OBB3D().Intersects(_Right->AABB3D());
}
bool KTransform::ColSphere3DToOBB3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->SPHERE3D().Intersects(_Right->OBB3D());
}
bool KTransform::ColSphere3DToSphere3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->SPHERE3D().Intersects(_Right->SPHERE3D());
}
bool KTransform::ColSphere3DToAABB3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->SPHERE3D().Intersects(_Right->AABB3D());
}
bool KTransform::ColAABB3DToOBB3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->AABB3D().Intersects(_Right->OBB3D());
}
bool KTransform::ColAABB3DToSphere3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->AABB3D().Intersects(_Right->SPHERE3D());
}
bool KTransform::ColAABB3DToAABB3D(const KPTR<KTransform>& _Left, const KPTR<KTransform>& _Right)
{
	return _Left->AABB3D().Intersects(_Right->AABB3D());
}
