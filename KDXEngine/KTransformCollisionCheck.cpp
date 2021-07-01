#include "KTransform.h"

bool KTransform::ColOBB2DToOBB2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;// _Left->OBB2D().Intersects(_Right->OBB2D());
}
bool KTransform::ColOBB2DToSphere2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->OBB2D().Intersects(_Right->SPHERE2D());
}
bool KTransform::ColOBB2DToAABB2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->OBB2D().Intersects(_Right->AABB2D());
}
bool KTransform::ColSphere2DToOBB2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->SPHERE2D().Intersects(_Right->OBB2D());
}
bool KTransform::ColSphere2DToSphere2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->SPHERE2D().Intersects(_Right->SPHERE2D());
}
bool KTransform::ColSphere2DToAABB2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->SPHERE2D().Intersects(_Right->AABB2D());
}
bool KTransform::ColAABB2DToOBB2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->AABB2D().Intersects(_Right->OBB2D());
}
bool KTransform::ColAABB2DToSphere2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->AABB2D().Intersects(_Right->SPHERE2D());
}
bool KTransform::ColAABB2DToAABB2D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return _Left->AABB2D().Intersects(_Right->AABB2D());
}
////////////// 3D

bool KTransform::ColOBB3DToOBB3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->OBB3D().Intersects(_Right->OBB3D());
}
bool KTransform::ColOBB3DToSphere3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->OBB3D().Intersects(_Right->SPHERE3D());
}
bool KTransform::ColOBB3DToAABB3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->OBB3D().Intersects(_Right->AABB3D());
}
bool KTransform::ColSphere3DToOBB3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->SPHERE3D().Intersects(_Right->OBB3D());
}
bool KTransform::ColSphere3DToSphere3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->SPHERE3D().Intersects(_Right->SPHERE3D());
}
bool KTransform::ColSphere3DToAABB3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->SPHERE3D().Intersects(_Right->AABB3D());
}
bool KTransform::ColAABB3DToOBB3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->AABB3D().Intersects(_Right->OBB3D());
}
bool KTransform::ColAABB3DToSphere3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->AABB3D().Intersects(_Right->SPHERE3D());
}
bool KTransform::ColAABB3DToAABB3D(KPTR<KTransform> _Left, KPTR<KTransform> _Right)
{
	return false;//_Left->AABB3D().Intersects(_Right->AABB3D());
}
