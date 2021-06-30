#pragma once
#include "KGameComponentManager.h"
#include "KVertexHeader.h"


enum class KCollisionType
{
	OBB2D,
	ABB2D,
	SPHERE2D,
	POINT2D,
	OBB3D,
	ABB3D,
	SPHERE3D,
	POINT3D,
	MAX
};

class KCamera;
class KRenderManager;
class KRenderPlayer;
class KTransform : public KActorGameComponent
{
private:
	class Starter
	{
	public:
		Starter();
	};

	static Starter m_sStarter;
	static bool(*ColFunc[(int)KCollisionType::MAX][(int)KCollisionType::MAX])(const KPTR<KTransform>& t0, const KPTR<KTransform>& t1);

public:
	friend KRenderManager;
	friend KRenderPlayer;

private:
	KWeakPTR<KTransform> m_Parent;
	std::vector<KPTR<KTransform>> m_Child;


	bool m_bTransUpdate;
	bool ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_MAX)];

	KVector m_localPosition;
	KVector m_localRotation;

	KVector m_localScale;

	KVector m_worldPosition;
	KVector m_worldRotation;
	KVector m_worldScale;

	TransformMatrix m_Matrix;

	bool m_isUpdate;
public:
	KPTR<KTransform> GetChild(int _index);

public:
	void SetFinal(KMatrix _matrix)
	{
		m_Matrix.Final = _matrix;
		m_isUpdate = false;
	}

	TransformMatrix MatrixData()
	{
		return m_Matrix;
	}

	TransformMatrix& RMatrixData()
	{
		return m_Matrix;
	}

	KPTR<KTransform> Parent()
	{
		return m_Parent;
	}

	void Parent(KTransform* Ptr)
	{
		m_Parent = Ptr;
		m_Parent->m_Child.push_back(this);

		CalculateWorldScale();
		CalculateWorldRotation();
		CalculateWorldPosition();
	}

public:
	TransformMatrix* pMatrix()
	{
		return &m_Matrix;
	}

public:
	KVector WorldRotation() const { return m_worldRotation; }
	KVector WorldPosition()const { return m_worldPosition; }
	const KVector& CWorldPosition()const { return m_worldPosition; }
	KVector WorldScale() const { return m_worldScale; }


	KVector LocalRotation() const { return m_localRotation; }
	KVector LocalPosition() const { return m_localPosition; }
	const KVector& CLocalPosition() const { return m_localPosition; }
	KVector LocalScale() const { return m_localScale; }
	KVector LocalQuaternion() const { return m_localRotation.EulerToQuaternion(); }
	KVector WorldQuaternion() const { return m_worldRotation.EulerToQuaternion(); }

	KVector WFORWARD() { return m_Matrix.Final.ArrVector[2].NormalToReturn(); }
	KVector WRIGHT() { return m_Matrix.Final.ArrVector[0].NormalToReturn(); }
	KVector WUP() { return m_Matrix.Final.ArrVector[1].NormalToReturn(); }
	KVector WBACK() { return -m_Matrix.Final.ArrVector[2].NormalToReturn(); }
	KVector WLEFT() { return -m_Matrix.Final.ArrVector[0].NormalToReturn(); }
	KVector WDOWN() { return -m_Matrix.Final.ArrVector[1].NormalToReturn(); }


	KVector LFORWARD() { return m_Matrix.ROT.ArrVector[2]; }
	KVector LRIGHT() { return m_Matrix.ROT.ArrVector[0]; }
	KVector LUP() { return m_Matrix.ROT.ArrVector[1]; }
	KVector LBACK() { return -m_Matrix.ROT.ArrVector[2]; }
	KVector LLEFT() { return -m_Matrix.ROT.ArrVector[0]; }
	KVector LDOWN() { return -m_Matrix.ROT.ArrVector[1]; }

public:
	void CalculateWorldScale()
	{
		m_worldScale = m_Parent->WorldScale() * m_localScale;
	}

	void CalculateWorldScaleChild()
	{
		for (auto& child : m_Child)
		{
			child->CalculateWorldScale();
		}
	}

	void CalculateWorldRotation()
	{
		m_worldRotation = m_Parent->WorldRotation() + m_localRotation;
	}

	void CalculateWorldRotationChild()
	{
		for (auto& child : m_Child)
		{
			child->CalculateWorldRotation();
		}
	}

	void CalculateWorldPosition()
	{
		KVector pos = m_localPosition;

		pos *= m_Parent->WorldScale();
		pos.RotationToDegree(m_Parent->WorldRotation());
		m_worldPosition = m_Parent->WorldPosition() + pos;
	}

	void CalculateWorldPositionChild()
	{
		for (auto& child : m_Child)
		{
			child->CalculateWorldPosition();
		}
	}

	void LocalPosition(const KVector& _value)
	{
		if (nullptr == m_Parent)
		{
			m_localPosition = m_worldPosition = _value;
			CalculateWorldPositionChild();
			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_POS)] = true;
			return;
		}

		m_localPosition = _value;
		CalculateWorldPosition();
		CalculateWorldPositionChild();
		ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_POS)] = true;
	}

	void WorldPosition(const KVector& _value)
	{
		if (nullptr == m_Parent)
		{
			m_localPosition = m_worldPosition = _value;
			CalculateWorldPositionChild();
			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_POS)] = true;

			return;
		}

		KVector pos = _value;

		pos = pos - m_Parent->WorldPosition();
		pos.RotationToDegree(-m_Parent->WorldRotation());
		pos = pos / m_Parent->WorldScale();

		m_localPosition = pos;
		CalculateWorldPosition();
		CalculateWorldPositionChild();
		ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_POS)] = true;
	}

	void LocalRotate(const KVector& _value)
	{
		if (nullptr == m_Parent)
		{
			m_localRotation = m_worldRotation = _value;
			CalculateWorldRotationChild();
			CalculateWorldPositionChild();
			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_ROT)] = true;

			return;
		}

		m_localRotation = _value;
		CalculateWorldRotation();
		CalculateWorldRotationChild();
		CalculateWorldPositionChild();
		ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_ROT)] = true;
	}

	void WorldRotate(const KVector& _value)
	{
		if (nullptr == m_Parent)
		{
			m_localRotation = m_worldRotation = _value;
			CalculateWorldRotationChild();
			CalculateWorldPositionChild();
			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_ROT)] = true;

			return;
		}

		m_localRotation = _value - m_Parent->WorldRotation();
		CalculateWorldRotation();
		CalculateWorldRotationChild();
		CalculateWorldPositionChild();
		ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_ROT)] = true;
	}


	void LocalScale(const KVector& _Value)
	{
		if (nullptr == m_Parent)
		{
			m_localScale = m_worldScale = _Value;
			CalculateWorldScaleChild();
			CalculateWorldPositionChild();
			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_SCALE)] = true;

			return;
		}

		m_localScale = _Value;
		CalculateWorldScale();
		CalculateWorldScaleChild();
		CalculateWorldPositionChild();

		ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_SCALE)] = true;
	}


	void WorldScale(const KVector& _Value)
	{
		if (nullptr == m_Parent)
		{
			m_localScale = m_worldScale = _Value;
			CalculateWorldScaleChild();
			CalculateWorldPositionChild();

			ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_SCALE)] = true;

			return;
		}

		m_localScale = _Value / m_Parent->WorldScale();

		CalculateWorldScale();
		CalculateWorldScaleChild();
		CalculateWorldPositionChild();
		ArrayMatrixUpdate[static_cast<int>(MatrixDDataType::MDT_SCALE)] = true;
	}

	void AddLocalScale(float _Value)
	{
		KVector Scale = m_localScale;
		Scale.x += _Value;
		Scale.y += _Value;
		Scale.z += _Value;
		LocalScale(Scale);
	}

	//////////////// ROT
	void WROTX(const float& _Value) { WorldRotate(KVector(_Value, WorldRotation().y, WorldRotation().z)); }
	void WROTY(const float& _Value) { WorldRotate(KVector(WorldRotation().x, _Value, WorldRotation().z)); }
	void WROTZ(const float& _Value) { WorldRotate(KVector(WorldRotation().x, WorldRotation().y, _Value)); }
	void LROTX(const float& _Value) { LocalRotate(KVector(_Value, LocalRotation().y, LocalRotation().z)); }
	void LROTY(const float& _Value) { LocalRotate(KVector(LocalRotation().x, _Value, LocalRotation().z)); }
	void LROTZ(const float& _Value) { LocalRotate(KVector(LocalRotation().x, LocalRotation().y, _Value)); }

	void WROTADDX(const float& _Value) { WorldRotate(KVector(WorldRotation().x + _Value, WorldRotation().y, WorldRotation().z)); }
	void WROTADDY(const float& _Value) { WorldRotate(KVector(WorldRotation().x, WorldRotation().y + _Value, WorldRotation().z)); }
	void WROTADDZ(const float& _Value) { WorldRotate(KVector(WorldRotation().x, WorldRotation().y, WorldRotation().z + _Value)); }
	void LROTADDX(const float& _Value) { LocalRotate(KVector(LocalRotation().x + _Value, LocalRotation().y, LocalRotation().z)); }
	void LROTADDY(const float& _Value) { LocalRotate(KVector(LocalRotation().x, LocalRotation().y + _Value, LocalRotation().z)); }
	void LROTADDZ(const float& _Value) { LocalRotate(KVector(LocalRotation().x, LocalRotation().y, LocalRotation().z + _Value)); }

	void WMOVE(const KVector& _Value) { WorldPosition(WorldPosition() + _Value); }
	void LMOVE(const KVector& _Value) { LocalPosition(LocalPosition().operator+(_Value)); }

	void WPOSX(const float& _Value) { WorldPosition(KVector(_Value, WorldPosition().y, WorldPosition().z)); }
	void WPOSY(const float& _Value) { WorldPosition(KVector(WorldPosition().x, _Value, WorldPosition().z)); }
	void WPOSZ(const float& _Value) { WorldPosition(KVector(WorldPosition().x, WorldPosition().y, _Value)); }
	void LPOSX(const float& _Value) { LocalPosition(KVector(_Value, LocalPosition().y, LocalPosition().z)); }
	void LPOSY(const float& _Value) { LocalPosition(KVector(LocalPosition().x, _Value, LocalPosition().z)); }
	void LPOSZ(const float& _Value) { LocalPosition(KVector(LocalPosition().x, LocalPosition().y, _Value)); }

	void WPOSADDX(const float& _Value) { WorldPosition(KVector(WorldPosition().x + _Value, WorldPosition().y, WorldPosition().z)); }
	void WPOSADDY(const float& _Value) { WorldPosition(KVector(WorldPosition().x, WorldPosition().y + _Value, WorldPosition().z)); }
	void WPOSADDZ(const float& _Value) { WorldPosition(KVector(WorldPosition().x, WorldPosition().y, WorldPosition().z + _Value)); }
	void LPOSADDX(const float& _Value) { LocalPosition(KVector(LocalPosition().x + _Value, LocalPosition().y, LocalPosition().z)); }
	void LPOSADDY(const float& _Value) { LocalPosition(KVector(LocalPosition().x, LocalPosition().y + _Value, LocalPosition().z)); }
	void LPOSADDZ(const float& _Value) { LocalPosition(KVector(LocalPosition().x, LocalPosition().y, LocalPosition().z + _Value)); }

public:
	void Init() override;
	void UpdateTransform();

public:
	bool Col(KCollisionType _thisType, KCollisionType _otherType, KPTR<KTransform> t1);

private:
	static bool ColOBB2DToOBB2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColOBB2DToSphere2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColOBB2DToAABB2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColSphere2DToOBB2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColSphere2DToSphere2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColSphere2DToAABB2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColAABB2DToOBB2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColAABB2DToSphere2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColAABB2DToAABB2D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);

	static bool ColOBB3DToOBB3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColOBB3DToSphere3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColOBB3DToAABB3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColSphere3DToOBB3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColSphere3DToSphere3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColSphere3DToAABB3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColAABB3DToOBB3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColAABB3DToSphere3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);
	static bool ColAABB3DToAABB3D(const KPTR<KTransform>& _t0, const KPTR<KTransform>& t1);

public:
	DirectX::BoundingBox AABB2D() const;
	DirectX::BoundingOrientedBox OBB2D() const;
	DirectX::BoundingSphere SPHERE2D() const;

	DirectX::BoundingBox AABB3D() const;
	DirectX::BoundingOrientedBox OBB3D() const;
	DirectX::BoundingSphere SPHERE3D() const;

public:
	void SetCameraMatrix(KPTR<KCamera> _ViewCam);
	void SetLightMatrix(KPTR<KLightManager> _light);
	void CalculateMatrixData();

public:
	KTransform();
};
