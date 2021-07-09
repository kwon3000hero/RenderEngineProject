#include "KTransform.h"
#include "KCamera.h"
#include "KLight.h"

bool (*KTransform::ColFunc[(int)KCollisionType::MAX][(int)KCollisionType::MAX])(KPTR<KTransform> t0, KPTR<KTransform> t1) = { nullptr, };

KTransform::Starter::Starter()
{
	ColFunc[(int)KCollisionType::OBB2D][(int)KCollisionType::OBB2D] = ColOBB2DToOBB2D;
	ColFunc[(int)KCollisionType::OBB2D][(int)KCollisionType::ABB2D] = ColOBB2DToAABB2D;
	ColFunc[(int)KCollisionType::OBB2D][(int)KCollisionType::SPHERE2D] = ColOBB2DToSphere2D;
	ColFunc[(int)KCollisionType::ABB2D][(int)KCollisionType::OBB2D] = ColAABB2DToOBB2D;
	ColFunc[(int)KCollisionType::ABB2D][(int)KCollisionType::ABB2D] = ColAABB2DToAABB2D;
	ColFunc[(int)KCollisionType::ABB2D][(int)KCollisionType::SPHERE2D] = ColAABB2DToSphere2D;
	ColFunc[(int)KCollisionType::SPHERE2D][(int)KCollisionType::OBB2D] = ColSphere2DToOBB2D;
	ColFunc[(int)KCollisionType::SPHERE2D][(int)KCollisionType::ABB2D] = ColSphere2DToAABB2D;
	ColFunc[(int)KCollisionType::SPHERE2D][(int)KCollisionType::SPHERE2D] = ColSphere2DToSphere2D;

	ColFunc[(int)KCollisionType::OBB3D][(int)KCollisionType::OBB3D] = ColOBB3DToOBB3D;
	ColFunc[(int)KCollisionType::OBB3D][(int)KCollisionType::ABB3D] = ColOBB3DToAABB3D;
	ColFunc[(int)KCollisionType::OBB3D][(int)KCollisionType::SPHERE3D] = ColOBB3DToSphere3D;
	ColFunc[(int)KCollisionType::ABB3D][(int)KCollisionType::OBB3D] = ColAABB3DToOBB3D;
	ColFunc[(int)KCollisionType::ABB3D][(int)KCollisionType::ABB3D] = ColAABB3DToAABB3D;
	ColFunc[(int)KCollisionType::ABB3D][(int)KCollisionType::SPHERE3D] = ColAABB3DToSphere3D;
	ColFunc[(int)KCollisionType::SPHERE3D][(int)KCollisionType::OBB3D] = ColSphere3DToOBB3D;
	ColFunc[(int)KCollisionType::SPHERE3D][(int)KCollisionType::ABB3D] = ColSphere3DToAABB3D;
	ColFunc[(int)KCollisionType::SPHERE3D][(int)KCollisionType::SPHERE3D] = ColSphere3DToSphere3D;

}

KTransform::KTransform() : m_localScale({ 1.0f, 1.0f, 1.0f, 1.0f }), m_worldScale({ 1.0f, 1.0f, 1.0f, 1.0f }), ArrayMatrixUpdate{ 0, }, m_isUpdate(true)
{
	m_Matrix.Init();
}

KTransform::~KTransform()
{

}

KPTR<KTransform> KTransform::GetChild(int _index)
{
	return m_Child[_index];
}

void KTransform::Init()
{
	if (nullptr != Actor()->GetComponent<KTransform>().get())
	{
		assert(false);
	}

	Actor()->SetTransform(KPTR<KTransform>(this));
	Scene()->PushTransform(KPTR<KTransform>(this));
}

void KTransform::UpdateTransform()
{
	if (true == m_isUpdate)
	{
		if (0.0f == m_localScale.x || 0.0f == m_localScale.y || 0.0f == m_localScale.z ||
			0.0f == m_worldScale.x || 0.0f == m_worldScale.y || 0.0f == m_worldScale.z)
		{
			AssertMsg(L"스케일은 0을 주지 맙시다.");
		}

		if (true == ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_SCALE)])
		{
			m_Matrix.SCALE.Scale(m_localScale);
			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_SCALE)] = false;
			m_bTransUpdate = true;
		}

		if (true == ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_ROT)])
		{
			m_Matrix.ROT.RotDegree(m_localRotation);
			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_ROT)] = false;
			m_bTransUpdate = true;
		}

		if (true == ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_POS)])
		{
			m_Matrix.POS.Pos(m_localPosition);
			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_POS)] = false;
			m_bTransUpdate = true;
		}

		if (true == m_bTransUpdate || (nullptr != m_Parent && true == m_Parent->m_bTransUpdate))
		{
			m_Matrix.CalculateFinalMatrix();

			m_bTransUpdate = true;

			CalculateWorldScaleChild();
			CalculateWorldRotationChild();
			CalculateWorldPositionChild();
		}
	}
	else
	{
		m_bTransUpdate = true;
	}


	for (int i = 0; i < m_Child.size(); ++i)
	{
		m_Child[i]->m_Matrix.Parent = m_Matrix.Final;
		m_Child[i]->UpdateTransform();
	}

	m_bTransUpdate = false;
}

bool KTransform::Col(KCollisionType _ThisType, KCollisionType _OtherType, KPTR<KTransform> _OtherTrans)
{
	return ColFunc[(int)_ThisType][(int)_OtherType](KPTR<KTransform>(this), _OtherTrans);
}

void KTransform::CalculateMatrixData()
{
	m_Matrix.CalculateTransformMatrix();
}

void KTransform::SetCameraMatrix(KPTR<KCamera> _ViewCam)
{
	m_Matrix.VIEW = _ViewCam->m_ViewMat;
	m_Matrix.PROJ = _ViewCam->m_ProjMat;
	m_Matrix.CalculateTransformMatrix();
}

void KTransform::SetLightMatrix(KPTR<KLightManager> _light)
{
	m_Matrix.VIEW = _light->View();
	m_Matrix.PROJ = _light->Projection();
	m_Matrix.CalculateTransformMatrix();
}

DirectX::BoundingBox KTransform::AABB2D()  const
{
	return DirectX::BoundingBox(WorldPosition(), WorldScale().HalfVector3D().AbsVector());
}
DirectX::BoundingOrientedBox KTransform::OBB2D() const
{
	return DirectX::BoundingOrientedBox(WorldPosition().Vector2D(), WorldScale().HalfVector3D().AbsVector(), WorldRotation().DegToQReturn());
}
DirectX::BoundingSphere KTransform::SPHERE2D() const
{
	return DirectX::BoundingSphere(WorldPosition().Vector2D(), WorldScale().HX());
}

DirectX::BoundingBox KTransform::AABB3D() const
{
	return DirectX::BoundingBox(WorldPosition(), WorldScale().HalfVector3D().AbsVector());
}
DirectX::BoundingOrientedBox KTransform::OBB3D() const
{
	return DirectX::BoundingOrientedBox(WorldPosition(), WorldScale().HalfVector3D().AbsVector(), WorldRotation().DegToQReturn());
}
DirectX::BoundingSphere KTransform::SPHERE3D() const
{
	return DirectX::BoundingSphere(WorldPosition(), WorldScale().HX());
}